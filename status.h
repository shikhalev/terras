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

class Status {
  public:
    static void set(byte value);
    static byte get();

  public:
    static bool red();
    static bool green();
    static bool blue();
    static bool blink();

  public:
    static void begin(byte red_pin, byte green_pin, byte blue_pin);
    static void tick();
};
