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

void setup(void){
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  updateLedStatus(SETUP);

  pinMode(optOutButtonPin, INPUT);

  connectToWifi();
  connectMqtt();

  setupImu();
  initializeMotionRange();
}

void loop(){
  if (isOptOutTimerRunning) { startOptOutTimer(); }
  if (!mqttClient.connected()) {
    connectMqtt();
  }
  mqttClient.loop();


  IMU.getAGT();
  float currentAccelZ = IMU.accZ();  //Get the current acceleration on the Z axis

  static int positiveReadingsCount = 0;
  static int totalReadingsCount = 0;

  if (currentAccelZ > (maximumAccelerationZAxiz + MOTION_THRESHOLD_DEFAULT_VALUE) || currentAccelZ < (minimumAccelerationZAxiz - MOTION_THRESHOLD_DEFAULT_VALUE)) {
    positiveReadingsCount++;
  }

  totalReadingsCount++;

  if (positiveReadingsCount >= MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE && totalReadingsCount <= MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE) {
    lastMotionDetectionTime = millis();

    if (!isDeskOccupied) {  //Only update status if a change is detected
      isDeskOccupied = true;
      updateLedStatus(OPERATIONAL_PRESENCE);
      turnSmartSwitchOn(true);
      publishOccupancyStatusToMqtt();
    }

    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  } else if (totalReadingsCount >= MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE) {
    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  }

  if (isDeskOccupied && (millis() - lastMotionDetectionTime) > (INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE * 60 * 1000)) {
    isDeskOccupied = false;
    updateLedStatus(OPERATIONAL_NO_PRESENCE);
    turnSmartSwitchOn(false);
    publishOccupancyStatusToMqtt();
  }

  delay(SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE);
}