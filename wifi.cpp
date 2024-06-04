/*
Description: Handles WIFI connection 
*/

#include "wifi.h"

WiFiClient espWifiClient;

/*
 *  Function: connectToWifi
 *  Description: Connects the ESP to WIFI based on the configs in "config.h". Attempts multiple times on failed connection within a given timeframe
 *  Parameters: None
 *  Returns: None
 */
void connectToWifi() {
  updateLedStatus(SETUP);
  Serial.println("Connecting to WI-Fi: " + String(WIFI_SSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(String(WIFI_SSID), String(WIFI_PSK));

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_IN_SECONDS * 1000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {  // Successful connection
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {  // Connection timed out
    Serial.println("Failed to connect to WiFi");
    wifi_set_sleep_type(MODEM_SLEEP_T); //Disable WIFI to conserve power
    ledBlinkError();
  }
}