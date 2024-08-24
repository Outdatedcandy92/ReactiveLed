#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "";
const char *password = "";

IPAddress local_IP(192, 168, 2, 90);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

ESP8266WebServer server(80);

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Starting WiFi connection...");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    if (!WiFi.config(local_IP, gateway, subnet, dns)) {
        Serial.println("STA Failed to configure");
    }

    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        attempts++;
        if (attempts > 30) {
            Serial.println("Failed to connect to WiFi");
            return;
        }
    }
    Serial.println();
    Serial.println("Connected to WiFi");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", "<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>");
    });
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
