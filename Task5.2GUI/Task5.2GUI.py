import tkinter as tk
import RPi.GPIO as GPIO

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

LED_PINS = {
    "Living Room": 18,  # Switch to GPIO 18 because it supports hardware PWM.
    "Bathroom":    27,
    "Closet":      22
}

PWM_FREQ = 1000  # PWM frequency: 1000 Hz

# Set all pins to OUTPUT
for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

# Create a separate PWM object for the Living Room.
living_pwm = GPIO.PWM(LED_PINS["Living Room"], PWM_FREQ)
living_pwm.start(0)  # Bắt đầu với duty cycle = 0 (tắt)


def update_leds():
    #Turn the LED on/off according to the selected room; reset the slider to 0 if changing rooms.
    selected_room = room_var.get()

    for room, pin in LED_PINS.items():
        if room == selected_room:
            if room == "Living Room":
                # Living Room: Use PWM, set according to the current slider value.
                duty = slider.get()
                living_pwm.ChangeDutyCycle(duty)
                slider_frame.pack(pady=10)  # Show Slider
            else:
                living_pwm.ChangeDutyCycle(0)  # Turn off PWM Living Room
                GPIO.output(pin, GPIO.HIGH)     # Turn on the LED in the other room.
                slider_frame.pack_forget()      # Hide slider
        else:
            if room == "Living Room":
                living_pwm.ChangeDutyCycle(0)
            else:
                GPIO.output(pin, GPIO.LOW)

def on_slider_change(value):
    #Called when the user drags the slider — updates the PWM duty cycle.
    if room_var.get() == "Living Room":
        duty = float(value)
        living_pwm.ChangeDutyCycle(duty)
        slider_label.config(text=f"Brightness: {int(duty)}%")

def on_exit():
   #Clean up GPIOs and close the window.
    living_pwm.stop()
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.LOW)
    GPIO.cleanup()
    window.destroy()

# GUI
window = tk.Tk()
window.title("Linda's Smart Lighting")
window.geometry("320x320")
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

# Frame contains slider (hidden by default, only visible when Living Room is selected)
slider_frame = tk.Frame(window)

slider_label = tk.Label(
    slider_frame,
    text="Brightness: 0%",
    font=("Arial", 11)
)
slider_label.pack()

slider = tk.Scale(
    slider_frame,
    from_=0,
    to=100,
    orient=tk.HORIZONTAL,
    length=220,
    label="Light Intensity (%)",
    font=("Arial", 10),
    command=on_slider_change
)
slider.set(0)
slider.pack()

tk.Button(
    window,
    text="Exit",
    font=("Arial", 12),
    bg="red",
    fg="white",
    width=10,
    command=on_exit
).pack(pady=20)

window.mainloop(