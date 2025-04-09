#include "WifiManager.h"

WifiManager::WifiManager() {
    // Constructor
}

void WifiManager::addAccessPoint(const char* ssid, const char* password) {
    wifiMulti.addAP(ssid, password);
}

bool WifiManager::connect() {
    Serial.print("Conectando a WiFi");
    
    // Intentar conectar con timeout
    int attempts = 0;
    while (wifiMulti.run() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    
    if (isConnected()) {
        Serial.println("\nWiFi conectado");
        Serial.print("IP del ESP32: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nFalló la conexión WiFi");
        return false;
    }
}

bool WifiManager::isConnected() {
    return wifiMulti.run() == WL_CONNECTED;
}

IPAddress WifiManager::getLocalIP() {
    return WiFi.localIP();
}