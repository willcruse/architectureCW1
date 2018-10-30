int playerOneButton = 2;
int playerTwoButton = 3;

void setup() {
  Serial.begin();
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), hello, FALLING);
}

void loop() {

}

void hello() {
  Serial.println("trig");
}
