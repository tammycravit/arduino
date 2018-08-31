/*
 * Morse code flasher for the ATTiny65
 */

#include <avr/pgmspace.h>

#define DOT 75           // Duration of a dot
#define DASH 375         // Duration of a dash
#define INNER_PAUSE 125  // Duration of pause between dot and dash in a character 
#define CHAR_BREAK 250   // Duration of pause between characters
#define WORD_BREAK 375   // Duration of pause between words (spaces, punctuation, etc)

#define PIN               0
#define NUM_MESSAGES      4

uint8_t whichMessage;

const char messageTable[NUM_MESSAGES][] PROGMEN = {
  "Cuddle dungeon is the best dungeon",
  "No, Lyle, that's not an alpaca in that bag, don't be silly",
  "And suddenly...Pip!",
  "Emergency blanket fort deployed"
};

void dash() {
  digitalWrite(PIN, HIGH);
  delay(DOT);
  digitalWrite(PIN, LOW);
  delay(INNER_PAUSE);
}

void dot() {
  digitalWrite(PIN, HIGH);
  delay(DASH);
  digitalWrite(PIN, LOW);
  delay(INNER_PAUSE);
}

void sendMessage(char *message) {
  for(char* it = message; *it; ++it) {
    char c = toupper(*it);
    switch(c) {
      case 'A': dot(); dash(); break;
      case 'B': dash(); dot(); dot(); dot(); break;
      case 'C': dash(); dot(); dash(); dot(); break;
      case 'D': dash(); dot(); dot(); break;
      case 'E': dot(); break;
      case 'F': dot(); dot(); dash(); dot(); break;
      case 'G': dash(); dash(); dot(); break;
      case 'H': dot(); dot(); dot(); dot(); break;
      case 'I': dot(); dot(); break;
      case 'J': dot(); dash(); dash(); dash(); break;
      case 'K': dash(); dot(); dash(); break;
      case 'L': dot(); dash(); dot(); dot(); break;
      case 'M': dash(); dash(); break;
      case 'N': dash(); dot(); break;
      case 'O': dash(); dash(); dash(); break;
      case 'P': dot(); dash(); dash(); dot(); break;
      case 'Q': dash(); dash(); dot(); dash(); break;
      case 'R': dot(); dash(); dot(); break;
      case 'S': dot(); dot(); dot(); break;
      case 'T': dash(); break;
      case 'U': dot(); dot(); dash(); break;
      case 'V': dot(); dot(); dot(); dash(); break;
      case 'W': dot(); dash(); dash(); break;
      case 'X': dash(); dot(); dot(); dash(); break;
      case 'Y': dash(); dash(); dot(); dash(); break;
      case 'Z': dash(); dash(); dot(); dot(); break;
      // Todo: Numbers/punctuation
      default:
        delay(WORD_BREAK);
        break;
    }
    delay(CHAR_BREAK);
  }
}

void setup() {
  pinMode(PIN, OUTPUT);
  whichMessage = 0;
}

void loop() {
  sendMessage(messageTable[whichMessage]);
  delay(WORD_BREAK*3);
  whichMessage++;
  if (whichMessge > NUM_MESSAGES) {
    whichMessage = 0;
  }
}

