#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "config.h"
#include "wifi.h"
#include "led.h"


extern PubSubClient mqttClient;

extern bool isMqttSetup;

void setupMqtt();
void connectMqtt();

#endif