#ifndef __EXPLORER__
#define __EXPLORER__

#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#include "FileInfoHolder.h"

#ifndef FILES_PER_PAGE
#define FILES_PER_PAGE 10
#endif

class Explorer
{
protected:
    Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9); // temp that is overwritten in the constructor

    uint16_t tftWidth = 320;
    uint16_t tftHeight = 240;

public:
    uint16_t backgroundColour = ILI9341_BLACK;
    uint16_t upperColour = ILI9341_DARKGREY;
    uint16_t textColour = ILI9341_WHITE;
    uint8_t screenPaddingX = 5;
    uint8_t itemHeight = 25;
    uint8_t fontSize = 1;
    uint8_t buttonSize = 32;

protected:
    uint16_t pageSize = FILES_PER_PAGE;
    String currentPath;
    uint16_t currentPage = 0;
    File currentFolder;
    FileInfoHolder fileList[FILES_PER_PAGE];
    bool pageHasFiles = false;

public:
    Explorer(String path = "");
    void begin(Adafruit_ILI9341 screen, uint8_t rotation = 0);

    void upDirectory();
    void openDirectory(String path);

    bool loadInFolder();

    void changePage(int direction);

    uint16_t getStrHeight(String *c);
    uint16_t getStrWidth(String *c);

    void hideCurrentFiles();

    void drawItem(uint8_t itemIndex, FileInfoHolder file);

    void renderCurrentDirectory(bool clearScreen = true);

    FileInfoHolder checkForTouch(TS_Point hit);
};


#endif