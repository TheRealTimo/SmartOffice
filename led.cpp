#include "led.h"

const uint8_t redLedPin = 4;
const uint8_t greenLedPin = 0;
const uint8_t blueLedPin = 2;

void updateLedStatus(const LedStatus& status) {
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(blueLedPin, LOW);

  switch (status) {
    case ERROR:  //Red
      digitalWrite(redLedPin, HIGH);
      break;
    case SETUP:  //White
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(blueLedPin, HIGH);
      break;
    case CALIBRATING:  //Yellow
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, HIGH);
      break;
    case OPERATIONAL_PRESENCE:  //Green
      digitalWrite(greenLedPin, HIGH);
      break;
    case OPERATIONAL_NO_PRESENCE:  //Blue
      digitalWrite(blueLedPin, HIGH);
      break;
    case OFF:
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      digitalWrite(blueLedPin, LOW);
      break;
  }
}

void ledBlinkError() {
  while (1) {
    updateLedStatus(ERROR);
    delay(250);
    updateLedStatus(OFF);
    delay(500);
    ESP.wdtFeed();
  }
}