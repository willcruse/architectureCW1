// assign LEDs and button to pins
int ledPin[] = {4,5,6};
int playerOneButton = 2;
int whiteLED = 9;

// declare variables
int delayTime = 500; // time delay between lights on/off
int randNumber;
int whiteLEDOn; 
int score = 0;

//setup interrupt, button input and LED outputs
void setup() {
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine
  for (int i=0; i<3; i++){
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(playerOneButton, INPUT);
  pinMode(whiteLED, OUTPUT);
  Serial.begin(9600);
}

//run main program loop
void loop() {
  randNumber = random(3); // select a random number
  digitalWrite(ledPin[randNumber], HIGH); // light the LED with this number
  delay(delayTime);
  digitalWrite(ledPin[randNumber], LOW);
  delay(delayTime);
  whiteLEDOn = digitalRead(whiteLED);
  //if whiteLED on = turn it off
  if(whiteLEDOn==HIGH){
    digitalWrite(whiteLED, LOW);
  }
  if (score >= 10) {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
          digitalWrite(ledPin[j], HIGH); 
        }
        digitalWrite(whiteLED, HIGH);
        tone(8, 262);
        delay(250);
        noTone(8);
        
        for (int j = 0; j < 3; j++) {
          digitalWrite(ledPin[j], LOW); 
        }
        digitalWrite(whiteLED, LOW);
        delay(250);
      }
      score = 0;  
    }

}


void playerOneInput() {
  if (digitalRead(ledPin[randNumber]) == HIGH) {
    digitalWrite(whiteLED, HIGH);
    score++;
    Serial.println(score, DEC); 
  }
}
