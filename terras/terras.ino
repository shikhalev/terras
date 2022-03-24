/* ***
   Читаем датчики, пишем на экран и в Serial тож.  
   *** */

// TODO: переупорядочить и сделать два режима вывода в сериал — табличный и отладочный
// TODO: отключение экрана в экономичном режиме (требует управления)

#include "sensors.h"
#include "timer.h"
#include "screen.h"

#define CHECK_DELAY 10000

void setup() {
  Serial.begin(9600);

  Screen::begin();
  Status::begin();
  Sensors::begin();
  Sensors::reset();
 
  setup_timer();

  Screen::tick();
}

void loop() {
  Sensors::tick();
  Screen::tick();
  delay(CHECK_DELAY);
}

ISR(TIMER1_COMPA_vect) {
  Status::tick();
}