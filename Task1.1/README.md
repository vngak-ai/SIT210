# SIT210 – Task 1.1P  
This projects impliments a lighting support system, which is designed to assit the elder when they return home at night. This system will record the state of the button. When the button is pressed, both the porch light and the hallway light are turned on. After 30 seconds, the porch light will turn off, and after 60 seconds, the hallway light will turn off (30 seconds after the porch light turns off). This is to ensure that the resident has enough time to safely enter the house. 

The hardware components included are:
- Arduino Nano 33 IoT
- Push button
- LEDs - Red for Porch light, and Orange for Hallway light
- Breadboard
- Jumper wires

Instead of putting all the codes in one block, they are all divided into seperated functions which perform certain tasks. 
The following functions are used in the program:
- getButtonState()
This function reads the current state of the push button and returns whether the button is pressed or not.
- turnOnPorch()
This function turns on the porch LED connected to the Arduino.
- turnOffPorch()
This function turns off the porch LED after the specified time.
- turnOnHallway()
This function activates the hallway LED.
- turnOffHallway()
This function turns off the hallway LED after the required duration.
