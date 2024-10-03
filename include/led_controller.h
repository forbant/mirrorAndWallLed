#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>

void setupLEDs();
void handleState();
void colorUpdate();
void setColorToStrip(int strip, uint32_t color);
void fillStrip(CRGB* strip, int numberOfLeds, CRGB color);
void fillStrip(CRGB* strip, int numberOfLeds, CHSV color);
uint32_t CRGBtoUInt32(CRGB color);
CHSV extractColorFromUint32(uint32_t u_color);
CHSV rgb2hsv(CRGB rgb);
CHSV rgb2hsv_approx(const CRGB& rgb);

extern CRGB leds_wall[];
extern CRGB leds_mirror[];
extern uint8_t hue_mirror, saturation_mirror, value_mirror, hue_wall, saturation_wall, value_wall;
extern enum LedState { MIRROR_ONLY, BOTH, WALL_ONLY } state;

#endif