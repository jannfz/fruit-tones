#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include "fruit_keyboard.h"

TFT_eSPI tft = TFT_eSPI();

const int cw = tft.width()/2;
const int ch = tft.height()/2;
const int s = min(cw/4,ch/4);

void setupDisplay(void) {  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
}

void displayFruit(const Note_t *fruit) {
    tft.pushImage(60,60, 120, 120, fruit->bitmap);
}

