#include "ServerClient.h"

ServerClient::ServerClient(const char* ip, int port) {
    serverIP = ip;
    serverPort = port;
}

bool ServerClient::connect() {
    Serial.print("Conectando a servidor ");
    Serial.println(serverIP);
    
    if (client.connect(serverIP, serverPort)) {
        Serial.println(" Conectado al servidor");
        return true;
    } else {
        Serial.println("Falló la conexión al servidor");
        return false;
    }
}

bool ServerClient::sendData(float distance) {
    if (!client.connected()) {
        return false;
    }
    
    // Enviar la distancia como texto seguido de un salto de línea
    client.println(distance);
    return true;
}

String ServerClient::readResponse() {
    String response = "";
    
    if (client.connected() && client.available()) {
        response = client.readStringUntil('\n');
        Serial.print("Respuesta del servidor: ");
        Serial.println(response);
    }
    
    return response;
}

void ServerClient::disconnect() {
    if (client.connected()) {
        client.stop();
        Serial.println("Conexión cerrada");
    }
}