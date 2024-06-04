#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include "utils.h"
#include <Arduino.h>
#include "eeprom.h"
#include "variables.h"

void updateMotionThreshold(const String& payloadStr);
void updateButtonTimeout(const String& payloadStr);
void updateInactivityTimeout(const String& payloadStr);
void updateSampleSpeed(const String& payloadStr);
void updateOptOut(const String& payloadStr);
void updateMinimumMotionCountRequired(const String& payloadStr);
void updateMinimumMotionCountTimeframe(const String& payloadStr);

#endif