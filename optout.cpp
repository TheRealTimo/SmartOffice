#include "optout.h"

const uint8_t optOutButtonPin = 5;

void startOptOutTimer() {
  //Debounce the button first to prevent the loop from exiting right again
  while (digitalRead(optOutButtonPin) == HIGH) {
    delay(200);
  }

  //Set all values to prepare the system for the optOut
  updateLedStatus(SYSTEM_PAUSED);
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < ((optOutButtonTimeoutInMinutes) * 60 * 1000)) {
    ESP.wdtFeed();


    if (digitalRead(optOutButtonPin) == HIGH) {
      return;
    }

    delay(100);
    elapsedTime = millis() - startTime;
  }

  updateLedStatus(OPERATIONAL_PRESENCE);
  return;
}