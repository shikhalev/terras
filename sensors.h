#include <Arduino.h>
#include <Wire.h>
#include "TCA9548A.h"
#include "SHT2x.h"

#include "status.h"

class Sensors {
  public:
    void begin();

  private:
    TCA9548A mux;
  
    void scan();

    SHT2x sht_sensors[8];

  public:
    bool has_sht(byte channel);
    bool has_sgp(byte channel);
  
  private:
    byte sht_flags = 0;
    byte sgp_flags = 0;

  public:
    void tick();
};
