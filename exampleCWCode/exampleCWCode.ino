// assign LEDs and button to pins
int ledPinPlayer1[] = {4,5,6};
int ledPinPlayer2[] = {10, 11, 12};
int playerOneButton = 2;
int playerTwoButton = 3;
int whiteLED = 9;
int greenPlayerLED = 13;

// declare variables
int delayTime = 500; // time delay between lights on/off
int randNumber;
int randNumber2;
int whiteLEDOn;
int greenPlayerLEDOn;
int scorePlayer1 = 0;
int scorePlayer2 = 0;

//setup interrupt, button input and LED outputs
void setup() {
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
  attachInterrupt(1, playerTwoInput, FALLING);
  for (int i=0; i<3; i++){
    pinMode(ledPinPlayer1[i], OUTPUT);
    pinMode(ledPinPlayer2[i], OUTPUT);
  }
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(greenPlayerLED, OUTPUT);
  Serial.begin(9600);
}

//run main program loop
void loop() {
  randNumber = random(3); // select a random number
  randNumber2 = random(3);
  digitalWrite(ledPinPlayer1[randNumber], HIGH); // light the LED with this number
  digitalWrite(ledPinPlayer2[randNumber2], HIGH);
  delay(delayTime);
  digitalWrite(ledPinPlayer1[randNumber], LOW);
  digitalWrite(ledPinPlayer2[randNumber2], LOW);
  delay(delayTime);
  whiteLEDOn = digitalRead(whiteLED);
  greenPlayerLEDOn = digitalRead(greenPlayerLED);
  //if whiteLED on = turn it off
  if(whiteLEDOn==HIGH){
    digitalWrite(whiteLED, LOW);
  }
  if (greenPlayerLEDOn == HIGH) {
    digitalWrite(greenPlayerLED, LOW);
  }
  if (scorePlayer1 >= 10) {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
          digitalWrite(ledPinPlayer1[j], HIGH);
        }
        digitalWrite(whiteLED, HIGH);
        tone(8, 262);
        delay(250);
        noTone(8);
        for (int j = 0; j < 3; j++) {
          digitalWrite(ledPinPlayer1[j], LOW);
        }
        digitalWrite(whiteLED, LOW);
        delay(250);
      }
      scorePlayer1 = 0;
    }
    if (scorePlayer2 >= 10) {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 3; j++) {
            digitalWrite(ledPinPlayer2[j], HIGH);
          }
          digitalWrite(greenPlayerLED, HIGH);
          tone(8, 131);
          delay(250);
          noTone(8);
          for (int j = 0; j < 3; j++) {
            digitalWrite(ledPinPlayer2[j], LOW);
          }
          digitalWrite(greenPlayerLED, LOW);
          delay(250);
        }
        scorePlayer2 = 0;
      }
}


void playerOneInput() {
  if (digitalRead(ledPinPlayer1[randNumber]) == HIGH) {
    digitalWrite(whiteLED, HIGH);
    scorePlayer1++;
    Serial.print("Player 1: ");
    Serial.println(scorePlayer1, DEC);
  }
}


void playerTwoInput() {
  if (digitalRead(ledPinPlayer2[randNumber]) == HIGH) {
    digitalWrite(greenPlayerLED, HIGH);
    scorePlayer2++;
    Serial.print("Player 2: ");
    Serial.println(scorePlayer2, DEC);
  }
}
