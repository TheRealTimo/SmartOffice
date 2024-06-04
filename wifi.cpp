#include "wifi.h"

WiFiClient espWifiClient;

void connectToWifi() {
  updateLedStatus(SETUP);
  Serial.println("Connecting to WI-Fi");
  WiFi.mode(WIFI_STA);
  WiFi.begin("SmartOfficePrototype", "Password24!");

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30 * 1000) {
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