#ifndef OPTOUT_H
#define OPTOUT_H

#include <stdint.h>
#include "led.h"
#include "mqtt.h"
#include "variables.h"

extern const uint8_t optOutButtonPin;

void startOptOutTimer();

#endif