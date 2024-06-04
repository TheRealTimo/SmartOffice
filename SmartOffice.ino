#include "ICM42688.h"

ICM42688 IMU(SPI, 15);

const float motionThreshold = 0.0005; //The threshold at which motion is detected

const int initialisationTimeInSeconds = 10;
const int sampleSpeedMs = 10;


float maxAccelZ;
float minAccelZ;

int detectionCounter = 0;

void setup(void){
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
  // Serial.print("Current Reading: ");
  // Serial.println(currentAccelZ);

 if (currentAccelZ > (maxAccelZ + motionThreshold) || 
      currentAccelZ < (minAccelZ - motionThreshold)) { 
    Serial.print("Motion detected, ");
    Serial.print("Current Reading: ");
    Serial.print(currentAccelZ, 6);
    Serial.print(", Difference: ");
    

    //Debug
    float difference;

    if (currentAccelZ > (maxAccelZ + motionThreshold)) {
        difference = currentAccelZ - (maxAccelZ + motionThreshold);
    } else {
        difference = (minAccelZ - motionThreshold) - currentAccelZ;
    }
    Serial.print(difference, 6);
    Serial.print(", Counter: ");
    Serial.println(detectionCounter);
    detectionCounter++;


  }

  delay(sampleSpeedMs);
}


void initializeMotionRange() {
  Serial.println("Calibrating");
  Serial.println("Please stand back");
  delay(5000); //Time to step back
  Serial.println("-->");
  digitalWrite(LED_BUILTIN, LOW);

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
  Serial.println("");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("MaxAccelZ:");
  Serial.println(maxAccelZ, 6);
  Serial.print("MinAccelZ:");
  Serial.println(minAccelZ, 6);
  delay(2500); // Time to read values
}