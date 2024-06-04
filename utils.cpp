#include "utils.h"

bool isStringValidBoolean(const String& inputValue) {
  if (inputValue.equalsIgnoreCase("true") || inputValue.equalsIgnoreCase("false") || inputValue.equalsIgnoreCase("on") || inputValue.equalsIgnoreCase("off") || inputValue == "1" || inputValue == "0") {
    return true;
  } else {
    return false;
  }
}

bool isStringValidFloat(const String& inputValue) {
  float value;
  int numConversions = sscanf(inputValue.c_str(), "%f", &value);

  if (numConversions != 1) {  // Ensure exactly one float was converted
    return false;
  }

  if (isnan(value) || isinf(value)) {  // Check for NaN or Inf values
    return false;
  }
  return true;
}

bool isStringValidUint8(const String& inputValue) {
  // Check if the string contains only digits
  for (size_t i = 0; i < inputValue.length(); ++i) {
    if (!isDigit(inputValue[i])) {
      return false;  // Non-digit character found
    }
  }

  // Convert the string to an unsigned long (to avoid int overflow)
  long value = inputValue.toInt();

  // Check if the value is within the uint8_t range
  return (value >= 0 && value <= UINT8_MAX);
}


bool isStringValidUint16(const String& inputValue) {
  // Check if the string contains only digits
  for (size_t i = 0; i < inputValue.length(); ++i) {
    if (!isDigit(inputValue[i])) {
      return false;  // Non-digit character found
    }
  }

  // Convert the string to an unsigned long (to avoid int overflow)
  long value = inputValue.toInt();

  // Check if the value is within the uint16_t range
  return (value >= 0 && value <= UINT16_MAX);
}


bool convertStringToBool(const String& str) {
  return (str.equalsIgnoreCase("true") || str.equalsIgnoreCase("on") || str == "1");
}