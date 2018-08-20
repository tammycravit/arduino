#include <Arduino.h>
#include "ucglib.h"

#define DISPLAY_SCLK 13
#define DISPLAY_DATA 11
#define DISPLAY_CD   10
#define DISPLAY_CS   9
#define DISPLAY_RESET 8

typedef enum { Blank, UpToNoGood, MischiefManaged } BadgeState;
Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(DISPLAY_SCLK, DISPLAY_DATA, DISPLAY_CD, DISPLAY_CS, DISPLAY_RESET);
BadgeState currentState = UpToNoGood;
int xpos;
int ypos;

const char *UpToNoGoodStr = F("I solemnly swear I am up to no good.");
const char *MischiefManagedStr = F("Mischief managed.");

void draw (void) {
  switch (currentState) {
    case UpToNoGood:
      ucg.drawStr(xpos, 36, UpToNoGoodStr);
      break;
    case MischiefManaged:
      ucg.drawStr(xpos, 36, MischiefManagedStr);
      break;
  }
}

void setup(void) {
  delay(1000);
  ucg.begin();
  ucg.clearScreen();
  ucg.setFont(ucg_font_ncenB18r);
  ucg.setColor(255, 255, 255);
}

void loop(void) {
  ucg.firstPage();
  do {
    draw();
  }
  while (ucg.netPage());
  if (++xpos >= 128) {
    xpos = 0;
  }
  delay(100);
}
