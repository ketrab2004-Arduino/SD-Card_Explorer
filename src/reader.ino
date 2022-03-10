#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#include <Colouring.h>
#include <Explorer.h>

#define DEBUG 1
#include <Extra_Debug.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_BL 5
#define SD_CS  4

#define BRIGHTNESS  127

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
Adafruit_FT6206 ts;

Explorer explorer;

void setup() {
    debugBegin(115200);
    tft.begin();
    ts.begin();

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, BRIGHTNESS);

    if (SD.begin(SD_CS)) {
        explorer.begin(tft, 2);

        explorer.loadInFolder();
        explorer.renderCurrentDirectory();
    }
}

void loop() {
    // check for touch using ts
    if (! ts.touched()) {
        return;
    }

    // Retrieve a point  
    TS_Point p = ts.getPoint();
    
    FileInfoHolder file = explorer.checkForTouch(p);
}
