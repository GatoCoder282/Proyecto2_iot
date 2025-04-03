#include "Led.h"

Led::Led(byte pinNumber) {
    pin = pinNumber;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Led::off() {
    isBlinking = false;
    digitalWrite(pin, LOW);
    ledState = false;
}

void Led::on() {
    isBlinking = false;
    digitalWrite(pin, HIGH);
    ledState = true;
}

void Led::blink(byte blinksPerSecond) {
    if (blinksPerSecond == 0) return;
    
    isBlinking = true;
    blinkInterval = 500 / blinksPerSecond; 
    lastToggle = millis();
    
    digitalWrite(pin, HIGH);
    ledState = true;
}

void Led::update() {
    if (!isBlinking) return;
    
    unsigned long currentTime = millis();
    
    if (currentTime - lastToggle >= blinkInterval) {
        lastToggle = currentTime;
        ledState = !ledState;
        digitalWrite(pin, ledState ? HIGH : LOW);
    }
}