/*
Description: Controls the status of the RGB LED
*/

#include "led.h"

const uint8_t redLedPin = 4;
const uint8_t greenLedPin = 0;
const uint8_t blueLedPin = 2;

/*
 *  Function: updateLedStatus
 *  Description: Switches the LED color to represent a certain state defined in LedStatus ENUM
 *  Parameters: LedStatus - ENUM of the status the LED is supposed to represent
 *  Returns: None
 */
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
    case SYSTEM_PAUSED:  //Cyan
      digitalWrite(blueLedPin, HIGH);
      digitalWrite(greenLedPin, HIGH);
      break;
    case OFF:
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      digitalWrite(blueLedPin, LOW);
      break;
  }
}

/*
 *  Function: ledBlinkError
 *  Description: Blinks the RGB LED on ERROR(red) indefinetly to indicate system failure
 *  Parameters: None
 *  Returns: None
 */
void ledBlinkError() {
  while (1) {
    updateLedStatus(ERROR);
    delay(250);
    updateLedStatus(OFF);
    delay(500);
    ESP.wdtFeed();
  }
}