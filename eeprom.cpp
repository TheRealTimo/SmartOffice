#include "eeprom.h"

const uint8_t SETUP_FLAG_VALUES[] = { 54, 48 }; //54 48 
const int SETUP_FLAG_SIZE = sizeof(SETUP_FLAG_VALUES);

namespace EEPROMAddresses {
const uint8_t MOTION_THRESHOLD = SETUP_FLAG_SIZE;
const uint8_t OPT_OUT_TIMEOUT = MOTION_THRESHOLD + sizeof(float);
const uint8_t INACTIVITY_TIMEOUT = OPT_OUT_TIMEOUT + sizeof(uint16_t);
const uint8_t SAMPLE_SPEED = INACTIVITY_TIMEOUT + sizeof(uint8_t);
const uint8_t MINIMUM_MOTION_COUNT = SAMPLE_SPEED + sizeof(uint8_t);
const uint8_t MINIMUM_MOTION_COUNT_TIMEFRAME = MINIMUM_MOTION_COUNT + sizeof(uint8_t);
}

void writeSetupFlag() {
  for (int i = 0; i < SETUP_FLAG_SIZE; i++) {
    EEPROM.write(i, SETUP_FLAG_VALUES[i]);
  }
  EEPROM.commit();
}


void checkSetupFlag() {
  for (int i = 0; i < SETUP_FLAG_SIZE; i++) {
    if (EEPROM.read(i) != SETUP_FLAG_VALUES[i]) {
      writeDefaultValuesToEeprom();
    }
  }
}

void writeDefaultValuesToEeprom() {
  if (verifyDefaultValuesForEeprom()) {
    if (updateMotionThresholdEeprom(MOTION_THRESHOLD_DEFAULT_VALUE) && updateOptOutButtonTimeoutEeprom(OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) && updateInactivityTimeoutEeprom(INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) && updateSampleSpeedEeprom(SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE) && updateMinimumMotionCountRequiredEeprom(MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE) && updateMinimumMotionCountTimeframeEeprom(MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE)) {
      writeSetupFlag();
    }
  }
}

bool verifyDefaultValuesForEeprom() {
  return (isStringValidFloat(String(MOTION_THRESHOLD_DEFAULT_VALUE)) && isStringValidUint16(String(OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE)) && isStringValidUint8(String(INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE)) && isStringValidUint8(String(SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE)) && isStringValidUint8(String(MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE)) && isStringValidUint8(String(MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE)));
}


void readEeprom() {
  EEPROM.get(EEPROMAddresses::MOTION_THRESHOLD, motionThreshold);
  EEPROM.get(EEPROMAddresses::OPT_OUT_TIMEOUT, optOutButtonTimeoutInMinutes);
  EEPROM.get(EEPROMAddresses::INACTIVITY_TIMEOUT, inactivityTimeoutInMinutes);
  EEPROM.get(EEPROMAddresses::SAMPLE_SPEED, sampleSpeedInMilliseconds);
  EEPROM.get(EEPROMAddresses::MINIMUM_MOTION_COUNT, minimumMotionDetectionCountRequired);
  EEPROM.get(EEPROMAddresses::MINIMUM_MOTION_COUNT_TIMEFRAME, minimumMotionDetectionCountTimeframe);
}


bool updateMotionThresholdEeprom(const float& newValue) {
  EEPROM.put(EEPROMAddresses::MOTION_THRESHOLD, newValue);
  return EEPROM.commit() ? true : false;
}


bool updateOptOutButtonTimeoutEeprom(const uint16_t& newValue) {
  EEPROM.put(EEPROMAddresses::OPT_OUT_TIMEOUT, newValue);
  return EEPROM.commit() ? true : false;
}

bool updateInactivityTimeoutEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::INACTIVITY_TIMEOUT, newValue);
  return EEPROM.commit() ? true : false;
}


bool updateSampleSpeedEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::SAMPLE_SPEED, newValue);
  return EEPROM.commit() ? true : false;
}

bool updateMinimumMotionCountRequiredEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::MINIMUM_MOTION_COUNT, newValue);
  return EEPROM.commit() ? true : false;
}

bool updateMinimumMotionCountTimeframeEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::MINIMUM_MOTION_COUNT_TIMEFRAME, newValue);
  return EEPROM.commit() ? true : false;
}