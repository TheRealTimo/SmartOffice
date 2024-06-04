#include "mqtt.h"

PubSubClient mqttClient(espWifiClient);
bool isMqttSetup = false;

void setupMqtt() {
  Serial.println("Setting up MQTT");
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  isMqttSetup = true;
}

void connectMqtt() {
  unsigned long startTime = millis();
  updateLedStatus(SETUP);
  while (!mqttClient.connected()) {
    isMqttSetup ? (void)0 : setupMqtt();
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Connected as " + String(clientId));
    } else {
      updateLedStatus(ERROR);
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  if (!mqttClient.connected()) {
    ledBlinkError();
  }
}