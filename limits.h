#pragma once

#include "status.h"

// TODO: реализовать настраиваемые лимиты

#define TEMPERATURE_WARN_LOW  23.0
#define TEMPERATURE_WARN_HIGH 30.0
#define TEMPERATURE_CRIT_LOW  20.0
#define TEMPERATURE_CRIT_HIGH 33.0

#define HUMIDITY_WARN_LOW   65.0
#define HUMIDITY_WARN_HIGH  80.0
#define HUMIDITY_CRIT_LOW   60.0
#define HUMIDITY_CRIT_HIGH  90.0

// TODO: уточнить
#define CO2_WARN_HIGH 800
#define CO2_CRIT_HIGH 1200

class Limits {
  public:
    static void validate(float temperature, float humidity);
    static void validate(uint16_t co2);
    static void validate(float temperature, float humidity, uint16_t co2);
};
 
