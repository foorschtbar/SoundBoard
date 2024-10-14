#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class SingleLED
{
public:
    SingleLED(Adafruit_NeoPixel &);
    void begin();
    void setColor(int r, int g, int b);
    void off();
    void setBrightness(int brightness);
    void toggleColor(int r, int g, int b);
    void saveColor();
    void restoreColor();

private:
    Adafruit_NeoPixel _pixels;
    uint32_t _color;
};