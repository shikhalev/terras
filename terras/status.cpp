#include "status.h"

static byte _value;

void Status::set(byte value) {
  if (Serial) {
    Serial.print(F("Set status = "));
    switch (value) {
      case STATUS_INIT: 
        Serial.println(F("STATUS_INIT"));
        break;
      case STATUS_OK:
        Serial.println(F("STATUS_OK"));
        break;
      case STATUS_NO_SENSORS:
        Serial.println(F("STATUS_NO_SENSORS"));
        break;
      case STATUS_SENSOR_ERROR:
        Serial.println(F("STATUS_SENSOR_ERROR"));
        break;
      case STATUS_DISPLAY_ERROR:
        Serial.println(F("STATUS_DISPLAY_ERROR"));
        break;
      case STATUS_WARNING:
        Serial.println(F("STATUS_WARNING"));
        break;
      case STATUS_DANGEROUS:
        Serial.println(F("STATUS_DANGEROUS"));
        break;
      default:
        Serial.print(F("0b"));
        Serial.println(value, BIN);
    }
    Serial.flush();
  }
  _value = value;
}

inline byte Status::get() {
  return _value;
}

inline bool Status::red() {
  return (_value & RED) != 0;
}

inline bool Status::green() {
  return (_value & GREEN) != 0;
}

inline bool Status::blue() {
  return (_value & BLUE) != 0;
}

inline bool Status::blink() {
  return (_value & BLINK) != 0;
}

static byte _red_pin;
static byte _green_pin;
static byte _blue_pin;

void Status::begin(byte red_pin, byte green_pin, byte blue_pin) {
  _red_pin = red_pin;
  _green_pin = green_pin;
  _blue_pin = blue_pin;
  pinMode(_red_pin, OUTPUT);
  pinMode(_green_pin, OUTPUT);
  pinMode(_blue_pin, OUTPUT);
}

void Status::begin() {
  begin(PIN_RED, PIN_GREEN, PIN_BLUE);
}

void Status::tick() {
  static bool state = true;
  bool r = red();
  bool g = green();
  bool b = blue();
  if (blink()) {
    r = r && state;
    g = g && state;
    b = b && state;
    state = !state;
  }
  // TODO: поменять полярность на прямую
  digitalWrite(_red_pin, r);
  digitalWrite(_green_pin, g);
  digitalWrite(_blue_pin, b);
}
