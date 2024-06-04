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
#define IMU_CONNECTION_ATTEMPTS 10

#define IMU_CALIBRATION_TIME_IN_SECONDS 10
#define MOTION_THRESHOLD 0.0005
#define INACTIVITY_TIMEOUT_IN_MINUTES 45
#define SAMPLE_SPEED_IN_MILLISECONDS 10

#define OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE 240

#define MINIMUM_MOTION_DETECTION_COUNT_REQUIRED 3
#define MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_TIMEFRAME 10

#define MQTT_PUBLISH_ATTEMPTS 3
#define MQTT_PUBLISH_RETRY_DELAY_IN_MILLISECONDS 500


/*
MQTT related settings below
________________________________________________
*/
#define MQTT_HOST "192.168.1.110"
#define MQTT_PORT 1883
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "password"

#define MQTT_CONNECTION_TIMEOUT_IN_SECONDS 30

#endif