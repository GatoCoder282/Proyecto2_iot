#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <Arduino.h>
#include <WiFiClient.h>

class ServerClient {
private:
    WiFiClient client;
    const char* serverIP;
    int serverPort;
    String inputBuffer;
    unsigned long lastReconnectAttempt;
    const unsigned long RECONNECT_INTERVAL = 5000;
    
public:
    ServerClient(const char* serverAddress, int port);
    bool connect();
    bool isConnected();
    bool reconnectIfNeeded();
    String readData();
    void sendData(const String& data);
};

#endif // SERVER_CLIENT_H