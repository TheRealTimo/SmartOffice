/*
Description: EEPROM allows for saving data to permanent storage and is used to hold user defined variables and perist them even through restarts
*/

#include "eeprom.h"

const uint8_t SETUP_FLAG_VALUES[] = { 54, 48 }; //54 48 
const int SETUP_FLAG_SIZE = sizeof(SETUP_FLAG_VALUES);

//Stores the memory addresses for each variable in EEPROM
namespace EEPROMAddresses {
const uint8_t MOTION_THRESHOLD = SETUP_FLAG_SIZE;
const uint8_t OPT_OUT_TIMEOUT = MOTION_THRESHOLD + sizeof(float);
const uint8_t INACTIVITY_TIMEOUT = OPT_OUT_TIMEOUT + sizeof(uint16_t);
const uint8_t SAMPLE_SPEED = INACTIVITY_TIMEOUT + sizeof(uint8_t);
const uint8_t MINIMUM_MOTION_COUNT = SAMPLE_SPEED + sizeof(uint8_t);
const uint8_t MINIMUM_MOTION_COUNT_TIMEFRAME = MINIMUM_MOTION_COUNT + sizeof(uint8_t);
}

/*
 *  Function: writeSetupFlag
 *  Description: Writes the values of the setup flag to EEPROM to indicate that EEPROM has been set up previously. Avoids writing to EEPROM at every startup (and overwriting the user defined values)
 *  Parameters: None
 *  Returns: None
 */
void writeSetupFlag() {
  for (int i = 0; i < SETUP_FLAG_SIZE; i++) {
    EEPROM.write(i, SETUP_FLAG_VALUES[i]);
  }
  EEPROM.commit();
}

/*
 *  Function: checkSetupFlag
 *  Description: Reads the values at the memory address of the setup flags. If no setup flags have been found (indicating that EEPROM has not been set up), the default values will be written 
 *  Parameters: None
 *  Returns: None
 */
void checkSetupFlag() {
  for (int i = 0; i < SETUP_FLAG_SIZE; i++) {
    if (EEPROM.read(i) != SETUP_FLAG_VALUES[i]) {
      writeDefaultValuesToEeprom();
    }
  }
}

/*
 *  Function: writeDefaultValuesToEeprom
 *  Description: Tries to write all default values to EEPROM if they are valid data types. If all writes succeed, it will call writeSetupFlag
 *  Parameters: None
 *  Returns: None
 */
void writeDefaultValuesToEeprom() {
  if (verifyDefaultValuesForEeprom()) {
    if (updateMotionThresholdEeprom(MOTION_THRESHOLD_DEFAULT_VALUE) && updateOptOutButtonTimeoutEeprom(OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) && updateInactivityTimeoutEeprom(INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) && updateSampleSpeedEeprom(SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE) && updateMinimumMotionCountRequiredEeprom(MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE) && updateMinimumMotionCountTimeframeEeprom(MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE)) {
      writeSetupFlag();
    }
  }
}

/*
 *  Function: verifyDefaultValuesForEeprom
 *  Description: Checks all default values that should be saved to EEPROM to ensure they conform to the data type to not write data beyond the intened memory addresses
 *  Parameters: None
 *  Returns: bool - true if all values defined are acceptable data types for saving to EEPROM
 */
bool verifyDefaultValuesForEeprom() {
  return (isStringValidFloat(String(MOTION_THRESHOLD_DEFAULT_VALUE)) && isStringValidUint16(String(OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE)) && isStringValidUint8(String(INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE)) && isStringValidUint8(String(SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE)) && isStringValidUint8(String(MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE)) && isStringValidUint8(String(MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE)));
}

/*
 *  Function: readEeprom
 *  Description: Reads the defined values from EEPROM at the EEPROMAddresses and saves the values right into the global variables from "variables.h"
 *  Parameters: None
 *  Returns: None
 */
void readEeprom() {
  EEPROM.get(EEPROMAddresses::MOTION_THRESHOLD, motionThreshold);
  EEPROM.get(EEPROMAddresses::OPT_OUT_TIMEOUT, optOutButtonTimeoutInMinutes);
  EEPROM.get(EEPROMAddresses::INACTIVITY_TIMEOUT, inactivityTimeoutInMinutes);
  EEPROM.get(EEPROMAddresses::SAMPLE_SPEED, sampleSpeedInMilliseconds);
  EEPROM.get(EEPROMAddresses::MINIMUM_MOTION_COUNT, minimumMotionDetectionCountRequired);
  EEPROM.get(EEPROMAddresses::MINIMUM_MOTION_COUNT_TIMEFRAME, minimumMotionDetectionCountTimeframe);
}

/*
 *  Function: updateMotionThresholdEeprom
 *  Description: Writes a new float value to EEPROM at the memory address of MOTION_THRESHOLD
 *  Parameters: float - new motionThreshold value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateMotionThresholdEeprom(const float& newValue) {
  EEPROM.put(EEPROMAddresses::MOTION_THRESHOLD, newValue);
  return EEPROM.commit() ? true : false;
}

/*
 *  Function: updateOptOutButtonTimeoutEeprom
 *  Description: Writes a new uint16_t value to EEPROM at the memory address of OPT_OUT_TIMEOUT
 *  Parameters: uint16_t - new optOutButtonTimeoutInMinutes value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateOptOutButtonTimeoutEeprom(const uint16_t& newValue) {
  EEPROM.put(EEPROMAddresses::OPT_OUT_TIMEOUT, newValue);
  return EEPROM.commit() ? true : false;
}

/*
 *  Function: updateInactivityTimeoutEeprom
 *  Description: Writes a new uint8_t value to EEPROM at the memory address of INACTIVITY_TIMEOUT
 *  Parameters: uint8_t - new inactivityTimeoutInMinutes value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateInactivityTimeoutEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::INACTIVITY_TIMEOUT, newValue);
  return EEPROM.commit() ? true : false;
}

/*
 *  Function: updateSampleSpeedEeprom
 *  Description: Writes a new uint8_t value to EEPROM at the memory address of SAMPLE_SPEED
 *  Parameters: uint8_t - new sampleSpeedInMilliseconds value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateSampleSpeedEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::SAMPLE_SPEED, newValue);
  return EEPROM.commit() ? true : false;
}

/*
 *  Function: updateMinimumMotionCountRequiredEeprom
 *  Description: Writes a new uint8_t value to EEPROM at the memory address of MINIMUM_MOTION_COUNT
 *  Parameters: uint8_t - new minimumMotionDetectionCountRequired value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateMinimumMotionCountRequiredEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::MINIMUM_MOTION_COUNT, newValue);
  return EEPROM.commit() ? true : false;
}

/*
 *  Function: updateMinimumMotionCountTimeframeEeprom
 *  Description: Writes a new uint8_t value to EEPROM at the memory address of MINIMUM_MOTION_COUNT_TIMEFRAME
 *  Parameters: uint8_t - new minimumMotionDetectionCountTimeframe value
 *  Returns: bool - true if writing to EEPROM was successful otherwise false
 */
bool updateMinimumMotionCountTimeframeEeprom(const uint8_t& newValue) {
  EEPROM.put(EEPROMAddresses::MINIMUM_MOTION_COUNT_TIMEFRAME, newValue);
  return EEPROM.commit() ? true : false;
}