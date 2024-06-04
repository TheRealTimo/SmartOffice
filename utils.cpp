/*
Description: A collection of functiions to validate Strings
*/

#include "utils.h"

/*
 *  Function: isStringValidBoolean
 *  Description: Checks of a string holds a value that can be converted to a boolean
 *  Parameters: String - String to be checked for being a valid boolean
 *  Returns: bool - true if the provided string can be converted to a boolean
 */
bool isStringValidBoolean(const String& inputValue) {
  if (inputValue.equalsIgnoreCase("true") || inputValue.equalsIgnoreCase("false") || inputValue.equalsIgnoreCase("on") || inputValue.equalsIgnoreCase("off") || inputValue == "1" || inputValue == "0") {
    return true;
  } else {
    return false;
  }
}

/*
 *  Function: isStringValidFloat
 *  Description: Checks of a string holds a value that can be converted to a float
 *  Parameters: String - String to be checked for being a valid Float
 *  Returns: bool - true if the provided string can be converted to a Float
 */
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

/*
 *  Function: isStringValidUint8
 *  Description: Checks of a string holds a value that can be converted to a uint8_t
 *  Parameters: String - String to be checked for being a valid uint8_t
 *  Returns: bool - true if the provided string can be converted to a uint8_t
 */
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

/*
 *  Function: isStringValidUint16
 *  Description: Checks of a string holds a value that can be converted to a uint16_t
 *  Parameters: String - String to be checked for being a valid uint16_t
 *  Returns: bool - true if the provided string can be converted to a uint16_t
 */
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

/*
 *  Function: convertStringToBool
 *  Description: Converts a String with given values to a boolean
 *  Parameters: String - String to be converted to a boolean
 *  Returns: bool - true if the provided string represents a true boolean
 */
bool convertStringToBool(const String& str) {
  return (str.equalsIgnoreCase("true") || str.equalsIgnoreCase("on") || str == "1");
}