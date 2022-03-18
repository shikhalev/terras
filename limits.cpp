#include "limits.h"

void Limits::validate(float temperature, float humidity) {
  if (temperature < HUMIDITY_CRIT_LOW || temperature > HUMIDITY_CRIT_HIGH) {
    if (Serial) {
      Serial.print(F("DANGEROUS: Temperature value = "));
      Serial.print(temperature, 2);
      Serial.print(F(", must be in "));
      Serial.print(HUMIDITY_CRIT_LOW);
      Serial.print(F(".."));
      Serial.print(HUMIDITY_CRIT_HIGH);
      Serial.println(".");
    }
    Status::set(STATUS_DANGEROUS);
  } else if (temperature < HUMIDITY_WARN_LOW || temperature > HUMIDITY_WARN_HIGH) {
    if (Serial) {
      Serial.print(F("WARNING: Temperature value = "));
      Serial.print(temperature, 2);
      Serial.print(F(", must be in "));
      Serial.print(HUMIDITY_WARN_LOW);
      Serial.print(F(".."));
      Serial.print(HUMIDITY_WARN_HIGH);
      Serial.println(F("."));
    }
    Status::set(STATUS_WARNING);
  }

  if (humidity < HUMIDITY_CRIT_LOW || humidity > HUMIDITY_CRIT_HIGH) {
    if (Serial) {
      Serial.print(F("DANGEROUS: Humidity value = "));
      Serial.print(humidity, 2);
      Serial.print(F(", must be in "));
      Serial.print(HUMIDITY_CRIT_LOW);
      Serial.print(F(".."));
      Serial.print(HUMIDITY_CRIT_HIGH);
      Serial.println(F("."));
    }
    Status::set(STATUS_DANGEROUS);
  } else if (humidity < HUMIDITY_WARN_LOW || humidity > HUMIDITY_WARN_HIGH) {
    if (Serial) {
      Serial.print(F("WARNING: Humidity value = "));
      Serial.print(humidity, 2);
      Serial.print(F(", must be in "));
      Serial.print(HUMIDITY_WARN_LOW);
      Serial.print(F(".."));
      Serial.print(HUMIDITY_WARN_HIGH);
      Serial.println(F("."));
    }
    Status::set(STATUS_WARNING);
  }
}

void Limits::validate(uint16_t co2) {
  if (co2 > CO2_CRIT_HIGH) {
    if (Serial) {
      Serial.print(F("DANGEROUS: CO2 value = "));
      Serial.print(co2, DEC);
      Serial.print(F(", must be <= "));
      Serial.print(CO2_CRIT_HIGH);
      Serial.print(F("."));
    }
    Status::set(STATUS_DANGEROUS);
  } else if (co2 > CO2_WARN_HIGH) {
    if (Serial) {
      Serial.print(F("WARNING: CO2 value = "));
      Serial.print(co2, DEC);
      Serial.print(F(", must be <= "));
      Serial.print(CO2_WARN_HIGH);
      Serial.print(F("."));
    }
    Status::set(STATUS_WARNING);
  }
}

void Limits::validate(float temperature, float humidity, uint16_t co2) {
  validate(temperature, humidity);
  validate(co2);
}
