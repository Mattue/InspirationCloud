#ifndef Led_h
#define Led_h

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 73
#define PIN1 12
#define PIN2 13
#define PIN3 15

class Led {
    public:
        Led();
        void switchOff();
        void rainbow();
        void color(int color);
        void color(String color);
    private:
        CRGB leds[NUM_LEDS];
        byte counter;
};

#endif