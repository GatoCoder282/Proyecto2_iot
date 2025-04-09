#include "Led.h"

// Constructor: inicializa el LED en un pin específico y lo apaga
Led::Led(byte pinNumber) {
    pin = pinNumber;
    ledState = false;
    pinMode(pin, OUTPUT);    // Configura el pin como salida
    digitalWrite(pin, LOW);  // Inicialmente apagado
}

// Apaga el LED
void Led::off() {
    digitalWrite(pin, LOW);  // Establece el pin en nivel bajo (0V)
    ledState = false;        // Actualiza el estado interno
    Serial.print("LED en pin ");
    Serial.print(pin);
    Serial.println(" APAGADO");
}

// Enciende el LED
void Led::on() {
    digitalWrite(pin, HIGH); // Establece el pin en nivel alto (3.3V en ESP32)
    ledState = true;         // Actualiza el estado interno
    Serial.print("LED en pin ");
    Serial.print(pin);
    Serial.println(" ENCENDIDO");
}

// Establece el estado del LED (encendido/apagado)
void Led::setState(bool state) {
    if (state) {
        on();  // Si state es true, enciende el LED
    } else {
        off(); // Si state es false, apaga el LED
    }
}

// Devuelve el estado actual del LED
bool Led::getState() {
    return ledState;
}

// Devuelve el número de pin al que está conectado el LED
byte Led::getPin() {
    return pin;
}