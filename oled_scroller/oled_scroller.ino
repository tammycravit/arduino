//////////////////////////////////////////////////////////////////////////////
//
// Reference for scrolling:  https://forums.adafruit.com/viewtopic.php?f=47&t=48762
//////////////////////////////////////////////////////////////////////////////

//****************************************************************************
//****************************************************************************

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// For Adafruit SSD1331 graphics library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

// Pin definitions
#define ISR_SENSE_PIN     2
#define CS_PIN    10
#define DC_PIN    8
#define RST_PIN    9

// Color definitions
#define RGB(R,G,B)      (((R>>3)<<11) | ((G>>2)<<5) | (B>>3))

#define BLACK           RGB(0,   0,   0)
#define BLUE            RGB(0,   0,   255)
#define RED             RGB(255, 0,   0)
#define GREEN           RGB(0,   255, 0)
#define CYAN            RGB(0,   255, 255)
#define MAGENTA         RGB(255, 0,   0)
#define YELLOW          RGB(0,   0,   255)
#define WHITE           RGB(255, 255, 255)

#define HUFFLEPUFF_FG  RGB(238, 225, 119)
#define HUFFLEPUFF_BG  RGB(0,   0,   0)

#define RAVENCLAW_FG   RGB(0,   10,  144)
#define RAVENCLAW_BG   RGB(148, 107, 45)

#define GRYFFINDOR_FG  RGB(127, 9l,  9)
#define GRYFFINDOR_BG  RGB(255, 197, 0)

#define SLYTHERIN_FG   RGB(13,  98,  23)
#define SLYTHERIN_BG   RGB(170, 170, 170)

//****************************************************************************
//****************************************************************************

// Current message state type
typedef enum { Blank, UpToNoGood, MischiefManaged, Hello} BadgeState;
typedef enum { Hufflepuff, Ravenclaw, Slytherin, Gryffindor } WizardHouse;

// Constants for the messages
const char *UpToNoGoodStr      = F("I solemnly swear I am up to no good.");
const char *MischiefManagedStr = F("Mischief managed.");
const char *HelloMessageStr    = F("Greetings, young wizard.");

// Variable definitions
Adafruit_SSD1331  display = Adafruit_SSD1331(CS_PIN, DC_PIN, RST_PIN);
BadgeState        currentState = UpToNoGood;
int               xpos;
int               minX;
volatile int      isrSenseValue;
volatile int      loopCounter;
char *            message;
WizardHouse       myHouse;

//****************************************************************************
//****************************************************************************

inline UInt16_t rgb(UInt8 red, UInt8 green, UInt8 blue) {
  UInt16_t r = (((red>>3)<<11) | ((green>>2)<<5) | (blue>>3));
  return(r);
}

// Interrupt service routine for the button
ISR(INT0_vect) {
  isrSenseValue = digitalRead(ISR_SENSE_PIN);
}

// Respond to a press of the button
void handleButtonPress() {
  switch (currentState)
    case UpToNoGood:
      currentState = MischiefManaged;
      resetDrawState();
      break;
    case MischiefManaged:
      currentState = Blank;
      resetDrawState();
      break;
    case Hello:
      currentState = UpToNoGood;
      resetDrawState();
      break;
    case Blank:
      currentState = Hello;
      resetDrawState();
      break;
    default:
      return;
      break;
  }
}

UInt16_t houseColorFG(void) {
  switch (myHouse) {
    case Hufflepuff:
      return HUFFLEPUFF_FG;
      break;
    case Slytherin:
      return SLYTHERIN_FG;
      break;
    case Ravenclaw:
      return RAVENCLAW_FG;
      break;
    case Gryffindor:
      return GRYFFINDOR_FG;
      break;
    default:
      return WHITE;
      break;
  }
}

UInt16_t houseColorBG(void) {
  switch (myHouse) {
    case Hufflepuff:
      return HUFFLEPUFF_BG;
      break;
    case Slytherin:
      return SLYTHERIN_BG;
      break;
    case Ravenclaw:
      return RAVENCLAW_BG;
      break;
    case Gryffindor:
      return GRYFFINDOR_BG;
      break;
    default:
      return BLACK;
      break;
  }
}

void resetDrawState(void) {

  display.clearDisplay();
  display.fillScreen(BLACK);

  switch (currentState)
  {
    case UpToNoGood:
      message = UpToNoGoodStr;
      display.setTextColor(houseColorFG());
      break;
    case MischiefManaged:
      message = MischiefManagedStr;
      display.setTextColor(RED);
      break;
    case Hello:
      message = HelloMessageStr;
      display.setTextColor(WHITE);
      break;
    default:
      return;
      break;
  }

  display.setCursor(xPos, 20);
  display.setTextSize(2);
  display.setTextWrap(false);
  xpos = display.width();
  minX = -12 * strlen(message);   // Adjust if you change the font size
}

//****************************************************************************
//****************************************************************************

// Setup the badge
void setup(void) {

  myHouse = Hufflepuff;

  // Enable interrupt sensing for the pushbutton
  pinMode(ISR_SENSE_PIN, INPUT);
  GICR |= ( 1 < < INT0 );
  MCUCR |= ( 1 << ISC00 );
  MCUCR |= ( 0 << ISC01 );

  // Initialize the OLED display
  display.begin();
  for (int i=0; i <= 255; i++) {
    display.fillScreen(rgb(i, i, i));
    delay(20);
  }
  display.clearDisplay();
  display.fillScreen(BLACK);

  currentState = Hello;
  resetDrawState();
}

//****************************************************************************
//****************************************************************************

// Event loop
void loop(void) {
  // Check if we caught an interrupt and change state
  if (HIGH == isrSenseValue) {
    handleButtonPress();
    isrSenseValue = LOW;
  }

  display.clearDisplay();
  display.setCursor(xPos, 20);
  display.print(message);
  display.display();

  if(--xpos < minX) xpos = display.width();

  delay(25);
}
