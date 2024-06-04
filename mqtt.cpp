/*
Description: Connects to MQTT and handles publishing as well a ssubscribing to topics
*/

#include "mqtt.h"

PubSubClient mqttClient(espWifiClient);
bool isMqttSetup = false;

/*
 *  Function: setupMqtt
 *  Description: Provides the MQTT client with basic connection information for the broker
 *  Parameters: None
 *  Returns: None
 */
void setupMqtt() {
  Serial.println("Setting up MQTT");
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(subscriptionCallback);
  isMqttSetup = true;
}

/*
 *  Function: connectMqtt
 *  Description: Connects to the MQTT broker, sets up a last will (vaue to be set for switch in case of client disconnection) and subscribes to the command topic. Attempts connection multiple times in case of failure
 *  Parameters: None
 *  Returns: None
 */
void connectMqtt() {
  unsigned long startTime = millis();
  updateLedStatus(SETUP);
  while (!mqttClient.connected() && millis() - startTime < MQTT_CONNECTION_TIMEOUT_IN_SECONDS * 1000) {
    isMqttSetup ? (void)0 : setupMqtt();
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    String willTopic = String(MQTT_SWITCH_CMND_TOPIC_PREFIX) + String(MQTT_SWITCH_TOPIC_SUFFIX) + "/POWER";
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD, willTopic.c_str(), 1, false, "1")) {
      Serial.println("Connected as " + String(clientId));
      bool status = mqttClient.subscribe((String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID) + "/+/set").c_str());
      Serial.println(status ? "Subscribed to command topic" : "Could not subscribe to MQTT topic");
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

/*
 *  Function: publishOccupancyStatusToMqtt
 *  Description: Publishes the current occupancy status to MQTT
 *  Parameters: None
 *  Returns: None
 */
void publishOccupancyStatusToMqtt() {
  mqttClient.publish((String(MQTT_ESP_STATUS_TOPIC) + String(MQTT_CONTROLLER_ID) + "/PRESENCE").c_str(), isDeskOccupied ? "OCCUPIED" : "UNOCCUPIED");
}

/*
 *  Function: publishTelemetryToMqtt
 *  Description: Publishes the telemetry data and variables to MQTT
 *  Parameters: None
 *  Returns: None
 */
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

/*
 *  Function: publishCommandValuesToMqtt
 *  Description: Publishes the values the user can set manually to the command topic
 *  Parameters: None
 *  Returns: None
 */
void publishCommandValuesToMqtt() {
  String baseTopic = String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID);

  mqttClient.publish((baseTopic + "/MOTION_THRESHOLD").c_str(), String(motionThreshold, 5).c_str(), true);
  mqttClient.publish((baseTopic + "/OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES").c_str(), String(optOutButtonTimeoutInMinutes).c_str(), true);
  mqttClient.publish((baseTopic + "/INACTIVITY_TIMEOUT_IN_MINUTES").c_str(), String(inactivityTimeoutInMinutes).c_str(), true);
  mqttClient.publish((baseTopic + "/SAMPLE_SPEED_IN_MILLISECONDS").c_str(), String(sampleSpeedInMilliseconds).c_str(), true);
  mqttClient.publish((baseTopic + "/OPTOUT").c_str(), isOptOutTimerRunning ? "TRUE" : "FALSE");
  mqttClient.publish((baseTopic + "/MIMIMUM_DETECTION_COUNT_REQUIRED").c_str(), String(minimumMotionDetectionCountRequired).c_str(), true);
  mqttClient.publish((baseTopic + "/MIMIMUM_DETECTION_COUNT_TIMEFRAME").c_str(), String(minimumMotionDetectionCountTimeframe).c_str(), true);
}

/*
 *  Function: publishOptOutValueToMqtt
 *  Description: Publishes the current optOutTimer status to MQTT
 *  Parameters: None
 *  Returns: None
 */
void publishOptOutValueToMqtt() {
  mqttClient.publish((String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID) + "/OPTOUT").c_str(), isOptOutTimerRunning ? "TRUE" : "FALSE");
}

/*
 *  Function: publishOptOutValueToMqtt
 *  Description: Handles callbacks from MQTT subscriptions. Identifies the target topic and redirects
 *  Parameters: char, byte, unsigned int - provided by the MQTT callback by default
 *  Returns: None
 */
void subscriptionCallback(char* topic, byte* payload, unsigned int length) {
  String expectedTopicPrefix = String(MQTT_ESP_CMND_TOPIC) + String(MQTT_CONTROLLER_ID);
  String topicStr = String(topic);

  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  if (topicStr.startsWith(expectedTopicPrefix + "/") && topicStr.endsWith("/set") && payloadStr.length() > 0) {
    int prefixEndIndex = expectedTopicPrefix.length();
    int setStartIndex = topicStr.lastIndexOf("/set");
    String subtopic = topicStr.substring(prefixEndIndex + 1, setStartIndex);

    if (subtopic == "MOTION_THRESHOLD") { //Switch cases use INT and are not compatible with Strings. Therefore multiple if/else are needed
      updateMotionThreshold(payloadStr);
    } else if (subtopic == "OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES") {
      updateButtonTimeout(payloadStr);
    } else if (subtopic == "INACTIVITY_TIMEOUT_IN_MINUTES") {
      updateInactivityTimeout(payloadStr);
    } else if (subtopic == "SAMPLE_SPEED_IN_MILLISECONDS") {
      updateSampleSpeed(payloadStr);
    } else if (subtopic == "OPTOUT") {
      updateOptOut(payloadStr);
    } else if (subtopic == "MIMIMUM_DETECTION_COUNT_REQUIRED"){
      updateMinimumMotionCountRequired(payloadStr);
    } else if (subtopic == "MIMIMUM_DETECTION_COUNT_TIMEFRAME"){
      updateMinimumMotionCountTimeframe(payloadStr);
    }

    publishCommandValuesToMqtt();
  }
}

/*
 *  Function: turnSmartSwitchOn
 *  Description: Sends a message to MQTT to enable or disable power for the workstation through the smart power plug
 *  Parameters: bool - desired status of the MQTT enabled smart power plug
 *  Returns: None
 */
void turnSmartSwitchOn(const bool& state) {
  mqttPublishWithRetry((String(MQTT_SWITCH_CMND_TOPIC_PREFIX) + String(MQTT_SWITCH_TOPIC_SUFFIX) + "/POWER").c_str(), state ? "1" : "0");
}

/*
 *  Function: mqttPublishWithRetry
 *  Description: Publishes a message that is critical for system functionality multiple times if it fails
 *  Parameters: bool - desired status of the MQTT enabled smart power plug
 *  Returns: None
 */
void mqttPublishWithRetry(const char* topic, const char* payload,const bool& retain){  
  for(int i = 0; i < MQTT_PUBLISH_ATTEMPTS; i++){
    if(mqttClient.publish(topic, payload, retain)){
      return;
    }
    delay(MQTT_PUBLISH_RETRY_DELAY_IN_MILLISECONDS);
  }
  ledBlinkError();
}