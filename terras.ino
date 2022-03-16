/* ***
   Читаем датчики, пишем на экран и в Serial тож.  
   *** */

#include "sensors.h"

Sensors sensors;

void setup() {
  Serial.begin(9600);

  sensors.begin();
  Status::begin();
  
  if (Serial) {
    Serial.println("Initialized");
    // TODO: обработать вероятные ошибки
  }
}

void loop() {
  sensors.tick();
}
