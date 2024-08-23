const int ledPin = 9;
const int soundSensorPin = A0;
const float sensitivity = 10;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(soundSensorPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    int soundValue = analogRead(soundSensorPin);
    soundValue = soundValue * sensitivity;

    if (soundValue > 1023) {
        soundValue = 1023;
    } else if (soundValue < 0) {
        soundValue = 0;
    }

    int brightness = map(soundValue, 390, 1023, 0, 255);

    analogWrite(ledPin, brightness);
    Serial.println(soundValue);
    delay(10);
}
