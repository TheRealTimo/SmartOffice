#ifndef EEPROM_H
#define EEPROM_H

#include <EEPROM.h>
#include "config.h"
#include "utils.h"
#include <stdint.h>
#include "variables.h"

void writeSetupFlag();
void checkSetupFlag();
void writeDefaultValuesToEeprom();
bool verifyDefaultValuesForEeprom();
void readEeprom();
bool updateMotionThresholdEeprom(const float& newValue);
bool updateOptOutButtonTimeoutEeprom(const uint16_t& newValue);
bool updateInactivityTimeoutEeprom(const uint8_t& newValue);
bool updateSampleSpeedEeprom(const uint8_t& newValue);
bool updateMinimumMotionCountRequiredEeprom(const uint8_t& newValue);
bool updateMinimumMotionCountTimeframeEeprom(const uint8_t& newValue);

#endif