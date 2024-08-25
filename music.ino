#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

IPAddress local_IP(192, 168, 2, 90);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int ledPins[] = {D0, D1, D2, D3, D4};
const int numLeds = 5;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
        String message = String((char*)data).substring(0, len);
        Serial.println("Received amplitude: " + message);
        float amplitude = message.toFloat();
        int numLedsToLight = map(amplitude, 0, 100, 0, numLeds);

        for (int i = 0; i < numLeds; i++) {
            digitalWrite(ledPins[i], i < numLedsToLight ? HIGH : LOW);
        }
    }
}

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    WiFi.config(local_IP, gateway, subnet, dns);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.begin();
    Serial.println("WebSocket server started");
}

void loop() {
    ws.cleanupClients();
}
