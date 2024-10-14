#include "SingleLED.h"

SingleLED::SingleLED(Adafruit_NeoPixel &pixels)
{
    _pixels = pixels;
}

void SingleLED::begin()
{
    _pixels.begin();
}

void SingleLED::setColor(int r, int g, int b)
{
    _pixels.setPixelColor(0, r, g, b);
    _pixels.show();
}

void SingleLED::saveColor()
{
    _color = _pixels.getPixelColor(0);
}

void SingleLED::toggleColor(int r, int g, int b)
{
    static boolean toggle;
    if(toggle)
    {
        _pixels.setPixelColor(0, 0, 0, 0);
        toggle = false;
    }
    else
    {
        _pixels.setPixelColor(0, r, g, b);
        toggle = true;
    }
    _pixels.show();
}

void SingleLED::restoreColor()
{
    _pixels.setPixelColor(0, _color);
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
}

