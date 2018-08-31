/*
	ARDUINO PUNK CONSOLE made with an Attiny85

	The sketch is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public	License as published by the Free Software
	Foundation; either version 3.0 of the License, or (at your option) any later
	version.
*/

//pins declaration
volatile byte starterTCNT0 = 0;
volatile byte starterOCR0A = 0;
volatile byte prescaler = 3;
unsigned int activations;
char dir = 1;


//initial setups
void setup() {
    DDRB = 0b00000001; // pin: D0=OUTPUT - D3=INPUT
    setupTimer();
}


//main loop
void loop() {
    int tempDelay = analogRead(1) * 10; //a pot is used to set the period
    if (tempDelay == 0) {
        tempDelay = 1;
    }
    delayMicroseconds(tempDelay);
    starterTCNT0 = analogRead(2) / 4; //the other one changes the frequency
    if (starterTCNT0 == 0) {
        starterTCNT0 = 1;
    }
    starterOCR0A += dir;
    //trying to avoid some particular cases that put the output off for a cycle
    if (starterOCR0A <= starterTCNT0) {
        dir = 1;
        starterOCR0A = starterTCNT0 + 1;
    } else if (starterOCR0A == 0) {
        dir = 1;
        starterOCR0A = 1;
    } else if (starterOCR0A == 255) {
        dir = -1;
        starterOCR0A = 254;
    }

    OCR0A  = starterOCR0A; //set the new OCR value

    if (!digitalRead(3)) { //with the button I change the prescaler
        activations++; //debounce
        if (activations > 30) {
            if (++prescaler > 5) {
                prescaler = 3;
            }
            cli();
            setupTimer(); //update the timer
            sei();
            activations = 0;
        }
    }
}


//setup of the timer
void setupTimer() {
    cli();
    TIMSK = 0;
    TCCR0A = ((1 << COM0A1) | (1 << WGM01) | (1 << WGM00)); //fastPWM with OCR0A - not inverting
    TCCR0B = prescaler; //prescaler: 3=64 / 4=256 / 5=1024
    TCNT0 = starterTCNT0;
    OCR0A = starterOCR0A;
    TIMSK = (1 << TOIE0); //interrupt at the overflow of the counter
    sei();
}


//interrupt service routine
ISR(TIMER0_OVF_vect) {
    TCNT0 = starterTCNT0; //restore the initial value of the counter (sets the frequency)
}


