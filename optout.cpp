/*
Description: The opt out logic allows for keeping the smart power plug turned on even with no motion detection. It also stops measurements from the IMU
*/
#include "optout.h"

const uint8_t optOutButtonPin = 5;

/*
 *  Function: startOptOutTimer
 *  Description: Interrupts the flow of the main loop to pause sensor measurements. Delays the program as long as set by the user
 *  Parameters: None
 *  Returns: None
 */
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

  //If the optOutTimeout has not been met or the timer is still running, run the loop.
  //The system will return to a operational presence state after the timeout to give it the opportunity to measure presence to avoid a shut off after the optOutTimer runs out even when someone is present
  while (elapsedTime < ((OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE - INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE) * 60 * 1000) && isOptOutTimerRunning) {
    ESP.wdtFeed();

    ESP.wdtFeed(); //Feed the ESP Watchdog to prevent a reset due to being stuck in a loop

    //If the button has been pressed again, exit the opt out
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

  // Return the system to a normal operational state
  isOptOutTimerRunning = false;
  isDeskOccupied = false;
  updateLedStatus(OPERATIONAL_PRESENCE);
  lastMotionDetectionTime = millis();
  return;
}