#include <Arduino.h>
#include "Bounce2.h"

//////////////////////////////////////////////////////////////////////////////
// Pin definitions
//////////////////////////////////////////////////////////////////////////////

#define LED_START_PIN 2
#define NUM_LEDS      8
#define SPEED_DN_BTN  11
#define SPEED_UP_BTN  12
#define REVERSE_BTN   13

//////////////////////////////////////////////////////////////////////////////
// Speed constants
//////////////////////////////////////////////////////////////////////////////

#define MS_PER_TICK            5
#define MIN_TICKS_PER_UPDATE   10
#define MAX_TICKS_PER_UPDATE   100
#define START_TICKS_PER_UPDATE 50
#define TICK_ADJUST_INCREMENT  10

//////////////////////////////////////////////////////////////////////////////
// Other constants
//////////////////////////////////////////////////////////////////////////////

#define DEBOUNCE_INTERVAL      5

//////////////////////////////////////////////////////////////////////////////
// An enum to track which direction the LED chaser is moving
//////////////////////////////////////////////////////////////////////////////

enum LEDDirection { DOWN, UP };

//////////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////////

uint8_t       currentSpeed     = START_TICKS_PER_UPDATE;
LEDDirection  currentDirection = UP;
uint8_t       currentLed       = 0;
uint8_t       tickCount        = 0;

Bounce        speedUpInput     = Bounce();
Bounce        speedDnInput     = Bounce();
Bounce        reverseInput     = Bounce();
int           speedDnBtnValue;
int           speedUpBtnValue;
int           reverseBtnValue;

//////////////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////////////

void updateLeds();
void pollButtons();
void ledLoop();

//////////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////////

//
// updateLeds(): Turn on the current LED and turn off the others
//

void updateLeds() {
  for (int i=LED_START_PIN; i <= LED_START_PIN + NUM_LEDS; i++) {
    digitalWrite(i, (currentLed == i ? HIGH : LOW));
  }
}

//
// pollButtons(): Poll, debounce and handle the speed and direction buttons
//

void pollButtons() {
  speedDnInput.update();
  speedUpInput.update();
  reverseInput.update();

  speedDnBtnValue = speedDnInput.read();
  speedUpBtnValue = speedUpInput.read();
  reverseBtnValue = reverseInput.read();

  if (speedDnBtnValue == LOW) {
    currentSpeed -= TICK_ADJUST_INCREMENT;
    if (currentSpeed < MIN_TICKS_PER_UPDATE) {
      currentSpeed = MIN_TICKS_PER_UPDATE;
    }
  }

  if (speedUpBtnValue == LOW) {
    currentSpeed += TICK_ADJUST_INCREMENT;
    if (currentSpeed > MAX_TICKS_PER_UPDATE) {
      currentSpeed = MAX_TICKS_PER_UPDATE;
    }
  }

  if (reverseBtnValue == LOW) {
    currentDirection = (currentDirection == DOWN ? UP : DOWN);
    ledLoop();
  }
}

//
// ledLoop(): Update the LED and current state/direction variables
//

void ledLoop() {
  if (tickCount == currentSpeed) {
    if ((currentLed == (NUM_LEDS-1)) && (currentDirection == UP)) {
      currentDirection = DOWN;
    }
    else if ((currentLed == 0) && (currentDirection == DOWN)) {
      currentDirection = UP;
    }
    currentLed = currentLed + (currentDirection == UP ? 1 : -1);
    updateLeds();
    tickCount = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Arduino sketch setup routine
//////////////////////////////////////////////////////////////////////////////

void setup() {
  for (int i=LED_START_PIN; i <= LED_START_PIN + NUM_LEDS; i++) {
    pinMode(i, OUTPUT);
  }

  pinMode(SPEED_DN_BTN, INPUT_PULLUP);
  speedDnInput.attach(SPEED_DN_BTN);
  speedDnInput.interval(DEBOUNCE_INTERVAL);

  pinMode(SPEED_UP_BTN, INPUT_PULLUP);
  speedUpInput.attach(SPEED_UP_BTN);
  speedUpInput.interval(DEBOUNCE_INTERVAL);

  pinMode(REVERSE_BTN,  INPUT_PULLUP);
  reverseInput.attach(REVERSE_BTN);
  reverseInput.interval(DEBOUNCE_INTERVAL);

  updateLeds();
}

//////////////////////////////////////////////////////////////////////////////
// Arduino main event loop
//////////////////////////////////////////////////////////////////////////////

void loop() {
  tickCount++;

  pollButtons();
  ledLoop();

  delay(MS_PER_TICK);
}
