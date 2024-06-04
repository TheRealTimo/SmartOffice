#include "ICM42688.h"


const float motionThreshold = 0.0005; //The threshold at which motion is detected

const int initialisationTimeInSeconds = 10;
const int sampleSpeedMs = 10;

float maxAccelZ;
float minAccelZ;

#include "wifi.h"
#include "led.h"
#include "mqtt.h"
#include "imu.h"
#include "optout.h"

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
  IMU.getAGT();
  float currentAccelZ = IMU.accZ();

 if (currentAccelZ > (maxAccelZ + motionThreshold) || 
      currentAccelZ < (minAccelZ - motionThreshold)) { 
    Serial.print("Motion detected, ");
    updateLedStatus(OPERATIONAL_PRESENCE);
    delay(1000);
    updateLedStatus(OPERATIONAL_NO_PRESENCE);
  }
  delay(sampleSpeedMs);
}