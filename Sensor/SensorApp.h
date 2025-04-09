#ifndef SENSOR_APP_H
#define SENSOR_APP_H

#include "UltrasonicSensor.h"
#include "WiFiManager.h"
#include "ServerClient.h"

class SensorApp {
private:
    UltrasonicSensor& sensor;
    WiFiManager& wifiManager;
    ServerClient& serverClient;
    
    unsigned long lastMeasurementTime;
    const unsigned long measurementInterval;

public:
    SensorApp(UltrasonicSensor& sensor, WiFiManager& wifiManager, ServerClient& serverClient, 
              unsigned long interval = 2000);
    void setup();
    void loop();
};

#endif // SENSOR_APP_H