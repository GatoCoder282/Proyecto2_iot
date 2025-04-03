#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(byte trig, byte echo) {
    triggerPin = trig;
    echoPin = echo;
}

void UltrasonicSensor::init() {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float UltrasonicSensor::getDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    unsigned long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}