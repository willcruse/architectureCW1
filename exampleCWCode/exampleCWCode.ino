#include <Servo.h>

// assign LEDs, buttons and servos to pins
Servo servo;
int playerOneButton = 2;
int playerTwoButton = 3;
int playerThreeButton = 12;
int data = 10;
int clock = 9;
int latch = 8;
int resistorPin = A0;
int speakerPin = 13;
int player3LED[] = {4, 5, 6, 7};
 
// declare variables
/*
 * Values arrays correspond to the base 10 value of 
 * the LEDs position in the shift register
 */
int valuesPlayer1[] = {1, 2, 4};
int valuesPlayer2[] = {16, 32, 64};
//playerRandoms contains the random numbers for each player
int playerRandoms[3];
/*shiftOutValue is the sum of the player values getting written
 * to the shift register
 */
int shiftOutValue;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int scorePlayer3 = 0;
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
  servo.attach(11);
  servo.write(0);
  //Set pin modes
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(playerThreeButton, INPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT); 
  for (int i = 0; i < 4; i++) {
    pinMode(player3LED[i], OUTPUT);
  }
  Serial.begin(9600);
}

//run main program loop
void loop() {
  //Serial.println(digitalRead(playerThreeButton));
  if (digitalRead(playerThreeButton) == LOW) {
    tone(speakerPin, 131, 250);
  }
  //Turn all LEDs off
  shiftWrite(0);
  //Assign a random number for each player
  for (int i = 0; i < 3; i++) {
    playerRandoms[i] = random(3);
  }
  //Sum the random values and write to shift reg
  shiftOutValue = valuesPlayer1[playerRandoms[0]] + valuesPlayer2[playerRandoms[1]];
  shiftWrite(shiftOutValue);
  digitalWrite(player3LED[playerRandoms[2]], HIGH);
  /*Leaves LEDs on for time determined by potentiometer
   * previous is set to the time the loop is started
   * current is updated each iteration until there is a difference >= delayTime
   */
  unsigned long previousMS = millis();
  unsigned long currentMS = millis();
  int playerThreeFlag = 0;
  while (currentMS - previousMS < delayTime()) {
    //Check if playerThree has pushed a button
    if (digitalRead(playerThreeButton) == LOW && playerThreeFlag == 0) {
    Serial.print("Player 3 score: ");
    Serial.println(scorePlayer3);
      scorePlayer3++;
      tone(speakerPin, 220, 250);
      digitalWrite(player3LED[3], HIGH);
      playerThreeFlag = 1;
    }
    currentMS = millis();
  }
  //Set all LEDs to LOW
  digitalWrite(player3LED[3], LOW);
  shiftWrite(0);
  digitalWrite(player3LED[playerRandoms[2]], LOW);
  delay(delayTime());
  /*Check to see if any player has won by having a score of 10 or more
   * if so flashes all LEDs
   */
  if (scorePlayer1 >= 10 || scorePlayer2 >= 10 || scorePlayer3 >= 10) {
      for (int i = 0; i < 5; i++) {
        shiftWrite(255);
        for (int i = 0; i < 4; i++) {
          digitalWrite(player3LED[i], HIGH);
        }
        delay(250);
        shiftWrite(0);
        for (int i = 0; i < 4; i++) {
          digitalWrite(player3LED[i], LOW);
        }
        delay(250);
      }
      resetGame();
    }
    if (scorePlayer1 > scorePlayer2 && scorePlayer1 > scorePlayer3) {
      servo.write(20);
    } else if (scorePlayer2 > scorePlayer3) {
      servo.write(90);
    } else {
      servo.write(160);
    }
}


//ISR function called when playerOneButton is pressed
void playerOneInput() {
  Serial.print("Player 1 score: ");
  Serial.println(scorePlayer1);
  if (state) {
    shiftWrite(8);
    scorePlayer1++;
    tone(speakerPin, 262, 250);
  } else {
    tone(speakerPin, 165, 250);
  }
}

//ISR function called when playerTwoButton is pressed
void playerTwoInput() {
  Serial.print("Player 2 score: ");
  Serial.println(scorePlayer2);
  if (state) {
    shiftWrite(128);
    scorePlayer2++;
    tone(speakerPin, 247, 250);
  } else {
    tone(speakerPin, 147, 250);
  }
}

//Sets scores to 0 and resets servos
void resetGame() {
  scorePlayer1 = 0;
  scorePlayer2 = 0;
  scorePlayer3 = 0;
  servo.write(0);
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
