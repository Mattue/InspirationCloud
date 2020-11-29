#pragma once

#ifndef Led_h
#define Led_h

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <FastLED.h>
#include <utils\LedUtils.h>

#define NUM_LEDS 73
#define PIN1 12
#define PIN2 13
#define PIN3 15

#ifndef RAINBOW_SPEED
#define RAINBOW_SPEED 500 //rainbow speed in ms
#endif

class Led
{
public:
    Led();
    void switchOff();
    void ICACHE_RAM_ATTR rainbow();
    void color(CRGB::HTMLColorCode color);
    void color(String color);
    void color(unsigned int color);
    void setColorByPercent(int percent);

private:
    CRGB leds[NUM_LEDS];
    byte counter;
    unsigned long lastMillis;
    static CRGB::HTMLColorCode colorToHex(String color);
};

#endif