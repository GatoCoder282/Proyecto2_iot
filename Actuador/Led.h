#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
private:
    byte pin;      // Pin del ESP32 al que está conectado el LED
    bool ledState; // Estado actual del LED (encendido/apagado)
    
public:
    Led(byte pinNumber);  // Constructor
    void off();          // Apagar el LED
    void on();           // Encender el LED
    void setState(bool state); // Establecer estado específico
    bool getState();     // Obtener estado actual
    byte getPin();       // Obtener número de pin
};

#endif // LED_H