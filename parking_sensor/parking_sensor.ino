/*
 * Arduino Parking Sensor
 *
 *----------------
 * Pin connections
 *----------------
 *
 * Digital 10 : Ultrasonic Sensor - Trigger
 * Digital 11 : Ultrasonic Sensor - Echo
 * Digital 2  : LCD Display - RS
 * Digital 3  : LCD Display - Enable
 * Digital 4  : LCD Display - D4
 * Digital 5  : LCD Display - D5
 * Digital 6  : LCD Display - D6
 * Digital 7  : LCD Display - D7
 * Digital 9  : Red LED (distance warning)
 */

#include <LiquidCrystal.h>
#include "Streaming.h"

//============================================================================
// Pin Definitions
//============================================================================

#define SENSOR_TRIG_PIN     10
#define SENSOR_ECHO_PIN     11
#define LCD_RS_PIN          2
#define LCD_ENABLE_PIN      3
#define LCD_D4_PIN          4
#define LCD_D5_PIN          5
#define LCD_D6_PIN          6
#define LCD_D7_PIN          7
#define LED_PIN             9

#define NUM_SAMPLES         3
#define WARNING_DISTANCE_IN 12

//============================================================================
// Global Variables
//============================================================================

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN,
    LCD_D6_PIN, LCD_D7_PIN);

//============================================================================
// Function Prototyoes
//============================================================================

uint16_t measurePulse();
uint16_t measureDistance(void);
uint16_t measureDistanceOne(void);
void updateLCD(uint16_t distance_inches);
void updateWarningLight(uint16_t distance_inches);

//============================================================================
// Implementation Functions
//============================================================================

uint16_t measureDistanceOne(void) {
  uint16_t duration;
  int      distance;

  // Clear the trigger pin
  digitalWrite(SENSOR_TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a measurement pulse
  digitalWrite(SENSOR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_TRIG_PIN, LOW);

  duration = pulseIn(SENSOR_ECHO_PIN, HIGH);
}

uint16_t measureDistance(void) {
  uint16_t measureDuration = 0;
  uint16_t averageDuration = 0;
  uint8_t  i;

  Serial << F("measureDistance(): Taking ") << NUM_SAMPLES << " samples" << endl;

  for (i=0; i < NUM_SAMPLES; i++) {
    measureDuration = measureDistanceOne();
    if (i == 0) {
      averageDuration = measureDuration;
    }
    else {
      averageDuration = ((averageDuration + measureDuration) / 2);
    }
    Serial << F("   Sample ") << i;
    Serial << F(", duration=") << measureDuration;
    Serial << F(", avg now=") << averageDuration << endl;

    delay(5);
  }

  Serial << "Calculated distance: " << (averageDuration/0.0133/2) << "in" << endl;
  // Distance in inches; if you want cm instead, change 0.0133 to 0.034
  return (averageDuration*0.0133/2);
}

void updateLCD(uint16_t distance_inches) {
  uint8_t spaces;
  char    indicator = ' ';
  uint8_t i;

  if (distance_inches > 48) {
    spaces    = 12;
    indicator = '*';
  }
  else {
    spaces = distance_inches / 4;
    if (spaces < (WARNING_DISTANCE_IN/4)) {
      indicator = '!';
    }
    else {
      indicator = '+';
    }
  }

  Serial << F("updateLCD: spaces=") << spaces;
  Serial << F("; indicator=") << indicator << endl;

  lcd.setCursor(0,0);
  lcd.print("Dist: ");
  lcd.print(distance_inches);
  lcd.print(" in");
  delay(10);
  lcd.setCursor(0,1);
  lcd.print("| ");
  for (i=0; i<spaces; i++) {
    lcd.print(" ");
  }
  lcd.print(indicator);
}

void updateWarningLight(uint16_t distance_inches) {
  if (distance_inches < WARNING_DISTANCE_IN) {
    Serial << F("updateWarningLight(): Distance < ") << WARNING_DISTANCE_IN;
    Serial << F("; warning light active") << endl;
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
}

//============================================================================
// Arduino Setup Function
//============================================================================

void setup(void) {
  Serial.begin(9600);

  pinMode(SENSOR_TRIG_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.begin(16,2);

  Serial << F("setup(): done") << endl;
}

//============================================================================
// Arduino Event Loop
//============================================================================

void loop(void) {
  uint16_t distanceMeasurement;

  distanceMeasurement = measureDistance();
  Serial << F("loop(): measured: ") << distanceMeasurement << " in" << endl;

  updateLCD(distanceMeasurement);
  updateWarningLight(distanceMeasurement);

  if (distanceMeasurement > 48) {
    Serial << F("loop(): long delay") << endl;
    delay(500);
  }
  else {
    Serial << F("loop(): short delay") << endl;
    delay(100);
  }
}
