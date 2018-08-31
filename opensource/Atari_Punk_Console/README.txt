********************************************
*   A T A R I    P U N K    C O N S O L E  *
********************************************


* Description
********************************************
The ATARI PUNK CONSOLE (APC) is a popular circuit made using two 555 timer
ICs. The original circuit, called a "Stepped Tone Generator", was realized
by Forrest M. Mims III and first published in 1980 in a Radio Shack booklet
entitled "Engineer's Notebook: Integrated Circuit Applications". It was
named "Atari Punk Console" by Kaustic Machines because its noise resembled
the sounds of classic Atari console games from the 80s, with a square wave
output very similar to those of the Atari 2600.

This implementation is done using an ATtiny85 microcontroller, but you can
use an ATtiny25 t oo,because the firmware is only 820 bytes in size. It
should be adaptable to other Atmel MCUs, just look for proper register
names into the datasheet of the microcontroller you want to use.

The circuit uses 2 potenziometers to adjust the frequency and the period
of the sound. The frequency is set using an internal timer while the period
is set modifying the delay used to increment/decrement the counter of the
timer (pulsing effect). An LED shows what signal is coming to the speaker.
The button is used to scroll the prescaler between 3 differents values so
that the noise has 3 different frequency ranges.

The circuit can be powered directly to the jack with a regulated 5V voltage
(at least 500 mA of current is needed) or by 4 AAA NiMH rechargeble batteries
(the Schottky diode prevents polarity inversions and decrease the voltage
input: 4 rechargeble batteris at full charge could provide up to 1.4*4=5.6V
so that the input voltage can drop of 0.2/0.3V and stays under 5.5V that
is the maximum allowed voltage for the MCU).


* Circuit
********************************************
The circuit is very simple and uses only through-hole components so that
anyone can replicate it. I realized it on a protoboard. The potenziometers
have a value of 100Kohm and are linear taper potenziometers. The speaker is 
a common 8 ohm/0.5W speaker: mine is more than 30 years old and it's still 
working greatly ;-)


* Sketch
********************************************
The sketch has been written with Arduino IDE 1.0.5 and compiled using the
Atmel toolchain 3.4.2 but it should be compilable with the toolchain included
into the Arduino IDE. To compile the sketch you also need the Tiny core
available here:
http://code.google.com/p/arduino-tiny

At the time of this document the most recent version of the core is the
0100-0017.


* License
********************************************
All the files of this project (schematic and Eagle files, readme file) are
released under the therms of the Creative Commons Attribution-Share Alike-
-Not Commercial (BY-SA-NC) 3.0.

The sketch is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public	License as published by the Free Software
Foundation; either version 3.0 of the License, or (at your option) any later
version.


* Warranties
********************************************
This project is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.
Any consequences derived by the use of this project can not be attributed
to its author in any way.


* Document revision
********************************************
Last revision: 2013/07/07

