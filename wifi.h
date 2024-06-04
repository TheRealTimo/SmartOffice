#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include "led.h"

void connectToWifi();

extern WiFiClient espWifiClient;

#endif