#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

bool isStringValidBoolean(const String& inputValue);
bool isStringValidFloat(const String& inputValue);
bool isStringValidUint8(const String& inputValue);
bool isStringValidUint16(const String& inputValue);
bool convertStringToBool(const String& str);

#endif