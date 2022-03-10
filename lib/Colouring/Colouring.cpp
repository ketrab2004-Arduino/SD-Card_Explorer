
#include <Arduino.h>
#include <stdint.h>
#include <Adafruit_ILI9341.h>

/**
 * @brief converts rgb 8-8-8
 * into rgb 5-6-5
 * (bits)
 */
uint16_t colourFromRGB(unsigned char red, unsigned char green, unsigned char blue) {
    return 
        ((red >> 3) << (5+6)) +
        ((green >> 2) << 5) +
        (blue >> 3);
}

/**
 * @brief converts hsv to rgb 5-6-5
 * 
 * @see https://piandmore.wordpress.com/2020/08/15/rgb-to-hsv-and-vice-versa/
 */
uint16_t colourFromHSV(int hue, float saturation, float value) {
    float c = value*saturation;
    float tmp = hue/60;
    int tmp2 = tmp-2*floor((float)tmp/2);
    float x = c*(1-abs(tmp2-1));
    float m = value-c;
    float r,g,b;
    int i = floor((float)tmp);

    switch (i) {
        case 0:
            r = c;
            g = x;
            b = 0;
            break;
        case 1:
            r = x;
            g = c;
            b = 0;
            break;
        case 2: 
            r = 0;
            g = c;
            b = x;
            break;
        case 3:
            r = 0;
            g = x;
            b = c;
            break;
        case 4:
            r = x;
            g = 0;
            b = c;
            break;
        case 5:
            r = c;
            g = 0;
            b = x;
            break;
    }
    return colourFromRGB(
        constrain((int)255*(r+m),0,255),
        constrain((int)255*(g+m),0,255),
        constrain((int)255*(b+m),0,255)
    );
}
