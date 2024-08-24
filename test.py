import requests
import sys

brightness_value = sys.argv[1]

esp_ip = "http://192.168.2.90"

url = f"{esp_ip}/brightness?value={brightness_value}"

try:
    response = requests.get(url)
    
    if response.status_code == 200:
        print("Brightness set successfully")
    else:
        print(f"Failed to set brightness. Status code: {response.status_code}")
except requests.exceptions.RequestException as e:
    print(f"Error: {e}")
