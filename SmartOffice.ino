#include "ICM42688.h"

float maxAccelZ;
float minAccelZ;

#include "wifi.h"
#include "led.h"
#include "mqtt.h"
#include "imu.h"
#include "optout.h"
#include "variables.h"

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
  if(optOutButtonPin == HIGH){
    startOptOutTimer();
  }
  if (!mqttClient.connected()) {
    connectMqtt();
  }
  mqttClient.loop();


  IMU.getAGT();
  float currentAccelZ = IMU.accZ();

  static int positiveReadingsCount = 0;
  static int totalReadingsCount = 0;

  if (currentAccelZ > (maximumAccelerationZAxiz + MOTION_THRESHOLD) || currentAccelZ < (MINIMUM_MOTION_DETECTION_COUNT_REQUIRED - MOTION_THRESHOLD)) {
    positiveReadingsCount++;
  }

  totalReadingsCount++;

  if (positiveReadingsCount >= MINIMUM_MOTION_DETECTION_COUNT_REQUIRED && totalReadingsCount <= MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_TIMEFRAME) {
    lastMotionDetectionTime = millis();

    if (!isDeskOccupied) {
      isDeskOccupied = true;
      updateLedStatus(OPERATIONAL_PRESENCE);
      turnSmartSwitchOn(true);
      publishOccupancyStatusToMqtt();
    }

    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  } else if (totalReadingsCount >= MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_TIMEFRAME) {
    positiveReadingsCount = 0;
    totalReadingsCount = 0;
  }

  if (isDeskOccupied && (millis() - lastMotionDetectionTime) > (INACTIVITY_TIMEOUT_IN_MINUTES * 60 * 1000)) {
    isDeskOccupied = false;
    updateLedStatus(OPERATIONAL_NO_PRESENCE);
    turnSmartSwitchOn(false);
    publishOccupancyStatusToMqtt();
  }

  delay(SAMPLE_SPEED_IN_MILLISECONDS);
}