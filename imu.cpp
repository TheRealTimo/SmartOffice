/*
Description: Initialises the IMU and takes baseline readings for the dynamic range
*/

#include "imu.h"

ICM42688 IMU(SPI, 15);  //Chip select/Slave select(CS/SS) on pin 15

float minimumAccelerationZAxiz;
float maximumAccelerationZAxiz;

/*
 *  Function: setupImu
 *  Description: Connects the ESP to the IMU via SPI. Attempts multiple connections in case of failure. 
 *  Parameters: None
 *  Returns: None
 */
void setupImu() {
  updateLedStatus(SETUP);
  int status;
  for (uint8_t attempt = 0; attempt < IMU_CONNECTION_ATTEMPTS; attempt++) {
    status = IMU.begin();
    if (status >= 0) {
      break;
    }

    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    updateLedStatus(ERROR);
    delay(500);
  }

  // Check final status after all attempts
  if (status < 0) {
    Serial.println("IMU initialization failed after all attempts.");
    ledBlinkError();
  } else {
    Serial.println("IMU initialization successful!");
    updateLedStatus(SETUP);
  }
}

/*
 *  Function: initializeMotionRange
 *  Description: Reads the acceleration values along the Z-axis for a define dtime whilst finding the minimum and maximum values to calibrate the dynamic range
 *  Parameters: None
 *  Returns: None
 */
void initializeMotionRange() {
  Serial.println("Calibrating");
  Serial.println("Please stand back");
  Serial.println("-->");
  updateLedStatus(CALIBRATING);

  IMU.getAGT();

  float firstReading = IMU.accZ();
  maximumAccelerationZAxiz = firstReading;
  minimumAccelerationZAxiz = firstReading;

  for (int i = 0; i < IMU_CALIBRATION_TIME_IN_SECONDS * 1000; i++) {
    IMU.getAGT();
    float accelZ = IMU.accZ();

    if (accelZ > maximumAccelerationZAxiz) {
      maximumAccelerationZAxiz = accelZ;
    }
    if (accelZ < minimumAccelerationZAxiz) {
      minimumAccelerationZAxiz = accelZ;
    }

    Serial.print(".");

    delay(1);
  }
  updateLedStatus(SETUP);
  Serial.print("\nMaxAccelZ:");
  Serial.println(maximumAccelerationZAxiz, 6);
  Serial.print("MinAccelZ:");
  Serial.println(minimumAccelerationZAxiz, 6);
}