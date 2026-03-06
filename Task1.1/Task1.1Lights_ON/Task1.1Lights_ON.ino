int button = 7;
int porchlight = 13;
int hallwaylight = 11;

void setup() {
  pinMode(porchlight, OUTPUT);
  pinMode(hallwaylight, OUTPUT);
  pinMode(button, INPUT);
}

void turnPorch()
{
  digitalWrite(porchlight, HIGH);
}
void turnHallway()
{
   digitalWrite(hallwaylight, HIGH);
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
    turnPorch();
    turnHallway();

    delay(30000);
    digitalWrite(porchlight, LOW);

    delay(30000);
    digitalWrite(hallwaylight, LOW);
  }

}