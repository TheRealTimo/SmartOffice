#include "variables.h"

/*
General variables
________________________________________________
*/
volatile bool isOptOutTimerRunning = false;
bool isDeskOccupied = false;
unsigned long lastMotionDetectionTime = 0;