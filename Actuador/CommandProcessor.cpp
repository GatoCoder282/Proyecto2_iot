#include "CommandProcessor.h"

// Constructor: inicializa con una referencia al monitor de distancia
CommandProcessor::CommandProcessor(DistanceMonitor* monitor) {
    distanceMonitor = monitor;  // Guarda la referencia al DistanceMonitor
}

// Procesa los comandos recibidos del servidor
void CommandProcessor::processCommand(const String& command) {
    Serial.print("Comando recibido: ");
    Serial.println(command);

    // Formato del comando es "DIST:123.45"
    if (command.startsWith("DIST:")) {
        String distStr = command.substring(5); // Extraer la parte después de "DIST:"
        float distance = distStr.toFloat();    // Convierte el string a float
        
        if (distance >= 0 || distStr == "0") { // Válido si es positivo o exactamente "0"
            Serial.print("Nueva distancia recibida: ");
            Serial.print(distance);
            Serial.println(" cm");
            
            // Actualizar la distancia en el monitor
            distanceMonitor->updateDistance(distance);
        } else {
            Serial.print("Valor de distancia inválido: ");
            Serial.println(distStr);
        }
    } else {
        Serial.println("Comando no reconocido");
    }
}