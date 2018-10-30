int playerOneButton = 3;
int playerTwoButton = 3;

void setup() {
  Serial.begin(9600);
  pinMode(playerOneButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(playerOneButton), hello, FALLING);
}

void loop() {
  Serial.println(digitalRead(playerOneButton));
  delay(1000);
}

void hello() {
  Serial.println("trig");
}
