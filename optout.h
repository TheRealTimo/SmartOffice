#ifndef OPTOUT_H
#define OPTOUT_H

#include <stdint.h>
#include "led.h"
#include "config.h"

extern const uint8_t optOutButtonPin;

void startOptOutTimer();

#endif