#include "configurator.h"

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

void updateOptOut(const String& payloadStr) {
  if (isStringValidBoolean(payloadStr)) {
    bool newValue = convertStringToBool(payloadStr);
    isOptOutTimerRunning = newValue;
  }
}

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