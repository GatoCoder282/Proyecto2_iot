#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>
#include "DistanceMonitor.h"

class CommandProcessor {
private:
    DistanceMonitor* distanceMonitor;  // Puntero al monitor de distancia
    
public:
    CommandProcessor(DistanceMonitor* monitor);  // Constructor
    void processCommand(const String& command);  // Procesa comandos recibidos
};

#endif // COMMAND_PROCESSOR_H