#ifndef VARIABLES_H
#define VARIABLES_H

#include "config.h"
#include <stdint.h>

extern float motionThreshold;
extern uint16_t optOutButtonTimeoutInMinutes;
extern uint8_t inactivityTimeoutInMinutes;
extern uint8_t sampleSpeedInMilliseconds;
extern uint8_t minimumMotionDetectionCountRequired;
extern uint8_t minimumMotionDetectionCountTimeframe;
volatile extern bool isOptOutTimerRunning;
extern bool isDeskOccupied;
extern unsigned long lastMotionDetectionTime;


#endif
