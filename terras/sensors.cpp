#include "sensors.h"

#define SHT2x_ADDRESS 0x40
#define SGP30_ADDRESS 0x58

static byte sht_mask;
static byte sgp_mask;

static TCA9548A MX;
static SHT2x sht_sensors[8];
static SGP30 sgp_sensors[8];

#define MAX_CHANNEL 7

void Sensors::reset() {
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

    SHT2x &SHT = sht_sensors[channel];
    SGP30 &SGP = sgp_sensors[channel];
    
    Wire.beginTransmission(SHT2x_ADDRESS);
    error = Wire.endTransmission();
    if (error == 0) {
      sht_mask |= mask;
      SHT.begin(); 
      SHT.reset();
      if (Serial) {
        Serial.print(F("Found SHT2x sensor at channel "));
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
      SGP.begin();
      SGP.GenericReset();
      if (Serial) {
        Serial.print(F("Found SGP30 sensor at channel "));
        Serial.print(channel, DEC);
        Serial.print(F(" with Id = "));
        SGP.getID();
        for (int i = 0; i < 6; i++) {
          if (SGP._id[i] < 0x10) 
            Serial.print(0);    
          Serial.print(SGP._id[i], HEX);
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
      Serial.print(F("Found "));
      Serial.print(sht_count, DEC);
      Serial.print(F(" SHT2x sensors and "));
      Serial.print(sgp_count, DEC);
      Serial.println(F(" SGP30 sensors."));
    }
    Status::set(STATUS_OK);
  }
}

void Sensors::begin() {
  Wire.begin();
  MX.begin(Wire);
}

Data data[8];

Data Sensors::getData(byte channel) {
//  Data item;
//  memcpy(&item, &data[channel], sizeof(Data));
  return data[channel];
}

void Sensors::tick() {
  Status::set(STATUS_OK);
  for (byte channel = 0; channel <= MAX_CHANNEL; ++channel) {
    byte mask = 1 << channel;
    bool has_sht = (sht_mask & mask) != 0;
    bool has_sgp = (sgp_mask & mask) != 0;
    if (has_sht || has_sgp) {
      float temperature;
      float humidity;

      SHT2x &SHT = sht_sensors[channel];
      SGP30 &SGP = sgp_sensors[channel];
      
      MX.openChannel(channel);

      data[channel].flags = 0;

      bool sht_readed = false;
      if (has_sht) {
        sht_readed = SHT.read();
        temperature = SHT.getTemperature();
        humidity = SHT.getHumidity();
        int sht_error = SHT.getError();
        if (sht_error != 0) {
          sht_readed = false;
        }
        if (sht_readed) {
          data[channel].flags |= DATA_SHT2X;
          data[channel].flags &= ~ERROR_SHT2X;
          data[channel].sht20.data.temperature = temperature;
          data[channel].sht20.data.humidity = humidity;
          Limits::validate(temperature, humidity);
          if (Serial) {
            Serial.print(F("Read SHT2x at channel "));
            Serial.print(channel, DEC);
            Serial.print(F(": temperature = "));
            Serial.print(temperature, 1);
            Serial.print(F(", humidity = "));
            Serial.print(humidity, 1);
            Serial.println(F("."));
          }
        } else {
          data[channel].flags &= ~DATA_SHT2X;
          data[channel].flags |= ERROR_SHT2X;
          data[channel].sht20.error = sht_error;
          if (Serial) {
            Serial.print(F("Can not read SHT2x at channel "));
            Serial.print(channel, DEC);
            Serial.print(F(": error = 0x"));
            Serial.print(sht_error, HEX);
            Serial.println(F("."));
          }
          Status::set(STATUS_SENSOR_ERROR);
        }
      }

      bool sgp_readed = false;
      if (has_sgp) {
        if (sht_readed) {
          SGP.setRelHumidity(temperature, humidity);
        }
        sgp_readed = SGP.measure(false);
        uint16_t co2 = SGP.getCO2();
        int sgp_error = SGP.lastError();
        if (sgp_error != 0) {
          sgp_readed = false;
        }
        if (sgp_readed) {
          data[channel].flags |= DATA_SGP30;
          data[channel].flags &= ~ERROR_SGP30;
          data[channel].sgp30.co2 = co2;
          Limits::validate(co2);
          if (Serial) {
            Serial.print(F("Read SGP30 at channel "));
            Serial.print(channel, DEC);
            Serial.print(F(": CO2 = "));
            Serial.print(co2, DEC);
            Serial.println(F("."));
          }
        } else {
          data[channel].flags &= ~DATA_SGP30;
          data[channel].flags |= ERROR_SGP30;
          data[channel].sgp30.error = sgp_error;
          if (Serial) {
            Serial.print(F("Can not read SGP30 at channel "));
            Serial.print(channel, DEC);
            Serial.print(F(": error = 0x"));
            Serial.print(sgp_error, HEX);
            Serial.println(F("."));
          }
          Status::set(STATUS_SENSOR_ERROR);
        }
      }
      
      MX.closeChannel(channel);
    }
  }
}
