import pyaudio
import numpy as np
import requests
import time

esp_ip = "http://192.168.2.90"

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
CHUNK = 1024

audio = pyaudio.PyAudio()

stream = audio.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

def get_amplitude(data):
    audio_data = np.frombuffer(data, dtype=np.int16)
    amplitude = np.abs(audio_data).mean()
    return amplitude

try:
    while True:
        data = stream.read(CHUNK)
        amplitude = get_amplitude(data)
        brightness = int((amplitude / 32767) * 1023)
        url = f"{esp_ip}/brightness?value={brightness}"
        try:
            response = requests.get(url)
            if response.status_code == 200:
                print(f"Brightness set to {brightness}")
            else:
                print(f"Failed to set brightness. Status code: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f"Error: {e}")
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Stopping audio stream...")
finally:
    stream.stop_stream()
    stream.close()
    audio.terminate()
