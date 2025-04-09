#include "ServerClient.h"

ServerClient::ServerClient(const char* serverAddress, int port) {
    serverIP = serverAddress;
    serverPort = port;
    inputBuffer = "";
    lastReconnectAttempt = 0;
}

bool ServerClient::connect() {
    if (client.connect(serverIP, serverPort)) {
        Serial.println("Conectado al servidor");
        return true;
    } else {
        Serial.println("Falló la conexión al servidor");
        return false;
    }
}

bool ServerClient::isConnected() {
    return client.connected();
}

bool ServerClient::reconnectIfNeeded() {
    if (!isConnected()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastReconnectAttempt > RECONNECT_INTERVAL) {
            lastReconnectAttempt = currentMillis;
            Serial.println("Intentando conectar al servidor...");
            return connect();
        }
        return false;
    }
    return true;
}

String ServerClient::readData() {
    String receivedData = "";
    
    if (!isConnected()) {
        return receivedData;
    }
    
    while (client.available()) {
        char c = client.read();
        if (c == '\n') {
            receivedData = inputBuffer;
            inputBuffer = "";
            return receivedData;
        } else {
            inputBuffer += c;
        }
    }
    
    return receivedData;
}

void ServerClient::sendData(const String& data) {
    if (isConnected()) {
        client.println(data);
    }
}