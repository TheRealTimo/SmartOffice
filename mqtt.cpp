#include "mqtt.h"

unsigned long publishTimer; 
uint8_t publishTimeout = 250;

PubSubClient mqttClient(espWifiClient);
bool isMqttSetup = false;

void setupMqtt() {
  Serial.println("Setting up MQTT");
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(callback);
  isMqttSetup = true;
}

void connectMqtt() {
  updateLedStatus(SETUP);
  while (!mqttClient.connected()) {
    isMqttSetup ? (void)0 : setupMqtt();
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    String willTopic = String(MQTT_SWITCH_CMND_TOPIC_PREFIX) + String(MQTT_SWITCH_TOPIC_SUFFIX) + "/POWER";
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD, willTopic.c_str(), 1, false, "1")) {
      Serial.println("Connected as " + String(clientId));
      bool status = mqttClient.subscribe((String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID) + "/#").c_str());
      Serial.println(status ? "Subscribed to command topic" : "Could not subscribe to MQTT topic");
    } else {
      updateLedStatus(ERROR);
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishOccupancyStatusToMqtt() {
  mqttClient.publish((String(MQTT_ESP_STATUS_TOPIC) + String(MQTT_CONTROLLER_ID) + "/PRESENCE").c_str(), isDeskOccupied ? "OCCUPIED" : "UNOCCUPIED");
}

void publishTelemetryToMqtt() {
  String telemetryJson = "{";
  telemetryJson += "\"initialisationTimeInSeconds\":";
  telemetryJson += IMU_CALIBRATION_TIME_IN_SECONDS;
  telemetryJson += ",\"sensorConnectionAttempts\":";
  telemetryJson += IMU_CONNECTION_ATTEMPTS;
  telemetryJson += ",\"networkSsid\":\"";
  telemetryJson += WIFI_SSID;
  telemetryJson += "\"";
  telemetryJson += ",\"wifiConnectionTimeoutInSeconds\":";
  telemetryJson += WIFI_TIMEOUT_IN_SECONDS;
  telemetryJson += ",\"mqttBrokerIp\":\"";
  telemetryJson += MQTT_HOST;
  telemetryJson += "\"";
  telemetryJson += ",\"mqttBrokerPort\":";
  telemetryJson += MQTT_PORT;
  telemetryJson += ",\"mqttUsername\":\"";
  telemetryJson += MQTT_USERNAME;
  telemetryJson += "\"";
  telemetryJson += "}";

  mqttClient.publish((String(MQTT_ESP_TELE_TOPIC) + String(MQTT_CONTROLLER_ID) + "/INFO1").c_str(), telemetryJson.c_str(), true);
}

void publishOptOutValueToMqtt() {
  mqttClient.publish((String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID) + "/OPTOUT").c_str(), isOptOutTimerRunning ? "TRUE" : "FALSE");
}

void turnSmartSwitchOn(const bool& state) {
  mqttPublishWithRetry((String(MQTT_SWITCH_CMND_TOPIC_PREFIX) + String(MQTT_SWITCH_TOPIC_SUFFIX) + "/POWER").c_str(), state ? "1" : "0");
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (millis() >= publishTimer + publishTimeout) {
    Serial.println(ESP.getFreeHeap());
    String expectedTopicPrefix = String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID);
    String topicStr = String(topic);

    String payloadStr = "";
    for (int i = 0; i < length; i++) {
      payloadStr += (char)payload[i];
    }

    if (String(topic).startsWith(expectedTopicPrefix + "/") && payloadStr.length() != 0) {
      int lastSlashIndex = topicStr.lastIndexOf('/');
      String subtopic = topicStr.substring(lastSlashIndex + 1);

      if (subtopic == "MOTION_THRESHOLD") {
        motionThreshold = payloadStr.toFloat();
      } else if (subtopic == "OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES") {
        optOutButtonTimeoutInMinutes = (uint16_t)payloadStr.toInt();
      } else if (subtopic == "INACTIVITY_TIMEOUT_IN_MINUTES") {
        inactivityTimeoutInMinutes = (uint8_t)payloadStr.toInt();
      } else if (subtopic == "SAMPLE_SPEED_IN_MILLISECONDS") {
        sampleSpeedInMilliseconds = (uint8_t)payloadStr.toInt();
      }
    }
    publishCommandValuesToMqtt();
  }
}


void mqttPublishWithRetry(const char* topic, const char* payload, const bool& retain) {
  for (int i = 0; i < MQTT_PUBLISH_ATTEMPTS; i++) {
    if (mqttClient.publish(topic, payload, retain)) {
      return;
    }
    delay(MQTT_PUBLISH_RETRY_DELAY_IN_MILLISECONDS);
  }
  ledBlinkError();
}

void publishCommandValuesToMqtt() {
  publishTimer = millis();
  String baseTopic = String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID);

  mqttClient.publish((baseTopic + "/MOTION_THRESHOLD").c_str(), String(motionThreshold, 5).c_str(), true);
  mqttClient.publish((baseTopic + "/OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES").c_str(), String(optOutButtonTimeoutInMinutes).c_str(), true);
  mqttClient.publish((baseTopic + "/INACTIVITY_TIMEOUT_IN_MINUTES").c_str(), String(inactivityTimeoutInMinutes).c_str(), true);
  mqttClient.publish((baseTopic + "/SAMPLE_SPEED_IN_MILLISECONDS").c_str(), String(sampleSpeedInMilliseconds).c_str(), true);
  mqttClient.publish((baseTopic + "/OPTOUT").c_str(), isOptOutTimerRunning ? "TRUE" : "FALSE");
}