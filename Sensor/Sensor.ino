#include "UltrasonicSensor.h"
#include "WiFiManager.h"
#include "ServerClient.h"
#include "SensorApp.h"

// Configuraciones
const char* WIFI_SSID = "Guizada Aliaga";
const char* WIFI_PASSWORD = "GA.150825m.a";
const char* SERVER_IP = "192.168.0.64";
const int SERVER_PORT = 8888;

// Pines sensor ultrasónico
const byte TRIGGER_PIN = 5;  // GPIO5 (TRIG)
const byte ECHO_PIN = 18;    // GPIO18 (ECHO)

// Intervalo de medición (milisegundos)
const unsigned long MEASUREMENT_INTERVAL = 2000;

// Instancias de objetos
UltrasonicSensor sensor(TRIGGER_PIN, ECHO_PIN);
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ServerClient serverClient(SERVER_IP, SERVER_PORT);
SensorApp app(sensor, wifiManager, serverClient, MEASUREMENT_INTERVAL);

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}