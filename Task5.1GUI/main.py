import tkinter as tk
import RPi.GPIO as GPIO

# GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

LED_PINS = {
    "Living Room": 17,
    "Bathroom":    27,
    "Closet":      22
}

for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

def update_leds():
    selected_room = room_var.get()
    for room, pin in LED_PINS.items():
        if room == selected_room:
            GPIO.output(pin, GPIO.HIGH)
        else:
            GPIO.output(pin, GPIO.LOW)

def on_exit():
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.LOW)
    GPIO.cleanup()
    window.destroy()

# Building GUI
window = tk.Tk()
window.title("Linda's Smart Lighting")
window.geometry("300x250")
window.resizable(False, False)

tk.Label(
    window,
    text="Select a Room to Turn On",
    font=("Arial", 14, "bold")
).pack(pady=15)

room_var = tk.StringVar(value="")

for room in LED_PINS.keys():
    tk.Radiobutton(
        window,
        text=room,

        variable=room_var,
        value=room,
        font=("Arial", 12),
        command=update_leds
    ).pack(anchor="w", padx=40)

tk.Button(
    window,
    text="Exit",
    font=("Arial", 12),
    bg="red",
    fg="white",
    width=10,
    command=on_exit
).pack(pady=20)

window.mainloop()
