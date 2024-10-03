#include "eeprom_manager.h"
#include "led_controller.h"
#include <EEPROM.h>

#define EEPROM_HUE_MIRROR 0
#define EEPROM_SATURATION_MIRROR 1
#define EEPROM_VALUE_MIRROR 2
#define EEPROM_HUE_WALL 3
#define EEPROM_SATURATION_WALL 4
#define EEPROM_VALUE_WALL 5
#define EEPROM_STATE 6
#define EEPROM_INITIALIZED 7
#define EEPROM_INIT_VALUE 41

void saveToEEPROM() {
  EEPROM.write(EEPROM_HUE_MIRROR, hue_mirror);
  EEPROM.write(EEPROM_SATURATION_MIRROR, saturation_mirror);
  EEPROM.write(EEPROM_VALUE_MIRROR, value_mirror);
  EEPROM.write(EEPROM_HUE_WALL, hue_wall);
  EEPROM.write(EEPROM_SATURATION_WALL, saturation_wall);
  EEPROM.write(EEPROM_VALUE_WALL, value_wall);
  EEPROM.write(EEPROM_STATE, static_cast<uint8_t>(state));
  EEPROM.write(EEPROM_INITIALIZED, EEPROM_INIT_VALUE);
  bool saved = EEPROM.commit();
  Serial.print("SAVED: "); Serial.println(saved);
  Serial.print("hm "); Serial.println(hue_mirror);
  Serial.print("sm "); Serial.println(saturation_mirror);
  Serial.print("vm "); Serial.println(value_mirror);
  Serial.print("hv "); Serial.println(hue_wall);
  Serial.print("sv "); Serial.println(saturation_wall);
  Serial.print("vv "); Serial.println(value_wall);
  Serial.print("state "); Serial.println(state);
}

void saveToEEPROM(uint32_t color_mirror, uint32_t color_wall) {
  CHSV colorMirror = extractColorFromUint32(color_mirror);
  CHSV colorWall = extractColorFromUint32(color_wall);

  hue_mirror = colorMirror.hue;
  saturation_mirror = colorMirror.saturation;
  value_mirror = colorMirror.value;

  hue_wall = colorWall.hue;
  saturation_wall = colorWall.saturation;
  value_wall = colorWall.value;

  saveToEEPROM();
}

void loadFromEEPROM() {
  Serial.println("LOAD");
  if (EEPROM.read(EEPROM_INITIALIZED) != EEPROM_INIT_VALUE) {
    Serial.println("INIT");
    hue_mirror = 0;
    saturation_mirror = 255;
    value_mirror = 255;
    hue_wall = 0;
    saturation_wall = 255;
    value_wall = 255;
    state = BOTH;
    saveToEEPROM();
  } else {
    Serial.println("SUCCESS");
    hue_mirror = EEPROM.read(EEPROM_HUE_MIRROR);
    saturation_mirror = EEPROM.read(EEPROM_SATURATION_MIRROR);
    value_mirror = EEPROM.read(EEPROM_VALUE_MIRROR);
    hue_wall = EEPROM.read(EEPROM_HUE_WALL);
    saturation_wall = EEPROM.read(EEPROM_SATURATION_WALL);
    value_wall = EEPROM.read(EEPROM_VALUE_WALL);
    state = static_cast<LedState>(EEPROM.read(EEPROM_STATE));

    Serial.print("hue_mirror "); Serial.println(hue_mirror);
    Serial.print("saturation_mirror "); Serial.println(saturation_mirror);
    Serial.print("value_mirror "); Serial.println(value_mirror);
    Serial.print("hue_wall "); Serial.println(hue_wall);
    Serial.print("saturation_wall "); Serial.println(saturation_wall);
    Serial.print("value_wall "); Serial.println(value_wall);
    Serial.print("state "); Serial.println(state);
  }
}