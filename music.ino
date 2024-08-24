#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>



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

    pinMode(D1, OUTPUT);

    server.on("/brightness", HTTP_GET, []() {
        if (server.hasArg("value")) {
            int brightness = server.arg("value").toInt();
            analogWrite(D1, brightness);
            server.send(200, "text/plain", "Brightness set");
        } else {
            server.send(400, "text/plain", "Bad Request");
        }
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}