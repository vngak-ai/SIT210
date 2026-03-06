int button = 7;
int porchlight = 13;
int hallwaylight = 11;

void setup() {
  pinMode(porchlight, OUTPUT);
  pinMode(hallwaylight, OUTPUT);
  pinMode(button, INPUT);
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
  if (digitalRead(button) == HIGH)
  {
    return true;
  } 
  else
  {
    return false;
  }
}

void loop() {

  if (getButtonState())
  {
    turnOnPorch();
    turnOnHallway();

    delay(3000);
    turnOffPorch();

    delay(3000);
    turnOffHallway();

    while (digitalRead(button) == HIGH)
    {

    }
  }

}