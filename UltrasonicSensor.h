#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
private:
    byte triggerPin;
    byte echoPin;
public:
    UltrasonicSensor(byte trig, byte echo);
    void init();
    float getDistance();
};

#endif