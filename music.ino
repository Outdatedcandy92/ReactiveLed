#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Major";
const char *password = "Major@2008";


IPAddress local_IP(192, 168, 2, 90);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

ESP8266WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Upload Audio File</title>
</head>
<body>
    <h1>Upload Audio File</h1>
    <input type="file" id="fileInput">
    <script>
        document.getElementById('fileInput').addEventListener('change', handleFile);

        function handleFile(event) {
            let file = event.target.files[0];
            if (file) {
                let reader = new FileReader();
                reader.onload = function(e) {
                    let audioContext = new (window.AudioContext || window.webkitAudioContext)();
                    audioContext.resume().then(() => {
                        audioContext.decodeAudioData(e.target.result, function(buffer) {
                            let source = audioContext.createBufferSource();
                            source.buffer = buffer;
                            let analyser = audioContext.createAnalyser();
                            source.connect(analyser);
                            analyser.connect(audioContext.destination);
                            source.start(0);

                            let dataArray = new Uint8Array(analyser.frequencyBinCount);
                            analyser.getByteTimeDomainData(dataArray);

                            let interval = setInterval(() => {
                                analyser.getByteTimeDomainData(dataArray);
                                let sum = 0;
                                for (let i = 0; i < dataArray.length; i++) {
                                    sum += dataArray[i];
                                }
                                let average = sum / dataArray.length;
                                let brightness = Math.floor((average / 255) * 1023); // Scale to 0-1023
                                fetch(`/setBrightness?value=${brightness}`);
                            }, 1000);

                            source.onended = () => clearInterval(interval);
                        });
                    });
                };
                reader.readAsArrayBuffer(file);
            }
        }
    </script>
</body>
</html>
)rawliteral";

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

    server.on("/", HTTP_GET, []() {
        server.send_P(200, "text/html", index_html);
    });

    server.on("/setBrightness", HTTP_GET, []() {
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