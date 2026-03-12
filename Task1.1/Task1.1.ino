int button = 7;
int porchlight = 13;
int hallwaylight = 11;

void setup() {
  pinMode(porchlight, OUTPUT);
  pinMode(hallwaylight, OUTPUT);
  pinMode(button, INPUT_PULLUP);   
}

//Turns on the porch LED connected to the Arduino
void turnOnPorch() 
{
  digitalWrite(porchlight, HIGH);
}

//Activates the hallway LED
void turnOnHallway()
{
  digitalWrite(hallwaylight, HIGH);
}

//Turns off the porch LED after the specified time
void turnOffPorch()
{
  digitalWrite(porchlight, LOW);
}

//Turns off the hallway LED after the required duration
void turnOffHallway()
{
  digitalWrite(hallwaylight, LOW);
}

//Reads the current state of the push button and returns whether the button is pressed or not
bool getButtonState()
{
  return digitalRead(button) == LOW;   
}

void loop()
{
  if (getButtonState())
  {
    turnOnPorch();
    turnOnHallway();

    delay(30000);
    turnOffPorch();

    delay(30000);
    turnOffHallway();

    while (getButtonState()){} //Prevents holding button
  }
}