#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

class WiFiManager {
private:
    WiFiMulti wifiMulti;
    const char* ssid;
    const char* password;

public:
    WiFiManager(const char* ssid, const char* password);
    bool connect();
    bool isConnected();
    String getLocalIP();
};

#endif // WIFI_MANAGER_H