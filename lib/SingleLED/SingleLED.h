#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

class SingleLED
{
public:
    inline static SingleLED *instance()
    {
        return _instance;
    }
    SingleLED(int16_t pin, neoPixelType t);
    void begin();
    void setColor(int r, int g, int b);
    void off();
    void setBrightness(int brightness);
    void toggleColor(int r, int g, int b);
    void saveColor();
    void restoreColor();
    void wink(int ms);

private:
    static SingleLED *_instance;
    Adafruit_NeoPixel* _pixels;
    Ticker _ticker;
    uint32_t _color;
    bool _locked;
};