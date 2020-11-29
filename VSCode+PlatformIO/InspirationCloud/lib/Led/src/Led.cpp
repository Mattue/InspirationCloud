#include "Led.h"

Led::Led()
{

  FastLED.setMaxPowerInVoltsAndMilliamps(4, 1000);

  //LED strips init
  FastLED.addLeds<WS2812B, PIN1, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN2, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN3, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Led::switchOff();

  FastLED.setBrightness(70);
}

void Led::setColorByPercent(int percent)
{
  int ledsToFill = ((NUM_LEDS * percent) / 100);
  for (int i = 0; i < ledsToFill; i++)
  {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void Led::switchOff()
{
  FastLED.clear();
  FastLED.show();
}

void Led::rainbow()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    //TODO: make is smooth
    leds[i] = CHSV(counter + i * 2, 255, 255); // HSV. Увеличивать HUE (цвет)
  }
  counter++; // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
}

void Led::color(CRGB::HTMLColorCode color)
{

#if DEBUG_MODE == 1
  Serial.println("DEBUG: Filling with " + String(color));
#endif

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }

  FastLED.show();
}

void Led::color(unsigned int color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }

  FastLED.show();
}

void Led::color(String color)
{
#if DEBUG_MODE == 1
  Serial.println("DEBUG: Filling with " + color);
#endif

  color.toUpperCase();

  if (color.startsWith("#"))
  {
    this->color(LedUtils::hexToDec(color));
  }
  else
  {
    this->color(LedUtils::colorToHex(color));
  }
}