#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
}

bool WiFiManager::connect() {
    wifiMulti.addAP(ssid, password);
    
    Serial.print("Conectando a WiFi");
    int attempts = 0;
    
    while (wifiMulti.run() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    
    if (isConnected()) {
        Serial.println("\n WiFi conectado");
        Serial.print("IP del ESP32: ");
        Serial.println(getLocalIP());
        return true;
    } else {
        Serial.println("\n Falló la conexión WiFi");
        return false;
    }
}

bool WiFiManager::isConnected() {
    return wifiMulti.run() == WL_CONNECTED;
}

String WiFiManager::getLocalIP() {
    return WiFi.localIP().toString();
}