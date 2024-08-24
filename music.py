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

print("Available audio devices:")
for i in range(audio.get_device_count()):
    info = audio.get_device_info_by_index(i)
    print(f"{i}: {info['name']}")

device_index = None
for i in range(audio.get_device_count()):
    info = audio.get_device_info_by_index(i)
    if 'CABLE Output (VB-Audio Virtual Cable)' in info['name']:
        device_index = i
        break

if device_index is None:
    print("Virtual Audio Cable not found. Please ensure it is installed and set as the default playback device.")
    exit(1)

print(f"Using device index: {device_index}")

stream = audio.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    input_device_index=device_index,
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
