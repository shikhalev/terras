/* ***
   Читаем датчики, пишем на экран и в Serial тож.  
   *** */

#include "sensors.h"

void setup() {
  Serial.begin(9600);

  Status::begin();
  Sensors::begin();

  Sensors::scan();
  
  if (Serial) {
    Serial.println(F("Initialized"));
    // TODO: обработать вероятные ошибки
  }
}

void loop() {
  Sensors::tick();
}
