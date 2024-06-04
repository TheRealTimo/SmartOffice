/*
  Project:        Smart Office Prototype
  Author:         Timo Hennig
  Date Created:   24.05.2024
  Last Modified:  24.05.2024
  Version:        0.1
  License:        All rights reserved

  Description:
    This sketch uses an ICM42688 IMU connected via SPI to read acceleratuon along the z-axis in order to sense vibrations on a desk surface.
    If vibrations are detected, occupancy status will be shared via MQTT.
*/

#include "wifi.h"
#include "led.h"
#include "mqtt.h"
#include "config.h"
#include "imu.h"
#include "eeprom.h"
#include "optout.h"
#include "variables.h"

void ICACHE_RAM_ATTR optOutButtonPressed() {
  isOptOutTimerRunning = !isOptOutTimerRunning;
}

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  updateLedStatus(SETUP);

  Serial.begin(115200);
  Serial.println("\nSmart Office Prototype");
  Serial.println("Setting up as " + String(MQTT_CONTROLLER_ID));

  pinMode(optOutButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(optOutButtonPin), optOutButtonPressed, RISING);

  EEPROM.begin(EEPROM_SIZE);
  checkSetupFlag();
  readEeprom();

  connectToWifi();
  connectMqtt();
  publishOccupancyStatusToMqtt();
  publishTelemetryToMqtt();
  publishCommandValuesToMqtt();
  setupImu();
  initializeMotionRange();
  updateLedStatus(OPERATIONAL_NO_PRESENCE);
}

void loop() {
  if (isOptOutTimerRunning) { startOptOutTimer(); }
  if (!mqttClient.connected()) {
    connectMqtt();
  }
  mqttClient.loop();


  IMU.getAGT();
  float currentAccelZ = IMU.accZ();  //Get the current acceleration on the Z axis

  static uint8_t positiveReadingsCount = 0;
  static uint8_t totalReadingsCount = 0;

  if (currentAccelZ > (maximumAccelerationZAxiz + motionThreshold) || currentAccelZ < (minimumAccelerationZAxiz - motionThreshold)) {  //Motion detected
    positiveReadingsCount++;
  }

  totalReadingsCount++;

  if (positiveReadingsCount >= minimumMotionDetectionCountRequired && totalReadingsCount <= minimumMotionDetectionCountTimeframe) {
    lastMotionDetectionTime = millis();

    if (!isDeskOccupied) {  //Only update status if a change is detected
      isDeskOccupied = true;
      updateLedStatus(OPERATIONAL_PRESENCE);
      turnSmartSwitchOn(true);
    }

    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  } else if (totalReadingsCount >= minimumMotionDetectionCountTimeframe) { //Reset variables 
    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  }

  if (isDeskOccupied && (millis() - lastMotionDetectionTime) > (inactivityTimeoutInMinutes * 60 * 1000)) { //Check if the inactivity requirements have been met
    isDeskOccupied = false;
    updateLedStatus(OPERATIONAL_NO_PRESENCE);
    turnSmartSwitchOn(false);
  }

  delay(sampleSpeedInMilliseconds);
}