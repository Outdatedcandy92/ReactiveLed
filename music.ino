#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

IPAddress local_IP(192, 168, 2, 90);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int ledPin = D1;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
        String message = String((char*)data).substring(0, len);
        Serial.println("Received amplitude: " + message);
        float amplitude = message.toFloat();
        int brightness = map(amplitude, 0, 100, 0, 255);
        analogWrite(ledPin, brightness);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);

    Serial.println();
    Serial.println("Starting WiFi connection...");
    Serial.print("Connecting to ");
    Serial.println(ssid);

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
