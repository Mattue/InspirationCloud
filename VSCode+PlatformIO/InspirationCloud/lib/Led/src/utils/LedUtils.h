#pragma once

#ifndef LedUtils_h
#define LedUtils_h

#include <Arduino.h>
#include <FastLED.h>

class LedUtils
{
public:
    static unsigned int hexToDec(String hexString);
    static CRGB::HTMLColorCode colorToHex(String color);
};

#endif