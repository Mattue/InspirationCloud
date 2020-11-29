#pragma once

#ifndef Status_h
#define Status_h

#include <Arduino.h>

/*
    returning statuses:
        -1 - idle
        0 - led switch off
        1 - led filled
        2 - led rainbow operating
        3 - return system status
*/

#define STATUS_IDLE -1
#define STATUS_LED_SWITCH_OFF 0
#define STATUS_LED_FILL 1
#define STATUS_LED_RAINBOW 2
#define STATUS_SYSTEM_STATUS 3

struct Status
{
    String ip;
    int status;
};

#endif