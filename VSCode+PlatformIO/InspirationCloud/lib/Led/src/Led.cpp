#include "Led.h"

Led::Led() {
    //LED strips init
  FastLED.addLeds<WS2812B, PIN1, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN2, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, PIN3, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  FastLED.setBrightness(70);
}

void Led::switchOff() {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    }

    FastLED.show();
}

void Led::rainbow() {
  for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети
    leds[i] = CHSV(counter + i * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
}

void Led::color(int color) {
  
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = color;
  }

  FastLED.show();
}

void Led::color(String color) {
  
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CRGB::Red;
  }

  FastLED.show();
}