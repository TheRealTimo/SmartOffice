/*
Description: Functions to allow for changing configurations in EEPROM and local variables. Validates and updates
*/

#include "configurator.h"

/*
 *  Function: updateMotionThreshold
 *  Description: Checks if the string can be converted to float, is within bounds and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for motionThreshold
 *  Returns: None
 */
void updateMotionThreshold(const String& payloadStr) {
  if (isStringValidFloat(payloadStr)) {
    float newValue = payloadStr.toFloat();
    if (newValue > 0.0 && newValue < 1.0 && motionThreshold != newValue) {
      if (updateMotionThresholdEeprom(newValue)) {
        motionThreshold = newValue;
      }
    }
  }
}

/*
 *  Function: updateButtonTimeout
 *  Description: Checks if the string can be converted to uint16_t and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for optOutButtonTimeoutInMinutes
 *  Returns: None
 */
void updateButtonTimeout(const String& payloadStr) {
  if (isStringValidUint16(payloadStr)) {
    uint16_t newValue = (uint16_t)payloadStr.toInt();
    if (optOutButtonTimeoutInMinutes != newValue) {
      if (updateOptOutButtonTimeoutEeprom(newValue)) {
        optOutButtonTimeoutInMinutes = newValue;
      }
    }
  }
}

/*
 *  Function: updateSampleSpeed
 *  Description: Checks if the string can be converted to uint8_t and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for inactivityTimeoutInMinutes
 *  Returns: None
 */
void updateInactivityTimeout(const String& payloadStr) {
  if (isStringValidUint8(payloadStr)) {
    uint8_t newValue = (uint8_t)payloadStr.toInt();
    if (inactivityTimeoutInMinutes != newValue) {
      if (updateInactivityTimeoutEeprom(newValue)) {
        inactivityTimeoutInMinutes = newValue;
      }
    }
  }
}

/*
 *  Function: updateSampleSpeed
 *  Description: Checks if the string can be converted to uint8_t and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for sampleSpeedInMilliseconds
 *  Returns: None
 */
void updateSampleSpeed(const String& payloadStr) {
  if (isStringValidUint8(payloadStr)) {
    uint8_t newValue = (uint8_t)payloadStr.toInt();
    if (sampleSpeedInMilliseconds != newValue) {
      if (updateSampleSpeedEeprom(newValue)) {
        sampleSpeedInMilliseconds = newValue;
      }
    }
  }
}

/*
 *  Function: updateOptOut
 *  Description: Checks if the string can be converted to boolean and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for isOptOutTimerRunning
 *  Returns: None
 */
void updateOptOut(const String& payloadStr) {
  if (isStringValidBoolean(payloadStr)) {
    bool newValue = convertStringToBool(payloadStr);
    isOptOutTimerRunning = newValue;
  }
}

/*
 *  Function: updateMinimumMotionCountRequired
 *  Description: Checks if the string can be converted to uint8_t, is within bounds and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for minimumMotionDetectionCountRequired
 *  Returns: None
 */
void updateMinimumMotionCountRequired(const String& payloadStr) {
  if (isStringValidUint8(payloadStr)) {
    uint8_t newValue = (uint8_t)payloadStr.toInt();
    if (minimumMotionDetectionCountRequired != newValue && minimumMotionDetectionCountRequired < minimumMotionDetectionCountTimeframe) {
      if (updateMinimumMotionCountRequiredEeprom(newValue)) {
        minimumMotionDetectionCountRequired = newValue;
      }
    }
  }
}

/*
 *  Function: updateMinimumMotionCountTimeframe
 *  Description: Checks if the string can be converted to uint8_t and not equal to the old value (to avoid writing to EEPROM unnecesarily). Updates EEPROM and local variable
 *  Parameters: String - String with the new value to be set for minimumMotionDetectionCountTimeframe
 *  Returns: None
 */
void updateMinimumMotionCountTimeframe(const String& payloadStr) {
  if (isStringValidUint8(payloadStr)) {
    uint8_t newValue = (uint8_t)payloadStr.toInt();
    if (minimumMotionDetectionCountTimeframe != newValue) {
      if (updateMinimumMotionCountTimeframeEeprom(newValue)) {
        minimumMotionDetectionCountTimeframe = newValue;
      }
    }
  }
}