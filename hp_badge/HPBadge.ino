#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SD.h>
#include <SPI.h>

#include "Streaming.h"

#define TFT_CS  10            // Chip select line for TFT display
#define TFT_RST  9            // Reset line for TFT (or see below...)
                              //Use this reset pin for the shield!
//#define TFT_RST  -1         // you can also connect this to the Arduino reset!
#define TFT_DC   8            // Data/command line for TFT
#define SD_CS    4            // Chip select line for SD card

#define changeStatePin  2
#define blankScreenPin  3

// Color definitions
#define RGB(R,G,B)      (((R>>3)<<11) | ((G>>2)<<5) | (B>>3))
#define BLACK           RGB(0,0,0)
#define BLUE            RGB(0,0,255)
#define RED             RGB(255,0,0)
#define GREEN           RGB(0,255,0)
#define CYAN            RGB(0,255,255)
#define MAGENTA         RGB(255,0,255)
#define YELLOW          RGB(255,255,0)
#define WHITE           RGB(255,255,255)
#define HUFFLEPUFF_BG   RGB(238,225,119)

// Function prototypes
void changeStateButton_ISR(void);
void blankScreenButton_ISR(void);
void updateDisplay(void);
void bmpDraw(char *filename, uint8_t x, uint16_t y);
uint16_t read16(File f);
uint32_t read32(File f);

typedef enum {
  Greeting,
  MarauderMap,
  MischiefManaged,
  BlankScreen
} MapState;

Adafruit_ST7735  tft                    = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
MapState         currentState           = BlankScreen;
MapState         previousState          = BlankScreen;
bool             blankScreen            = false;

uint8_t          changeStateButtonState = 0;
uint8_t          blankScreenButtonState = 0;
boolean          displayStateChanged    = false;

void changeStateButton_ISR(void) {
  uint8_t currState = digitalRead(changeStatePin);
  if ((currState == HIGH) && (changeStateButtonState == LOW)) {
    previousState = currentState;
    switch (currentState)
    {
      case BlankScreen:
        currentState = Greeting;
        break;
      case Greeting:
        currentState = MarauderMap;
        break;
      case MarauderMap:
        currentState = MischiefManaged;
        break;
      case MischiefManaged:
        currentState = Greeting;
        break;
      default:
        currentState = Greeting;
        break;
    }
    displayStateChanged = true;
  }
  changeStateButtonState = currState;
}

void blankScreenButton_ISR(void) {
  uint8_t currState = digitalRead(blankScreenPin);
  if ((currState == HIGH) && (blankScreenButtonState == LOW)) {
    if (currentState == BlankScreen) {
      currentState = previousState;
      previousState = BlankScreen;
    }
    else {
      previousState = currentState;
      currentState  = BlankScreen;
    }
    displayStateChanged = true;
  }
  blankScreenButtonState = currState;
}

void updateDisplay(void) {
  if (false == displayStateChanged)
    return;

  Serial << F("updateDisplay()") << endl;
  Serial << F("Display state was ") << previousState << F(", now ") << currentState << endl;

  switch (currentState)
  {
      case BlankScreen:
        tft.fillScreen(HUFFLEPUFF_BG);
        break;
      case Greeting:
        bmpDraw("greeting.bmp", 0, 0);
        break;
      case MarauderMap:
        bmpDraw("map.bmp", 0, 0);
        break;
      case MischiefManaged:
        bmpDraw("managed.bmp", 0, 0);
        break;
      default:
        bmpDraw("greeting.bmp", 0, 0);
        break;
  }
}

void setup(void) {

  Serial.begin(9600);
  while(!Serial) {
    delay(10);
  }

  Serial << F("setup(): initializing") << endl;

  pinMode(changeStatePin, INPUT);
  pinMode(blankScreenPin,  INPUT);

  attachInterrupt(changeStatePin, changeStateButton_ISR, CHANGE);
  attachInterrupt(blankScreenPin,  blankScreenButton_ISR, CHANGE);

  Serial << F("setup(): init display pins") << endl;
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(HUFFLEPUFF_BG);

  Serial << F("setup(): init SD card") << endl;
  if (!SD.begin(SD_CS)) {
    Serial << F("setup(): init SD card FAILED") << endl;
    return;
  }
  delay(200);

  currentState = Greeting;
  updateDisplay();
}

void loop(void) {
  Serial << F("loop()") << endl;
  updateDisplay();
  delay(250);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial << F("bmpDraw(): Loading ") << filename << endl;

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial << F("bmpDraw(): File not found") << endl;
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial << F("File size: ") << read32(bmpFile) << endl;
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial << F("Image offset: ") << bmpImageoffset << endl;
    Serial << F("Header size: ") << read32(bmpFile) << endl;

    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial << F("Bit depth: ") << bmpDepth << endl;
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial << F("Image size: ") << bmpWidth << 'x' << bmpHeight << endl;

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial << F("Bitmap loaded successfully ") << endl;
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) {
    Serial << F("BMP format not recognized") << endl;
  }
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
