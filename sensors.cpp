#include "sensors.h"

#define SHT2x_ADDRESS 0x40
#define SGP30_ADDRESS 0x58

static byte sht_mask;
static byte sgp_mask;

static TCA9548A MX;
static SHT2x sht_sensors[8];
static SGP30 sgp_sensors[8];

#define MAX_CHANNEL 7

void Sensors::scan() {
  Status::set(STATUS_INIT);
  sht_mask = 0;
  sgp_mask = 0;
  MX.closeAll();

  byte sht_count = 0;
  byte sgp_count = 0;
  for (byte channel = 0; channel <= MAX_CHANNEL; ++channel) {
    byte mask = 1 << channel;
    byte neg_mask = ~mask;
    byte error;
    MX.openChannel(channel);
    
    Wire.beginTransmission(SHT2x_ADDRESS);
    error = Wire.endTransmission();
    if (error == 0) {
      sht_mask |= mask;
      sht_sensors[channel].begin();
      if (Serial) {
        Serial.print("Found SHT2x sensor at channel ");
        Serial.println(channel, DEC);
      }
      ++sht_count;
    } else {
      sht_mask &= neg_mask;
    }

    Wire.beginTransmission(SGP30_ADDRESS);
    error = Wire.endTransmission();
    if (error == 0) {
      sgp_mask |= mask;
      sgp_sensors[channel].begin();
      sgp_sensors[channel].GenericReset();
      if (Serial) {
        Serial.print("Found SGP30 sensor at channel ");
        Serial.print(channel, DEC);
        Serial.print(" with Id = ");
        sgp_sensors[channel].getID();
        for (int i = 0; i < 6; i++) {
          if (sgp_sensors[channel]._id[i] < 0x10) 
            Serial.print(0);    
          Serial.print(sgp_sensors[channel]._id[i], HEX);
        }
        Serial.println();
      }
      ++sgp_count;
    } else {
      sgp_mask &= neg_mask;
    }
    
    MX.closeChannel(channel);
  }
  
  if (sht_mask == 0 && sgp_mask == 0) {
    Status::set(STATUS_NO_SENSORS);
  } else {
    if (Serial) {
      Serial.print("Found ");
      Serial.print(sht_count, DEC);
      Serial.print(" SHT2x sensors and ");
      Serial.print(sgp_count, DEC);
      Serial.println(" SGP30 sensors.");
    }
    Status::set(STATUS_OK);
  }
}

void Sensors::begin() {
  Wire.begin();
  MX.begin(Wire);
}

static Data data[0];

void Sensors::tick() {
  Status::set(STATUS_OK);
  for (byte channel = 0; channel <= MAX_CHANNEL; ++channel) {
    byte mask = 1 << channel;
    bool has_sht = (sht_mask & mask) != 0;
    bool has_sgp = (sgp_mask & mask) != 0;
    if (has_sht || has_sgp) {
      float temperature;
      float humidity;
      
      MX.openChannel(channel);

      bool sht_readed = false;
      if (has_sht) {
        sht_readed = sht_sensors[channel].read();
        temperature = sht_sensors[channel].getTemperature();
        humidity = sht_sensors[channel].getHumidity();
        int sht_error = sht_sensors[channel].getError();
        if (sht_error != 0) {
          sht_readed = false;
        }
        if (sht_readed) {
          data[channel].flags |= DATA_SHT2X;
          data[channel].flags &= ~ERROR_SHT2X;
          data[channel].temperature = temperature;
          data[channel].humidity = humidity;
          Limits::validate(temperature, humidity);
          if (Serial) {
            Serial.print("Read SHT2x at channel ");
            Serial.print(channel, DEC);
            Serial.print(": temperature = ");
            Serial.print(temperature, 1);
            Serial.print(", humidity = ");
            Serial.print(humidity, 1);
            Serial.print(".");
          }
        } else {
          data[channel].flags &= ~DATA_SHT2X;
          data[channel].flags |= ERROR_SHT2X;
          data[channel].sht_error = sht_error;
          if (Serial) {
            Serial.print("Can not read SHT2x at channel ");
            Serial.print(channel, DEC);
            Serial.print(": error = 0x");
            Serial.print(sht_error, HEX);
            Serial.println(".");
          }
          Status::set(STATUS_SENSOR_ERROR);
        }
      }

      bool sgp_readed = false;
      if (has_sgp) {
        if (sht_readed) {
          sgp_sensors[channel].setRelHumidity(temperature, humidity);
        }
        sgp_readed = sgp_sensors[channel].measure(false);
        uint16_t co2 = sgp_sensors[channel].getCO2();
        int sgp_error = sgp_sensors[channel].lastError();
        if (sgp_error != 0) {
          sgp_readed = false;
        }
        if (sgp_readed) {
          data[channel].flags |= DATA_SGP30;
          data[channel].flags &= ~ERROR_SGP30;
          data[channel].co2 = co2;
          Limits::validate(co2);
          if (Serial) {
            Serial.print("Read SGP30 at channel ");
            Serial.print(channel, DEC);
            Serial.print(": CO2 = ");
            Serial.print(co2, DEC);
            Serial.print(".");
          }
        } else {
          data[channel].flags &= ~DATA_SGP30;
          data[channel].flags |= ERROR_SGP30;
          data[channel].sgp_error = sgp_error;
          if (Serial) {
            Serial.print("Can not read SGP30 at channel ");
            Serial.print(channel, DEC);
            Serial.print(": error = 0x");
            Serial.print(sgp_error, HEX);
            Serial.println(".");
          }
          Status::set(STATUS_SENSOR_ERROR);
        }
        //
      }
      
      MX.closeChannel(channel);
    }
    //
  }
  // TODO
}

//
//void Sensors::tick() {
//  for (byte ch = 0; ch <= MAX_CHANNEL; ++ch) {
//    mux.openChannel(ch);
//    if (has_sht(ch)) {
//      if (sht_sensors[ch].read()) {
//        // TODO
//      }
//    }
//    mux.closeChannel(ch);
//  }
//}
//
//  
