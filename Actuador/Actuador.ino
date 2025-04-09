#include "Led.h"
#include "WifiManager.h"
#include "ServerClient.h"
#include "DistanceMonitor.h"
#include "CommandProcessor.h"

// Configuración WiFi
const char* SSID = "Guizada Aliaga";
const char* PASSWORD = "GA.150825m.a";
const char* SERVER_IP = "192.168.0.64";
const int SERVER_PORT = 8889;

// Umbrales de distancia para activar los LEDs (en cm)
const int NUM_THRESHOLDS = 4;
float THRESHOLDS[NUM_THRESHOLDS] = {10.0, 30.0, 50.0, 70.0};

// Configuración de LEDs
const int NUM_LEDS = 4;
Led leds[NUM_LEDS] = {Led(5), Led(4), Led(16), Led(17)};

// Instancias de clases principales
WifiManager wifiManager;
ServerClient serverClient(SERVER_IP, SERVER_PORT);
DistanceMonitor distanceMonitor(leds, NUM_LEDS, THRESHOLDS, NUM_THRESHOLDS);
CommandProcessor commandProcessor(&distanceMonitor);

void setup() {
    Serial.begin(115200);

    // Apagar todos los LEDs explícitamente
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].off();
    }

    // Configurar y conectar WiFi
    wifiManager.addAccessPoint(SSID, PASSWORD);
    if (wifiManager.connect()) {
        // Si WiFi se conectó, intentar conectar al servidor
        serverClient.connect();
    }
}

void loop() {
    // Verificar conexión WiFi
    if (!wifiManager.isConnected()) {
        Serial.println("⚠️ Conexión WiFi perdida. Reintentando...");
        wifiManager.connect();
        return;
    }

    // Intentar reconectar al servidor si es necesario
    serverClient.reconnectIfNeeded();

    // Leer datos del servidor
    String receivedCommand = serverClient.readData();
    
    if (receivedCommand != "") {
        commandProcessor.processCommand(receivedCommand);
    }

    delay(50);
}