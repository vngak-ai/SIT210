import asyncio
import speech_recognition as sr
from bleak import BleakClient, BleakScanner

# BLE configuration
DEVICE_NAME = "LindaLightSystem"
CHAR_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"

# Map voice phrases to command bytes
COMMAND_MAP = {
    "lights on": 1,
    "light on": 1,
    "turn on the lights": 1,
    "turn on light": 1,
    "lights off": 2,
    "light off": 2,
    "turn off the lights": 2,
    "turn off light": 2,
    "fan on": 3,
    "turn on the fan": 3,
    "turn on fan": 3,
    "fan off": 4,
    "turn off the fan": 4,
    "turn off fan": 4,
}

CMD_NAMES = {1: "Lights ON", 2: "Lights OFF", 3: "Fan ON", 4: "Fan OFF"}


def listen_for_command(recognizer, mic):
    """Capture audio and convert to text using Google API"""
    with mic as source:
        recognizer.adjust_for_ambient_noise(source, duration=0.3)
        print("\nListening...")
        try:
            audio = recognizer.listen(source, phrase_time_limit=3, timeout=5)
        except sr.WaitTimeoutError:
            return None

    try:
        text = recognizer.recognize_google(audio).lower()
        print(f"Heard: '{text}'")
        return text
    except sr.UnknownValueError:
        print("Could not understand")
        return None
    except sr.RequestError as e:
        print(f"API error: {e}")
        return None


def parse_command(text):
    """Find a matching command in the text"""
    if text is None:
        return None
    for phrase, code in COMMAND_MAP.items():
        if phrase in text:
            return code
    return None


async def find_arduino():
    """Scan for the Arduino by its BLE name"""
    print("Scanning for Arduino...")
    devices = await BleakScanner.discover(timeout=5.0)
    for d in devices:
        if d.name == DEVICE_NAME:
            print(f"Found: {d.address}")
            return d.address
    return None


async def main():
    address = await find_arduino()
    if not address:
        print("Arduino not found")
        return

    async with BleakClient(address) as client:
        print(f"Connected to {address}")
        print("Say a command: lights on / lights off / fan on / fan off")
        print("Press Ctrl+C to stop")

        recognizer = sr.Recognizer()
        mic = sr.Microphone()

        while True:
            text = listen_for_command(recognizer, mic)
            cmd = parse_command(text)

            if cmd:
                await client.write_gatt_char(CHAR_UUID, bytes([cmd]))
                print(f"Sent: {CMD_NAMES[cmd]}")
            elif text:
                print(f"No matching command in: '{text}'")

            await asyncio.sleep(0.1)


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nStopped")
