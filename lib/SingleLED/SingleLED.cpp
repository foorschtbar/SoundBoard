#include "SingleLED.h"

SingleLED *SingleLED::_instance = nullptr;

SingleLED::SingleLED(int16_t pin, neoPixelType t) : _pixels(new Adafruit_NeoPixel(1, pin, t))
{
    _instance = this;
}

void SingleLED::begin()
{
    _pixels->begin();
}

void SingleLED::setColor(int r, int g, int b)
{
    _pixels->setPixelColor(0, r, g, b);
    saveColor(r, g, b);
    _pixels->show();
}

void SingleLED::setColor(uint32_t color)
{
    _pixels->setPixelColor(0, color);
    _color = color;
    _pixels->show();
}

void SingleLED::toggleColor(int r, int g, int b)
{
    static boolean toggle;
    if (toggle)
    {
        _pixels->setPixelColor(0, 0, 0, 0);
        saveColor(0, 0, 0);
        toggle = false;
    }
    else
    {
        _pixels->setPixelColor(0, r, g, b);
        saveColor(r, g, b);
        toggle = true;
    }
    _pixels->show();
}

void SingleLED::off()
{
    _pixels->setPixelColor(0, 0, 0, 0);
    saveColor(0, 0, 0);
    _pixels->show();
}

void SingleLED::setBrightness(int brightness)
{
    _pixels->setBrightness(brightness);
}

u_int32_t SingleLED::getColor()
{
    return _color;
}

void SingleLED::saveColor(int r, int g, int b)
{
    _color = _pixels->Color(r, g, b);
}

void SingleLED::blink(int ms)
{
   blink(ms, 0, 0, 0);
}

void SingleLED::blink(int ms, int r, int g, int b)
{
    if (!_locked)
    {
        _locked = true;
        uint32_t color = getColor();
        setColor(r, g, b);
        _ticker.once_ms(ms, +[](uint32_t color)
                            {
            SingleLED::instance()->setColor(color);
            SingleLED::instance()->_locked = false; },
                        color);
    }
}