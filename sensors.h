#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "TCA9548A.h"
#include "SHT2x.h"
#include "SGP30.h"

#include "status.h"
#include "limits.h"

#define DATA_SHT2X  0b0001
#define DATA_SGP30  0b0010
#define ERROR_SHT2X 0b0100
#define ERROR_SGP30 0b1000

struct Data {
  byte flags;
  float temperature;
  float humidity;
  uint16_t co2;
  int sht_error;
  int sgp_error;
};

class Sensors {
  public:
    static void begin();
    static void tick();

  public:
    static void scan();
};
