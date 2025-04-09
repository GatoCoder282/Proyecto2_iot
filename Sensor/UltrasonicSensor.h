#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

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

#endif // ULTRASONIC_SENSOR_H