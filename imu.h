#ifndef IMU_H
#define IMU_H

#include "ICM42688.h"
#include "config.h"
#include "led.h"

extern ICM42688 IMU;

extern float maximumAccelerationZAxiz;
extern float minimumAccelerationZAxiz;

void setupImu();
void initializeMotionRange();

#endif