#include "status.h"

byte _value;
byte _red_pin;
byte _green_pin;
byte _blue_pin;

void Status::set(byte value) {
  if (Serial) {
    Serial.print("Set status = ");
    switch (value) {
      case STATUS_INIT: 
        Serial.println("STATUS_INIT");
        break;
      case STATUS_OK:
        Serial.println("STATUS_OK");
        break;
      case STATUS_NO_SENSORS:
        Serial.println("STATUS_NO_SENSORS");
        break;
      case STATUS_SENSOR_ERROR:
        Serial.println("STATUS_SENSOR_ERROR");
        break;
      case STATUS_DISPLAY_ERROR:
        Serial.println("STATUS_DISPLAY_ERROR");
        break;
      case STATUS_WARNING:
        Serial.println("STATUS_WARNING");
        break;
      case STATUS_DANGEROUS:
        Serial.println("STATUS_DANGEROUS");
        break;
      default:
        Serial.print("0b");
        Serial.println(value, BIN);
    }
    Serial.flush();
  }
  _value = value;
}

byte Status::get() {
  return _value;
}

bool Status::red() {
  return (_value & RED) != 0;
}

bool Status::green() {
  return (_value & GREEN) != 0;
}

bool Status::blue() {
  return (_value & BLUE) != 0;
}

bool Status::blink() {
  return (_value & BLINK) != 0;
}

void Status::begin(byte red_pin, byte green_pin, byte blue_pin) {
  _red_pin = red_pin;
  _green_pin = green_pin;
  _blue_pin = blue_pin;
}

void Status::tick() {
  static bool _state = false;
  bool r = (_value & RED) != 0;
  bool g = (_value & GREEN) != 0;
  bool b = (_value & BLUE) != 0;
  if ((_value & BLINK) != 0) {
    r = r && _state;
    g = g && _state;
    b = b && _state;
    _state = !_state;
  }
  digitalWrite(_red_pin, r);
  digitalWrite(_green_pin, g);
  digitalWrite(_blue_pin, b);
}
