int button = 7;
int porchlight = 13;
int hallwaylight = 11;

void setup() {
  pinMode(porchlight, OUTPUT);
  pinMode(hallwaylight, OUTPUT);
  pinMode(button, INPUT_PULLUP);   
}

void turnOnPorch()
{
  digitalWrite(porchlight, HIGH);
}

void turnOnHallway()
{
  digitalWrite(hallwaylight, HIGH);
}

void turnOffPorch()
{
  digitalWrite(porchlight, LOW);
}

void turnOffHallway()
{
  digitalWrite(hallwaylight, LOW);
}

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

    while (getButtonState())   
    {

    }
  }
}