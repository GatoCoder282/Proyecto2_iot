#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <Arduino.h>
#include <WiFiClient.h>

class ServerClient {
private:
    const char* serverIP;
    int serverPort;
    WiFiClient client;

public:
    ServerClient(const char* ip, int port);
    bool connect();
    bool sendData(float distance);
    String readResponse();
    void disconnect();
};

#endif // SERVER_CLIENT_H