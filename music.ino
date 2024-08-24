#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);
File fsUploadFile;

const int ledPin = D1;

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    server.on("/", HTTP_GET, handleRoot);
    server.on("/upload", HTTP_POST, []() {
        server.send(200);
    }, handleFileUpload);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    server.send(200, "text/html", "<form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='file'><input type='submit' value='Upload'></form>");
}

void handleFileUpload() {
    HTTPUpload& upload = server.upload();
    static String filename;
    if (upload.status == UPLOAD_FILE_START) {
        filename = "/" + upload.filename;
        if (!filename.startsWith("/")) filename = "/" + filename;
        Serial.print("Upload File Name: "); Serial.println(filename);
        fsUploadFile = SPIFFS.open(filename, "w");
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
    } else if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile) fsUploadFile.close();
        Serial.print("Upload Size: "); Serial.println(upload.totalSize);
        server.send(200, "text/plain", "File Uploaded Successfully");
        processAudioFile(filename.c_str());
    }
}

void processAudioFile(const char* filename) {
    File audioFile = SPIFFS.open(filename, "r");
    if (!audioFile) {
        Serial.println("Failed to open file for reading");
        return;
    }

    while (audioFile.available()) {
        int audioValue = audioFile.read();
        int brightness = map(audioValue, 0, 255, 0, 1023);
        analogWrite(ledPin, brightness);
        delay(10);
    }

    audioFile.close();
}
