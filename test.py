import asyncio
import websockets
import pyaudio
import numpy as np

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

async def send_audio():
    uri = "ws://192.168.2.90/ws"
    async with websockets.connect(uri, ping_interval=10, ping_timeout=5) as websocket:
        try:
            while True:
                data = stream.read(CHUNK)
                amplitude = get_amplitude(data)
                await websocket.send(str(amplitude))
                await asyncio.sleep(0.1)  
        except websockets.exceptions.ConnectionClosed:
            print("Server disconnected")
        except Exception as e:
            print(f"An error occurred: {e}")

if __name__ == "__main__":
    try:
        asyncio.run(send_audio())
    except KeyboardInterrupt:
        print("WebSocket client stopped")
    finally:
        stream.stop_stream()
        stream.close()
        audio.terminate()
        print("Audio stream closed")
