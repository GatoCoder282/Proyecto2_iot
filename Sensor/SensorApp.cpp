#include "SensorApp.h"

SensorApp::SensorApp(UltrasonicSensor& sensor, WiFiManager& wifiManager, ServerClient& serverClient, 
                     unsigned long interval)
    : sensor(sensor), wifiManager(wifiManager), serverClient(serverClient), 
      measurementInterval(interval) {
    
    lastMeasurementTime = 0;
}

void SensorApp::setup() {
    Serial.begin(115200);
    sensor.init();
    
    // Intentar conectar a WiFi
    if (!wifiManager.connect()) {
        Serial.println("No se pudo conectar a WiFi. Reiniciando...");
        ESP.restart();
    }
    
    delay(500);
}

void SensorApp::loop() {
    unsigned long currentTime = millis();
    
    // Verificar conexión WiFi
    if (!wifiManager.isConnected()) {
        Serial.println("Conexión WiFi perdida. Reintentando...");
        wifiManager.connect();
        return;
    }
    
    // Realizar medición cada intervalo
    if (currentTime - lastMeasurementTime >= measurementInterval) {
        lastMeasurementTime = currentTime;
        
        // Medir distancia
        float distance = sensor.getDistance();
        Serial.print("Distancia: ");
        Serial.print(distance);
        Serial.println(" cm");
        
        // Enviar datos al servidor
        if (serverClient.connect()) {
            if (serverClient.sendData(distance)) {
                // Leer respuesta (opcional)
                serverClient.readResponse();
            }
            
            // Cerrar conexión
            serverClient.disconnect();
        } else {
            Serial.println("Reintentando en el próximo ciclo...");
        }
    }
}