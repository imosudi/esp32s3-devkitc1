#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   38   // try 48, then 2, then 8 etc.
#define LED_COUNT 1

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  // simple rainbow cycle
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color((i + j) & 255, (i * 2 + j) & 255, (i * 3 + j) & 255));
    }
    strip.show();
    delay(20);
  }
}
