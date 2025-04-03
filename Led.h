#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
private:
    byte pin;
    bool isBlinking = false;
    unsigned long blinkInterval = 0;
    unsigned long lastToggle = 0;
    bool ledState = false;
public:
    Led(byte pinNumber);
    void off();
    void on();
    void blink(byte blinksPerSecond);
    void update();
};

#endif