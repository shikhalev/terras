#pragma once

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <SPI.h>

#include "sensors.h"

#define TFT_CS  10
#define TFT_RST 9 
#define TFT_DC  8

class Screen {
  public:
    static void begin();
    static void tick();
};
