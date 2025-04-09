#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

class WifiManager {
private:
    WiFiMulti wifiMulti;
    
public:
    WifiManager();
    void addAccessPoint(const char* ssid, const char* password);
    bool connect();
    bool isConnected();
    IPAddress getLocalIP();
};

#endif // WIFI_MANAGER_H