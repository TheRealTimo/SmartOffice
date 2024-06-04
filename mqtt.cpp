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
  while (!mqttClient.connected() && millis() - startTime < MQTT_CONNECTION_TIMEOUT_IN_SECONDS * 1000) {
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
  if (!mqttClient.connected()) {
    ledBlinkError();
  }
}

void publishOccupancyStatusToMqtt() {
  mqttClient.publish((String(MQTT_ESP_STATUS_TOPIC) + String(MQTT_CONTROLLER_ID) + "/PRESENCE").c_str(), isDeskOccupied ? "OCCUPIED" : "UNOCCUPIED");
}

void turnSmartSwitchOn(const bool& state) {
  mqttPublishWithRetry((String(MQTT_SWITCH_CMND_TOPIC_PREFIX) + String(MQTT_SWITCH_TOPIC_SUFFIX) + "/POWER").c_str(), state ? "1" : "0");
}

void mqttPublishWithRetry(const char* topic, const char* payload,const bool& retain){  
  for(int i = 0; i < MQTT_PUBLISH_ATTEMPTS; i++){
    if(mqttClient.publish(topic, payload, retain)){
      return;
    }
    delay(MQTT_PUBLISH_RETRY_DELAY_IN_MILLISECONDS);
  }
  ledBlinkError();
}