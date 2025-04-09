#ifndef DISTANCE_MONITOR_H
#define DISTANCE_MONITOR_H

#include <Arduino.h>
#include "Led.h"

class DistanceMonitor {
private:
    Led* leds;              // Array de objetos LED
    int numLeds;            // Número de LEDs
    float* thresholds;      // Umbrales de distancia para activación
    int numThresholds;      // Número de umbrales
    float currentDistance;  // Distancia actual registrada
    
public:
    DistanceMonitor(Led* ledArray, int ledCount, float* thresholdArray, int thresholdCount);
    void updateDistance(float distance); // Actualiza la distancia y los LEDs
    void updateLeds();                   // Actualiza estados de LEDs según distancia
    float getCurrentDistance();          // Obtiene la distancia actual
};

#endif // DISTANCE_MONITOR_H