/*
 * Pin definitions for the Adafruit Trinket
 *
 * ATtiny85 Pinout:
 *
 *           +-----+
 * PB5 1  ---|*    |--- 8  Vcc
 * PB3 2  ---|     |--- 7  PB2
 * PB4 3  ---|     |--- 6  PB1
 * GND 4  ---|     |--- 5  PB0
 *           +-----+
 *
 * Trinket Pins:
 *
 *       +-------+
 * BAT+  |o     o| USB+
 * GND   |o     o| #0
 * #4    |o     o| #1
 * #3    |o     o| #2
 * RST   |o     o| 5V
 *       +-------+
 *
 * Pin Descriptions:
 *
 * Pin    ATtiny Pin    Usage(s)
 * ---    ----------    --------
 * BAT+      --         Vcc for circuits (Trinket voltage up to 16V)
 * GND     4 (GND)      Common ground
 * #4      3 (PB4)      Analog input A2, PWM output, digital output, USB
 *                      programming
 * #3      2 (PB3)      Analog input A3, digital output, USB programming (pullup
 *                      resistor on 3.3V version)
 * RST     1 (PB5)      ATtiny Reset
 * 5V        --         5V/3.3V (depending on Trinket version) @ 150ma
 * #2      7 (PB2)      Analog output, digital input, digital output,
 *                      analog input A1, SPI/I2C clock
 * #1      6 (PB1)      Analog output, digital input, digital output,
 *                      SPI output, LED
 * #0      5 (PB0)      Analog output, digital input, digital output, I2C data,
 *                      SPI Input
 * USB+      --         5V @ 500 mA when connected to USB
 */

#ifndef __HAS_TRINKET_PINS_H__
#define __HAS_TRINKET_PINS_H__

/* Define the actual hardware pins on the Trinket, so we can map them to functions */
#define TRINKET_HW_BAT+    1
#define TRINKET_HW_GND     2
#define TRINKET_HW_IO4     3
#define TRINKET_HW_IO3     4
#define TRINKET_HW_RST     5
#define TRINKET_HW_5V      6
#define TRINKET_HW_IO2     7
#define TRINKET_HW_IO1     8
#define TRINKET_HW_IO0     9
#define TRINKET_HW_USB+    10

#define TRINKET_ATTINY_PB5 1
#define TRINKET_ATTINY_PB3 2
#define TRINKET_ATTINY_PB4 3
#define TRINKET_ATTINY_GND 4
#define TRINKET_ATTINY_PB0 5
#define TRINKET_ATTINY_PB1 6
#define TRINKET_ATTINY_PB2 7
#define TRINKET_ATTINY_VCC 8

#define TRINKET_PB0        TRINKET_HW_IO0
#define TRINKET_PB1        TRINKET_HW_IO1
#define TRINKET_PB2        TRINKET_HW_IO2
#define TRINKET_PB3        TRINKET_HW_IO3
#define TRINKET_PB4        TRINKET_HW_IO4

#define TRINKET_ANALOG_A1  TRINKET_PB2
#define TRINKET_ANALOG_A2  TRINKET_PB4
#define TRINKET_ANALOG_A3  TRINKET_PB3

#endif /* __HAS_TRINKET_PINS_H__ */
