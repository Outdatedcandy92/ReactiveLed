# ReactiveLed

https://github.com/user-attachments/assets/9a9d768a-4d46-4e9a-ba69-05f7019a8f49
## Overview


ReactiveLed is a project that uses an ESP8266 microcontroller to control a series of LEDs based on amplitude values received via WebSocket. The project includes a Python script to send amplitude data and an Arduino sketch to handle the WebSocket communication and control the LEDs.


## Requirements

- ESP8266 microcontroller
- Arduino IDE
- Python 3.x
- Required Python libraries: `websockets`, `asyncio`, `pyaudio`

## Setup

### Hardware

1. Connect the LEDs to the ESP8266 pins defined in the [`ledPins`]

### Software


1. **Arduino Sketch**:
    - Open [`music.ino`]in the Arduino IDE.
    - Install the required libraries: [`ESP8266WiFi`],[`ESPAsyncTCP`],[`ESPAsyncWebServer`]
    - Update the code wth your own WiFi `SSID` and `Password`
    - Update all the IP address values with your own values (you can leave the dns as is)
    - Upload the sketch to the ESP8266.

2. **Python Script**:
    - Install the required Python libraries:
      ```sh
      pip install websockets asyncio pyaudio
      ```
    - Change the IPaddr variable to your Esp's IP address  
    - Run the Python script [`main.py`] to start sending amplitude data to the ESP8266.

## Usage

1. Power on the ESP8266 and ensure it connects to the WiFi network.
2. Run the Python script [`main.py`]to start sending amplitude data.
3. The LEDs connected to the ESP8266 will react to the amplitude values received via WebSocket.





