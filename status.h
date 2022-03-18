#pragma once

#include <Arduino.h>

#define RED   0b0001
#define GREEN 0b0010
#define BLUE  0b0100
#define BLINK 0b1000

#define CYAN    (       GREEN | BLUE )
#define MAGENTA ( RED |         BLUE )
#define YELLOW  ( RED | GREEN        )

#define STATUS_INIT           ( GREEN   | BLINK )
#define STATUS_OK             ( GREEN           )
#define STATUS_NO_SENSORS     ( BLUE    | BLINK )
#define STATUS_SENSOR_ERROR   ( YELLOW  | BLINK )
#define STATUS_DISPLAY_ERROR  ( MAGENTA | BLINK )
#define STATUS_WARNING        ( YELLOW          )
#define STATUS_DANGEROUS      ( RED     | BLINK )

#define PIN_RED   4
#define PIN_GREEN 5
#define PIN_BLUE  6

class Status {
  public:
    static void set(byte value);
    inline static byte get();

  public:
    inline static bool red();
    inline static bool green();
    inline static bool blue();
    inline static bool blink();

  public:
    static void begin(byte red_pin, byte green_pin, byte blue_pin);
    static void begin();
    static void tick();
};
