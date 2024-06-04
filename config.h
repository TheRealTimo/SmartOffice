#ifndef CONFIG_H
#define CONFIG_H

/*
Network related settings below
________________________________________________
*/
#define WIFI_SSID "SmartOfficePrototype"
#define WIFI_PSK "Password24!"
#define WIFI_TIMEOUT_IN_SECONDS 30

/*
IMU related settings below
________________________________________________
*/
#define IMU_CALIBRATION_TIME_IN_SECONDS 10

/*
MQTT related settings below
________________________________________________
*/
#define MQTT_HOST "192.168.1.110"
#define MQTT_PORT 1883
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "password"

#endif