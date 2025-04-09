#include "DistanceMonitor.h"

// Constructor: inicializa con arrays de LEDs y umbrales
DistanceMonitor::DistanceMonitor(Led* ledArray, int ledCount, float* thresholdArray, int thresholdCount) {
    leds = ledArray;
    numLeds = ledCount;
    thresholds = thresholdArray;
    numThresholds = thresholdCount;
    currentDistance = 0.0;
}

// Actualiza la distancia y desencadena la actualización de LEDs
void DistanceMonitor::updateDistance(float distance) {
    currentDistance = distance;  // Almacena la nueva distancia
    updateLeds();                // Actualiza los LEDs según esta distancia
}

// Actualiza el estado de los LEDs según la distancia actual
void DistanceMonitor::updateLeds() {
    Serial.print("Actualizando LEDs para distancia: ");
    Serial.print(currentDistance);
    Serial.println(" cm");
    
    // Crea un array temporal para almacenar los estados de los LEDs
    bool* ledStates = new bool[numLeds]();  // Inicializado a false
    
    // Lógica de activación: solo un LED se encenderá según el rango de distancia
    if (currentDistance <= thresholds[0]) {  // Menor o igual a 10cm
        ledStates[0] = true;
        Serial.println("Activando LED 0");
    } 
    else if (currentDistance <= thresholds[1]) {  // Entre 10cm y 30cm
        ledStates[1] = true;
        Serial.println("Activando LED 1");
    } 
    else if (currentDistance <= thresholds[2]) {  // Entre 30cm y 50cm
        ledStates[2] = true;
        Serial.println("Activando LED 2");
    } 
    else if (currentDistance <= thresholds[3]) {  // Entre 50cm y 70cm
        ledStates[3] = true;
        Serial.println("Activando LED 3");
    } 
    else {  // Mayor a 70cm
        Serial.println("Todos los LEDs apagados");
        // Todos permanecen apagados (false)
    }
    
    // Actualiza el estado físico de cada LED según el array de estados
    for (int i = 0; i < numLeds; i++) {
        leds[i].setState(ledStates[i]);
    }
    
    // Libera la memoria del array temporal
    delete[] ledStates;
}

// Devuelve la distancia actual almacenada
float DistanceMonitor::getCurrentDistance() {
    return currentDistance;
}