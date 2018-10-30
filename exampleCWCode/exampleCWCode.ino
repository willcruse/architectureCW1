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
/*
 * Values arrays correspond to the base 10 value of 
 * the LEDs position in the shift register
 */
int valuesPlayer1[] = {1, 2, 4};
int valuesPlayer2[] = {16, 32, 64};
//playerRandoms contains the random numbers for each player
int playerRandoms[2];
/*shiftOutValue is the sum of the player values getting written
 * to the shift register
 */
int shiftOutValue;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
//Input from the potentiometer
int sensorValue = 0;
/*Used to check if LEDs are on or off
 * flipped from 1 -> 0 and back each time
 * shiftWrite is called
 */
int state = 1;

//setup interrupt, button input and shift register
void setup() {
  // specify interrupt routine
  attachInterrupt(digitalPinToInterrupt(2), playerOneInput, FALLING); 
  attachInterrupt(digitalPinToInterrupt(3), playerTwoInput, FALLING);
  //Attach servos and write to starting postion
  player1Servo.attach(7);
  player1Servo.write(0);
  //Set pin modes
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT); 
  Serial.begin(9600);
}

//run main program loop
void loop() {
  //Turn all LEDs off
  shiftWrite(0);
  //Assign a random number for each player
  for (int i = 0; i < 2; i++) {
    playerRandoms[i] = random(3);
  }
  //Sum the random values and write to shift reg
  shiftOutValue = valuesPlayer1[playerRandoms[0]] + valuesPlayer2[playerRandoms[1]];
  shiftWrite(shiftOutValue);
  //Leaves LEDs on for time determined by potentiometer
  delay(delayTime());
  shiftWrite(0);
  delay(delayTime());
  //Check to see if either player has won by having a score of 10 or more
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


//ISR function called when playerOneButton is pressed
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

//ISR function called when playerTwoButton is pressed
void playerTwoInput() {
  if (state) {
    shiftWrite(128);
    scorePlayer2++;
    tone(speakerPin, 131, 250);
  } else {
    tone(speakerPin, 87, 250);
  }
}

//Moves a given servo to 18 * the given score degrees
void moveServo(int score, Servo servo) {
  float pos = score * 18;
  servo.write(pos);
}

//Sets scores to 0 and resets servos
void resetGame() {
  scorePlayer1 = 0;
  scorePlayer2 = 0;
  player1Servo.write(0);
}

//Function to get input from the potentiometer and then translates that into a delay time
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

//Function to write a value to the shift register and change state
void shiftWrite(int value) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, value);
    digitalWrite(latch, HIGH);
    state = !state;
}
