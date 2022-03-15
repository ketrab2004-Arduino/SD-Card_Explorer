# SD Card Explorer

![A dark photograph showcasing the explorer in the root folder of my microSD](./memorabilia/banner_daylight_trimmed.jpg)
<br/>

## Parts:
* Arduino MEGA 2560
* A [2.8'' TFT LCD shield w/Cap Touch and microSD](https://www.adafruit.com/product/1947)
(anything that works with ILI9341 should work)
* A microSD

## Setup:
1. Make sure your microSD has files and folders to explore
1. Attach the TFT to the top of the arduino
1. Change your settings in reader.ino
    * `DEBUG`: whether or not to serial print debug messages
    * `BRIGHTNESS`: 0-255 for the backlight strength
    * `SD_CS`: if the SD card is connected to a different port
1. Upload the sketch
1. Explore :)
