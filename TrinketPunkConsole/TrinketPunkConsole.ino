#include <Arduino.h>
#include <avr/io.h>

#include "trinket_pins.h"

#define FREQ_PIN      TRINKET_PB2     // GPIO #2 / PB2 / Analog Input A1
#define RATE_PIN      TRINKET_PB4     // GPIO #4 / PB4 / Analog Input A2
#define LED_PIN       TRINKET_PB1     // GPIO #1 / PB1 / Analog Output / LED
#define SPEAKER_PIN   TRINKET_PB0     // GPIO #0 / PB0 / I2c/SPI

