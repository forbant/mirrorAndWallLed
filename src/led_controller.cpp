#include "led_controller.h"
#include "eeprom_manager.h"

#define PIN_WALL 13
#define PIN_MIRROR 14
#define NUM_LEDS_WALL 146
#define NUM_LEDS_MIRROR 330
#define HUE_STEP 1

CRGB leds_wall[NUM_LEDS_WALL];
CRGB leds_mirror[NUM_LEDS_MIRROR];

uint8_t hue_mirror;
uint8_t saturation_mirror;
uint8_t value_mirror;
uint8_t hue_wall;
uint8_t saturation_wall;
uint8_t value_wall;
LedState state;

void setupLEDs() {
  FastLED.addLeds<WS2812B, PIN_WALL, GRB>(leds_wall, NUM_LEDS_WALL);
  FastLED.addLeds<WS2812B, PIN_MIRROR, GRB>(leds_mirror, NUM_LEDS_MIRROR);
  FastLED.clear();
  FastLED.show();
}

void handleState() {
  CHSV color_mirror = CHSV(hue_mirror, saturation_mirror, value_mirror);
  CHSV color_wall = CHSV(hue_wall, saturation_wall, value_wall);
  switch (state) {
    case WALL_ONLY:
      Serial.println("WALL_ONLY");
      fillStrip(leds_wall, NUM_LEDS_WALL, color_mirror);
      fillStrip(leds_mirror, NUM_LEDS_MIRROR, CRGB::Black);
      break;
    case BOTH:
      Serial.println("BOTH");
      fillStrip(leds_wall, NUM_LEDS_WALL, color_mirror);
      fillStrip(leds_mirror, NUM_LEDS_MIRROR, color_wall);
      break;
    case MIRROR_ONLY:
      Serial.println("MIRROR_ONLY");
      fillStrip(leds_wall, NUM_LEDS_WALL, CRGB::Black);
      fillStrip(leds_mirror, NUM_LEDS_MIRROR, color_wall);
      break;
  }
  delay(10);
  Serial.println("SWITCH OUT");
  FastLED.show();
}

void colorUpdate() {
  hue_mirror += HUE_STEP;
  hue_wall += HUE_STEP;
  if (hue_mirror >= 255) {
    hue_mirror = 0;
    saturation_mirror = (saturation_mirror == 255) ? 0 : 255;
  }
  if (hue_wall >= 255) {
    hue_wall = 0;
    saturation_wall = (saturation_wall == 255) ? 0 : 255;
  }
  handleState();
}

void fillStrip(CRGB* strip, int numberOfLeds, CRGB color) {
  CHSV hsv_color = rgb2hsv_approx(color);
  fillStrip(strip, numberOfLeds, hsv_color);
}

void fillStrip(CRGB* strip, int numberOfLeds, CHSV color) {
  for(int i = 0; i < numberOfLeds; i++) {
    strip[i] = color;
  }
  FastLED.show();
}

CHSV rgb2hsv(CRGB rgb) {
  CHSV hsv;
  unsigned char min, max, delta;

  min = rgb.r < rgb.g ? rgb.r : rgb.g;
  min = min < rgb.b ? min : rgb.b;

  max = rgb.r > rgb.g ? rgb.r : rgb.g;
  max = max > rgb.b ? max : rgb.b;

  hsv.v = max;
  delta = max - min;
  if (delta < 1) {
      hsv.s = 0;
      hsv.h = 0; // undefined, maybe nan?
      return hsv;
  }
  if (max > 0) {
      hsv.s = (delta * 255) / max;
  } else {
      // if max is 0, then r = g = b = 0
      hsv.s = 0;
      hsv.h = 0; // undefined, maybe nan?
      return hsv;
  }
  if (rgb.r >= max)
      hsv.h = (rgb.g - rgb.b) * 255 / delta;        // between yellow & magenta
  else if (rgb.g >= max)
      hsv.h = 85 + (rgb.b - rgb.r) * 255 / delta;  // between cyan & yellow
  else
      hsv.h = 171 + (rgb.r - rgb.g) * 255 / delta;  // between magenta & cyan

  return hsv;
}

CHSV rgb2hsv_approx(const CRGB& rgb) {
    // Extract RGB values
    uint8_t r = rgb.r;
    uint8_t g = rgb.g;
    uint8_t b = rgb.b;

    // Find the minimum and maximum values of r, g, b
    uint8_t minVal = min(r, min(g, b));
    uint8_t maxVal = max(r, max(g, b));

    // Calculate the Value (V) component
    uint8_t v = maxVal;

    // Calculate the Saturation (S) component
    uint8_t delta = maxVal - minVal;
    uint8_t s = (maxVal == 0) ? 0 : (255 * delta) / maxVal;

    // Calculate the Hue (H) component
    uint8_t h = 0;

    if (delta > 0) {
        if (maxVal == r) {
            h = 43 * (g - b) / delta;
        } else if (maxVal == g) {
            h = 85 + 43 * (b - r) / delta;
        } else {
            h = 171 + 43 * (r - g) / delta;
        }
    }

    if (h < 0) {
        h += 255; // Ensure hue is within 0-255 range
    }

    // Return the CHSV color
    return CHSV(h, s, v);
}

void setColorToStrip(int strip, uint32_t u_color) {
  CHSV hsv_color = extractColorFromUint32(u_color);
  if (strip == 1) {
    fillStrip(leds_mirror, NUM_LEDS_MIRROR, hsv_color);
  } else if (strip == 2) {
    fillStrip(leds_wall, NUM_LEDS_WALL, hsv_color);
  }

  FastLED.show();
}

CHSV extractColorFromUint32(uint32_t u_color) {
  CRGB extracted = CRGB(u_color >> 16, (u_color >> 8) & 0xFF, u_color & 0xFF);
  CHSV converted = rgb2hsv_approx(extracted);
  return converted;
}

uint32_t CRGBtoUInt32(CRGB color) {
    return ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | color.b;
}
