/* ***
   Читаем датчики, пишем на экран и в Serial тож.  
   *** */

#include "sensors.h"
#include "timer.h"

void setup() {
  Serial.begin(9600);

  Status::begin();
  Sensors::begin();

  Sensors::reset();
  
  setup_timer();

  if (Serial) {
    Serial.println(F("Initialized"));
    // TODO: обработать вероятные ошибки
  }
}

void loop() {
  Sensors::tick();
  delay(2000);
}

ISR(TIMER1_COMPA_vect) {
  static byte counter = 0;
  if (++counter % 8 == 0)
    Status::tick();
}