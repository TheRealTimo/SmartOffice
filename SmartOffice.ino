#include "ICM42688.h"

ICM42688 IMU(SPI, 15);

const float motionThreshold = 0.0005; //The threshold at which motion is detected

const int initialisationTimeInSeconds = 10;
const int sampleSpeedMs = 10;

float maxAccelZ;
float minAccelZ;

#include "wifi.h"
#include "led.h"
#include "mqtt.h"

void setup(void){
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  updateLedStatus(SETUP);

  connectToWifi();
  connectMqtt();

  Serial.begin(115200);
  while (!Serial)
    delay(10); 
  Serial.println("ICM42688 Advanced Motion Detection Test");

  int status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  initializeMotionRange();
}

void loop(){
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

void initializeMotionRange() {
  updateLedStatus(CALIBRATING);

  IMU.getAGT();

  float firstReading = IMU.accZ();
  maxAccelZ = firstReading;
  minAccelZ = firstReading;

  for (int i = 0; i <  initialisationTimeInSeconds * 1000; i++) {
    IMU.getAGT();
    float accelZ = IMU.accZ();

    // Update min and max
    if (accelZ > maxAccelZ) { 
        maxAccelZ = accelZ;
    }
    if (accelZ < minAccelZ) {
        minAccelZ = accelZ;
    } 

    Serial.print(".");

    delay(1); 
    
  }
updateLedStatus(SETUP);
}