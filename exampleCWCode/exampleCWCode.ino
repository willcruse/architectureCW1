#include <Servo.h>

// assign LEDs, buttons and servos to pins
Servo player1Servo;
int playerOneButton = 2;
int playerTwoButton = 3;
int data = 10;
int clock = 9;
int latch = 8;
int resistorPin = A0;
int speakerPin = 13;

// declare variables
int valuesPlayer1[] = {1, 2, 4};
int valuesPlayer2[] = {16, 32, 64};
int playerRandoms[2];
int shiftOutValue;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int sensorValue = 0;
int state = 1;

//setup interrupt, button input and LED outputs
void setup() {
  attachInterrupt(digitalPinToInterrupt(2), playerOneInput, FALLING); // specify interrupt routine
  attachInterrupt(digitalPinToInterrupt(3), playerTwoInput, FALLING);
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
  shiftWrite(0);
  for (int i = 0; i < 2; i++) {
    playerRandoms[i] = random(3);
  }
  shiftOutValue = valuesPlayer1[playerRandoms[0]] + valuesPlayer2[playerRandoms[1]];
  shiftWrite(shiftOutValue);
  delay(delayTime());
  shiftWrite(0);
  delay(delayTime());
  if (scorePlayer1 >= 10) {
      for (int i = 0; i < 5; i++) {
        shiftWrite(15);
        tone(speakerPin, 262);
        delay(250);
        shiftWrite(0);
        delay(250);
      }
      resetGame();
    }
    if (scorePlayer2 >= 10) {
        for (int i = 0; i < 5; i++) {
          shiftWrite(240);
          tone(speakerPin, 262);
          delay(250);
          shiftWrite(0);
          delay(250);
      }
        resetGame();
    }
}


void playerOneInput() {
  if (state) {
    shiftWrite(8);
    scorePlayer1++;
    moveServo(scorePlayer1, player1Servo);
    tone(speakerPin, 262, 250);
  } else {
    tone(speakerPin, 175, 250);
  }
}


void playerTwoInput() {
  if (state) {
    shiftWrite(128);
    scorePlayer2++;
    tone(speakerPin, 131, 250);
  } else {
    tone(speakerPin, 87, 250);
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

void shiftWrite(int value) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, value);
    digitalWrite(latch, HIGH);
    state = !state;
}
