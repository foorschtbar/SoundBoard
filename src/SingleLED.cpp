#include "singleled.h"

SingleLED::SingleLED(int16_t pin, neoPixelType t)
{
    _pixels = Adafruit_NeoPixel(1, pin, t);
}

void SingleLED::begin()
{
    _pixels.begin();
    _pixels.setBrightness(255);
    _pixels.clear();
}

void SingleLED::setColor(int r, int g, int b)
{
    _pixels.setPixelColor(0, r, g, b);
    _pixels.show();
}

void SingleLED::setColor(int r, int g, int b, int brightness)
{
    _pixels.setPixelColor(0, r, g, b);
    _pixels.setBrightness(brightness);
    _pixels.show();
}

void SingleLED::off()
{
    _pixels.setPixelColor(0, 0, 0, 0);
    _pixels.show();
}

void SingleLED::setBrightness(int brightness)
{
    _pixels.setBrightness(brightness);
    _pixels.show();
}

