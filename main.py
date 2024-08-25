import asyncio
import websockets
import numpy as np
import pyaudio

IPaddr = "192.168.2.90"

CHUNK = 512  
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100

p = pyaudio.PyAudio()
stream = p.open(format=FORMAT,
                channels=CHANNELS,
                rate=RATE,
                input=True,
                frames_per_buffer=CHUNK)

def get_amplitude(data):
    audio_data = np.frombuffer(data, dtype=np.int16)
    amplitude = np.abs(audio_data).mean()
    return amplitude

async def send_audio():
    uri = f"ws://{IPaddr}/ws"
    async with websockets.connect(uri, ping_interval=10, ping_timeout=5) as websocket:
        try:
            while True:
                data = stream.read(CHUNK, exception_on_overflow=False)
                amplitude = get_amplitude(data)
                print(amplitude, flush=True) 
                await websocket.send(str(amplitude))
                await asyncio.sleep(0.1)  
        except websockets.exceptions.ConnectionClosed:
            print("Server disconnected")
        except Exception as e:
            print(f"An error occurred: {e}")

asyncio.run(send_audio())