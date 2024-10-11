#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class SingleLED
{
    public:
        SingleLED(int16_t pin, neoPixelType t);
        void begin();
        void setColor(int r, int g, int b);
        void setColor(int r, int g, int b, int brightness);
        void off();
        void setBrightness(int brightness);
    
    private:
        Adafruit_NeoPixel _pixels;
        int _pin;
};
       
