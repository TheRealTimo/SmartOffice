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
  if (currentAccelZ > (maximumAccelerationZAxiz + MOTION_THRESHOLD_) || currentAccelZ < (minimumAccelerationZAxiz - MOTION_THRESHOLD_)) {
    lastMotionDetectionTime = millis();
    if (!isDeskOccupied) {
      isDeskOccupied = true;
      updateLedStatus(OPERATIONAL_PRESENCE);
      mqttClient.publish("smartOffice/plugs/cmnd/plug1/POWER", "1");
    }
  } else {
    if (isDeskOccupied && (millis() - lastMotionDetectionTime) > (INACTIVITY_TIMEOUT_IN_MINUTES * 60 * 1000)) {
      isDeskOccupied = false;
      updateLedStatus(OPERATIONAL_NO_PRESENCE);
      mqttClient.publish("smartOffice/plugs/cmnd/plug1/POWER", "0");
    }
  }
  delay(SAMPLE_SPEED_IN_MILLISECONDS);
}