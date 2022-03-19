/* ***
   Читаем датчики, пишем на экран и в Serial тож.  
   *** */

// TODO: переупорядочить и сделать два режима вывода в сериал — табличный и отладочный
// TODO: отключение экрана в экономичном режиме (требует управления)

#include "sensors.h"
#include "timer.h"
#include "screen.h"

void setup() {
  Serial.begin(9600);

  Screen::begin();
  Status::begin();
  Sensors::begin();

  Sensors::reset();
  
  setup_timer();
}

void loop() {
  Sensors::tick();
  delay(2000);
}

ISR(TIMER1_COMPA_vect) {
  static byte counter = 0;
  ++counter;
  Status::tick();
  if (counter % 20 == 0)
    Screen::tick();
}