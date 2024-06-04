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

#define MOTION_THRESHOLD_DEFAULT_VALUE 0.0005
#define OPT_OUT_BUTTON_TIMEOUT_IN_MINUTES_DEFAULT_VALUE 240
#define INACTIVITY_TIMEOUT_IN_MINUTES_DEFAULT_VALUE 45
#define SAMPLE_SPEED_IN_MILLISECONDS_DEFAULT_VALUE 10

#define MINIMUM_MOTION_DETECTION_COUNT_REQUIRED_DEFAULT_VALUE 3
#define MINIMUM_MOTION_DETECTION_TIMEFRAME_IN_CYCLES_DEFAULT_VALUE 10

/*
MQTT related settings below
________________________________________________
*/
#define MQTT_HOST "192.168.1.110"
#define MQTT_PORT 1883
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "password"

#define MQTT_SWITCH_CMND_TOPIC_PREFIX "smartOffice/plugs/cmnd/"
#define MQTT_SWITCH_TOPIC_SUFFIX "plug3"
#define MQTT_ESP_STATUS_TOPIC "smartOffice/esps/stat/esp"
#define MQTT_ESP_TELE_TOPIC "smartOffice/esps/tele/esp"
#define MQTT_ESP_CMND_TOPIC "smartOffice/esps/cmnd/esp"


#define MQTT_CONTROLLER_ID "3"

#define MQTT_CONNECTION_TIMEOUT_IN_SECONDS 30
#define MQTT_PUBLISH_ATTEMPTS 3
#define MQTT_PUBLISH_RETRY_DELAY_IN_MILLISECONDS 500

#endif