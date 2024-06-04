#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "config.h"
#include "wifi.h"
#include "led.h"
#include "variables.h"


extern PubSubClient mqttClient;

extern bool isMqttSetup;

void setupMqtt();
void connectMqtt();

void publishOccupancyStatusToMqtt();
void turnSmartSwitchOn(const bool& state);

#endif