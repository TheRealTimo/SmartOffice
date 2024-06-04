#ifndef LED_H
#define LED_H

#include <Arduino.h>

enum LedStatus {
  ERROR,
  SETUP,
  CALIBRATING,
  OPERATIONAL_PRESENCE,
  OPERATIONAL_NO_PRESENCE,
  OFF
};

extern const uint8_t redLedPin;
extern const uint8_t greenLedPin;
extern const uint8_t blueLedPin;

void updateLedStatus(const LedStatus& status);
void ledBlinkError();

#endif