#include "Explorer.h"

Explorer::Explorer(String path = "") {
    currentPath = path;
}
void Explorer::begin(Adafruit_ILI9341 screen, uint8_t rotation = 0) {
    tft = screen;
    tft.setRotation(rotation);

    if (rotation % 2 == 0) { // switch width and height when screen is not in landscape mode
        uint16_t temp = tftWidth;
        tftWidth = tftHeight;
        tftHeight = temp;
    }

    currentFolder = SD.open(currentPath.c_str());
}

void Explorer::upDirectory() {
    uint16_t slashPos = 0-1;

    for (int i = currentPath.length(); i >= 0; i--) {
        if (currentPath.charAt(i) == '/') {
            slashPos = i;
            break;
        }
    }

    openDirectory( currentPath.substring(0, slashPos) );
}
void Explorer::openDirectory(String path) {
    currentPath = path;

    hideCurrentFiles();
    currentFolder.close();
    currentFolder = SD.open(path.c_str());

    currentPage = 0;

    loadInFolder();
    renderCurrentDirectory();
}

bool Explorer::loadInFolder() {
    currentFolder.rewindDirectory(); // rewind so openNextFile starts at the beginning again

    for (int i = 0; i < pageSize; i++) {
        fileList[i] = FileInfoHolder(); // replace all files in list with empty null files
    }

    // skip through files from previous pages
    for (int i = 0; i < pageSize * currentPage; i++) {
        File f = currentFolder.openNextFile();
        if (! f) { // if file doesnt exist
            // return false
            // signifying that this page doesn't exist
            f.close();
            pageHasFiles = false;
            return false;
        }
        f.close();
    }

    for (int i = 0; i < pageSize; i++) {
        File entry = currentFolder.openNextFile();

        if (! entry) { // file doesnt exist
            entry.close();
            if (i == 0) { // index is still 0, so there were no files
                pageHasFiles = false;
                return false; // so return false
            }
            break; // else just break out of the loop
        }
        fileList[i] = FileInfoHolder(entry, currentPath);
        entry.close();
    }
    pageHasFiles = true;
    return true; // return true because there were files on this page
}

void Explorer::changePage(int direction) {
    int16_t desiredPage = currentPage + direction;

    Serial.print("Direction: ");
    Serial.println(direction);

    Serial.print("Desired page: ");
    Serial.println(desiredPage);

    if (desiredPage >= 0 && (pageHasFiles || direction < 0)) {
        currentPage = desiredPage;

        // fill area with files, to signify loading
        hideCurrentFiles();

        loadInFolder();
        renderCurrentDirectory();
    }

    Serial.print("Result page: ");
    Serial.println(currentPage);

    Serial.print("Page ");
    Serial.print(pageHasFiles ? "has" : "doesn't have");
    Serial.println(" files.");

    Serial.println("========");
}

uint16_t Explorer::getStrHeight(String *c) {
    int16_t
        x = 0,
        y = 25,
        x1 = 0,
        y1 = 0;
    uint16_t
        height = 0,
        width = 0;

    tft.getTextBounds((const String&)c, x,y, &x1,&y1,&height,&width);

    return height;
}
uint16_t Explorer::getStrWidth(String *c) {
    int16_t
        x = 0,
        y = 25,
        x1 = 0,
        y1 = 0;
    uint16_t
        height = 0,
        width = 0;

    tft.getTextBounds((const String&)c, x,y, &x1,&y1,&height,&width);

    return c->length() * 6;
}

void Explorer::hideCurrentFiles() {
    for (int i = buttonSize; i < tftHeight - buttonSize; i+=2) {
        tft.drawFastHLine(0, i, tftWidth, backgroundColour);
    }
    //tft.fillRect(0, buttonSize, tftWidth, tftHeight - buttonSize*2, backgroundColour);
}

void Explorer::drawItem(uint8_t itemIndex, FileInfoHolder file) {
    tft.setCursor(
        screenPaddingX,
        buttonSize + (itemIndex*itemHeight) + itemHeight/2 - getStrHeight(&file.name)/4
    );
    tft.print(file.name);
    
    String size = String(file.size) + " b";

    tft.setCursor(
        tftWidth - screenPaddingX - getStrWidth(&size),
        buttonSize + (itemIndex*itemHeight) + itemHeight/2 - getStrHeight(&size)/4
    );
    tft.print(size);
}

void Explorer::renderCurrentDirectory(bool clearScreen = true) {
    if (clearScreen) {
        tft.fillScreen(backgroundColour);
    }

    tft.drawRect(0,0, tftWidth, buttonSize, upperColour);
    tft.setCursor(
        screenPaddingX,
        buttonSize/2 - getStrHeight(&currentPath)/4
    );
    tft.print(currentPath);

    String folderSize = String( currentFolder.size() ) + " b";
    tft.setCursor(
        tftWidth - screenPaddingX - getStrWidth(&folderSize),
        buttonSize/2 - getStrHeight(&folderSize)/4
    );
    tft.print(folderSize);

    for (int i = 0; true; i++) {
        if (!fileList[i]) {
            break;
        }

        drawItem(i, fileList[i]);
    }

    // reuse folderSize string
    folderSize = currentPage+1;
    tft.setCursor(
        tftWidth/2 - getStrWidth(&folderSize),
        tftHeight - buttonSize/2 - getStrHeight(&folderSize)/4
    );
    tft.print(folderSize);

    folderSize = "<";
    tft.setCursor(
        buttonSize - getStrWidth(&folderSize),
        tftHeight - buttonSize/2 - getStrHeight(&folderSize)/4
    );
    tft.print(folderSize);
    tft.drawRect(0, tftHeight-buttonSize, buttonSize*2, buttonSize, upperColour);

    folderSize = ">";
    tft.setCursor(
        tftWidth - buttonSize - getStrWidth(&folderSize),
        tftHeight - buttonSize/2 - getStrHeight(&folderSize)/4
    );
    tft.print(folderSize);
    tft.drawRect(tftWidth-buttonSize*2, tftHeight-buttonSize, buttonSize*2, buttonSize, upperColour);
}

FileInfoHolder Explorer::checkForTouch(TS_Point hit) {
    if (hit.x == 0 && hit.y == 0) { // if both x and y are 0
        return FileInfoHolder(); // return empty file (because 0,0 comes from bug?)
    }

    if (hit.x <= buttonSize*2 && hit.y >= tftHeight-buttonSize) { // previous button
        changePage(-1);
    } else if (hit.x >= tftWidth-buttonSize*2 && hit.y >= tftHeight-buttonSize) { // next button
        changePage(1);
    } else if (hit.y < buttonSize) { // top bar
        upDirectory();
    } else {
        // get file that was pressed
        FileInfoHolder found = fileList[ (hit.y - buttonSize) /itemHeight ];

        if (found.isDirectory) { // if directory, open it
            openDirectory( found.folderPath + "/" + found.name );
        }

        return found;
    }

    return FileInfoHolder(); // empty file (null)
}
