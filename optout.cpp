#include "optout.h"

const uint8_t optOutButtonPin = 5;

void startOptOutTimer() {
  //Debounce the button first to prevent the loop from exiting right again
  while (digitalRead(optOutButtonPin) == HIGH) {
    delay(200);
  }

  //Set all values to prepare the system for the optOut
  updateLedStatus(SYSTEM_PAUSED);
  turnSmartSwitchOn(true);
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  //The system will return to a operational presence state after the timeout to give it the opportunity to measure presence to avoid a shut off after the optOutTimer runs out even when someone is present
  while (elapsedTime < ((OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE - INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) * 60 * 1000) && isOptOutTimerRunning) {
    ESP.wdtFeed();


    if (digitalRead(optOutButtonPin) == HIGH) {
      while (digitalRead(optOutButtonPin) == HIGH) {
        delay(200);
      }
      isOptOutTimerRunning = false;
      break;
    }

    delay(100);
    elapsedTime = millis() - startTime;
  }
  isOptOutTimerRunning = false;
  isDeskOccupied = false;
  updateLedStatus(OPERATIONAL_PRESENCE);
  lastMotionDetectionTime = millis();
  return;
}