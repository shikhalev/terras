#include "screen.h"

static Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

static bool screen_initialized = false;

void Screen::begin() {
  tft.init(240, 240, SPI_MODE2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 100);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println(F("Initialization..."));
  screen_initialized = true;
}

#define LINE_HEIGHT 25

void Screen::tick() {
  if (!screen_initialized)
    return;
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  byte top = 20;
  for (byte channel = 0; channel < 8; ++channel) {
    Data item = Sensors::getData(channel);
    if (item.flags == 0) 
      continue;
    tft.setCursor(0, top);
    tft.setTextColor(ST77XX_CYAN);
    tft.print(channel);
    tft.print(F(": "));

    if ((item.flags & DATA_SHT2X) != 0) {
      float t = item.sht20.data.temperature;
      float h = item.sht20.data.humidity;
      switch (Limits::tz(t))
      {
      case ZONE_DANGEROUS:
        tft.setTextColor(ST77XX_RED);
        break;
      case ZONE_WARNING:
        tft.setTextColor(ST77XX_YELLOW);
        break;
      default:
        tft.setTextColor(ST77XX_GREEN);
        break;
      }
      tft.print(t, 0);
      tft.print(F("'C "));
      switch (Limits::hz(h))
      {
      case ZONE_DANGEROUS:
        tft.setTextColor(ST77XX_RED);
        break;
      case ZONE_WARNING:
        tft.setTextColor(ST77XX_YELLOW);
        break;
      default:
        tft.setTextColor(ST77XX_GREEN);
        break;
      }
      tft.print(h, 0);
      tft.print(F("% "));
    } else if ((item.flags & ERROR_SHT2X) != 0) {
      tft.setTextColor(ST77XX_RED);
      tft.print(F(" Err Err ")); // TODO: номер ошибки
    } else {
      tft.print(F("         "));
    }

    if ((item.flags & DATA_SGP30) != 0) {
      uint16_t c = item.sgp30.co2;
      switch (Limits::cz(c))
      {
      case ZONE_DANGEROUS:
        tft.setTextColor(ST77XX_RED);
        break;
      case ZONE_WARNING:
        tft.setTextColor(ST77XX_YELLOW);
        break;
      default:
        tft.setTextColor(ST77XX_GREEN);
        break;
      }
      tft.print(c);
      tft.print(F("ppm"));
    } else if ((item.flags & ERROR_SGP30) != 0) {
      tft.setTextColor(ST77XX_RED);
      tft.print(F("Err"));
    }

    // TODO: вывод данных с раскраской

    top += LINE_HEIGHT;
  }
}
