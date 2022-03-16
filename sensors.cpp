#include "sensors.h"

#define SHT2x_ADDRESS 0x40
#define SGP30_ADDRESS 0x58

const byte MAX_CHANNEL = 7;

void Sensors::begin() {
  Wire.begin();
  mux.begin(Wire);
  mux.closeAll();
  scan();
}

bool Sensors::has_sht(byte channel) {
  return (sht_flags & (1 << channel)) != 0;
}

bool Sensors::has_sgp(byte channel) {
  return (sgp_flags & (1 << channel)) != 0;
}

void Sensors::scan() {
  // TODO: log in Serial
  for (byte ch = 0; ch <= MAX_CHANNEL; ++ch) {
    byte mask = 1 << ch;
    byte n_mask = ~mask;
    byte error;
    mux.openChannel(ch);
    Wire.beginTransmission(SHT2x_ADDRESS);
    error = Wire.endTransmission();
    if (error == 0) {
      sht_flags |= mask;
      sht_sensors[ch].begin();
    } else {
      sht_flags &= n_mask;
    }
    Wire.beginTransmission(SGP30_ADDRESS);
    error = Wire.endTransmission();
    if (error == 0) {
      sgp_flags |= mask;
    } else {
      sgp_flags &= n_mask;
    }
    if (sht_flags == 0 && sgp_flags == 0) {
      Status::set(STATUS_NO_SENSORS);
    } else {
      Status::set(STATUS_OK);
    }
    mux.closeChannel(ch);
  }
}

void Sensors::tick() {
  for (byte ch = 0; ch <= MAX_CHANNEL; ++ch) {
    mux.openChannel(ch);
    if (has_sht(ch)) {
      if (sht_sensors[ch].read()) {
        // TODO
      }
    }
    mux.closeChannel(ch);
  }
}

  
