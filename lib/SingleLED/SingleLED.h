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
    void setColor(uint32_t color);
    void off();
    void setBrightness(int brightness);
    void toggleColor(int r, int g, int b);
    void blink(int ms);
    void blink(int ms, int r, int g, int b);
    uint32_t getColor(); // Actual color without the impact of brightness

private:
    static SingleLED *_instance;
    Adafruit_NeoPixel *_pixels;
    Ticker _ticker;
    uint32_t _color;
    bool _locked;
    uint32_t _brightness;
    void saveColor(int r, int g, int b);
    
};