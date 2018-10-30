#include <Servo.h>

// assign LEDs, buttons and servos to pins
Servo player1Servo;
int playerOneButton = 4;
int playerTwoButton = 5;
int data = 2;
int clock = 3;
int latch = 4;

// declare variables
int valuesPlayer1[] = {1, 2, 4};
int valuesPlayer2[] = {16, 32, 64};
int playerRandoms[2];
int shiftOutValue;
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
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT); 
  Serial.begin(9600);
}

//run main program loop
void loop() {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0);
  digitalWrite(latch, HIGH);
  for (int i = 0; i < 2; i++) {
    playerRandoms[i] = random(3);
  }
  shiftOutValue = valuesPlayer1[playerRandoms[0]] + valuesPlayer2[playerRandoms[1]];
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, shiftOutValue);
  digitalWrite(latch, HIGH);
  delay(delayTime());
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0);
  digitalWrite(latch, HIGH);
  for (int i = 0; i < 2; i++) {
    playerRandoms[i] = 0;
  }
  delay(delayTime());
  /**if (scorePlayer1 >= 10) {
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
    }*/
}


void playerOneInput() {
  if (playerRandoms[0]) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, 8);
    digitalWrite(latch, HIGH);
    scorePlayer1++;
    Serial.print("Player 1: ");
    Serial.println(scorePlayer1, DEC);
    moveServo(scorePlayer1, player1Servo);
  }
}


void playerTwoInput() {
  if (playerRandoms[1]) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, 128);
    digitalWrite(latch, HIGH);
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
