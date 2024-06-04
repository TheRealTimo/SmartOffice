#include "variables.h"

/*
IMU related settings below
________________________________________________
*/
float motionThreshold = MOTION_THRESHOLD_DEFAULT_VALUE;
uint16_t optOutButtonTimeoutInMinutes = OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE;
uint8_t inactivityTimeoutInMinutes = INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE;
uint8_t sampleSpeedInMilliseconds = SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE;
uint8_t minimumMotionDetectionCountRequired = MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE;
uint8_t minimumMotionDetectionCountTimeframe = MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE;

/*
General variables
________________________________________________
*/
volatile bool isOptOutTimerRunning = false;  //Volatile prevents optimization of variable when compiling and thus making it fatser accessible
bool isDeskOccupied = false;
unsigned long lastMotionDetectionTime = 0;