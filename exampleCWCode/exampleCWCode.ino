#include <Servo.h>

// assign LEDs, buttons and servos to pins
int ledPinPlayer1[] = {4,5,6};
int ledPinPlayer2[] = {10, 11, 12};
int playerOneButton = 2;
int playerTwoButton = 3;
int whiteLED = 9;
int greenPlayerLED = 13;
Servo player1Servo;

// declare variables
int randNumber;
int randNumber2;
int whiteLEDOn;
int greenPlayerLEDOn;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int resistorPin = A0;
int sensorValue = 0;

//setup interrupt, button input and LED outputs
void setup() {
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
  attachInterrupt(1, playerTwoInput, FALLING);
  player1Servo.attach(7);
  player1Servo.write(0);
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
  delay(delayTime());
  digitalWrite(ledPinPlayer1[randNumber], LOW);
  digitalWrite(ledPinPlayer2[randNumber2], LOW);
  delay(delayTime());
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
      resetGame();
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
        resetGame();
    }
}


void playerOneInput() {
  if (digitalRead(ledPinPlayer1[randNumber]) == HIGH) {
    digitalWrite(whiteLED, HIGH);
    scorePlayer1++;
    Serial.print("Player 1: ");
    Serial.println(scorePlayer1, DEC);
    moveServo(scorePlayer1, player1Servo);
  }
}


void playerTwoInput() {
  if (digitalRead(ledPinPlayer2[randNumber2]) == HIGH) {
    digitalWrite(greenPlayerLED, HIGH);
    scorePlayer2++;
    Serial.print("Player 2: ");
    Serial.println(scorePlayer2, DEC);
  }
}

void moveServo(int score, Servo servo) {
  float pos = score * 18;
  servo.write(pos);
}

void resetGame() {
  scorePlayer1 = 0;
  scorePlayer2 = 0;
  player1Servo.write(0);
}

int delayTime(){
    sensorValue = analogRead(resistorPin);
  
  if (sensorValue >=900){
    return 100;
  }
  else if (sensorValue >=700){
    return 300;
  }
  else if (sensorValue >= 500){
    return 500;
    }
  else if (sensorValue >= 300){
    return 700;
  }
  else if (sensorValue >= 100){
    return 900;
  }
  else {
    return 1000;
  }
}
