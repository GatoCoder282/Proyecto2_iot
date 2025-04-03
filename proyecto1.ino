#include <Arduino.h>
#include "UltrasonicSensor.h"
#include "Led.h"

// Definición de pines
Led leds[] = {Led(17), Led(16), Led(4), Led(0), Led(2), Led(15)};
UltrasonicSensor sensor(5, 18);

int activeLedIndex = -1;

void setup() {
    Serial.begin(115200);
    sensor.init();
    
    for (int i = 0; i < 6; i++) {
        leds[i].off();
    }
}

void loop() {
    for (int i = 0; i < 6; i++) {
        leds[i].update();
    }
    
    static unsigned long lastMeasurement = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastMeasurement >= 200) {
        lastMeasurement = currentTime;
        
        float distance = sensor.getDistance();

        Serial.print(", Distancia Medida: ");
        Serial.print(distance);
        Serial.println(" cm");
        
        const float measures[] = {5, 10, 15, 20, 25};
        const byte frequencies[] = {7, 3.4, 2.6, 1.8, 1, 0};
        
        int newActiveLed = 5;
        
        for (int i = 0; i < 5; i++) {
            if (distance < measures[i]) {
                newActiveLed = i;
                break;
            }
        }
      
        if (newActiveLed != activeLedIndex) {
            for (int i = 0; i < 6; i++) {
                leds[i].off();
            }
            
            if (frequencies[newActiveLed] > 0) {
                leds[newActiveLed].blink(frequencies[newActiveLed]);
            } else {
                leds[newActiveLed].on(); 
            }
            
            activeLedIndex = newActiveLed;
        }
    }
    
    delay(5);
}