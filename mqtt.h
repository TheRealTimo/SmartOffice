#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "config.h"
#include "wifi.h"
#include "led.h"
#include "configurator.h"
#include "variables.h"

extern PubSubClient mqttClient;

extern bool isMqttSetup;

void setupMqtt();
void connectMqtt();
void publishOccupancyStatusToMqtt();
void publishTelemetryToMqtt();
void publishCommandValuesToMqtt();
void publishOptOutValueToMqtt();
void subscriptionCallback(char* topic, byte* payload, unsigned int length);
void turnSmartSwitchOn(const bool& state);
void mqttPublishWithRetry(const char* topic, const char* payload, const bool& retain = false);

#endif