int buttonPin[4] = {33, 34, 35, 36};
int ledPin[4] = {2, 3, 4, 5};
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {
  {false, false, true, false},
  {true, false, false, false},
  {false, false, false, true},
};
int x;
int stepButtonPin1 = 31;
int stepButtonPin2 = 32;

int tempo = 0;


int currentStep = 0;
unsigned long lastStepTime = 0;
int currentSequence = 0;
int currentButton1State, currentButton2State = LOW;
int lastButton1State, lastButton2State;


void setup() {
  for (x = 0; x < 4; x++) {
    pinMode(buttonPin[x], INPUT);
    pinMode(ledPin[x], OUTPUT);
  }
  pinMode(stepButtonPin1, INPUT);
  pinMode(stepButtonPin2, INPUT);
}

void loop() {
  checkButtons();
  setSequence();
  sequence();
  setLeds();

}
//set sequence function that controls the two buttons that can switch forwards and backwards between the different sequences
void setSequence () {
  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;
  currentButton1State = digitalRead(stepButtonPin1);
  currentButton2State = digitalRead(stepButtonPin2);

  if (currentButton1State == HIGH && lastButton1State == LOW) {
    currentSequence++;
    //jumps from "highest" sequence to "lowest"
    if (currentSequence >= 3) {
      currentSequence = 0;
    }
  }
  if (currentButton2State == HIGH && lastButton2State == LOW) {
    currentSequence --;
    //jumps from "lowest" sequence to "highest"
    if (currentSequence < 0) {
      currentSequence = 2;
    }
  }
}

//function moves steps along and determines what note is played
void sequence() {

  tempo = analogRead(A18);
  //determines if it's time to move to the next step
  if (millis() > lastStepTime + tempo) {   
    //digitalWrite(ledPin[currentStep], LOW);  
  //moves to next step
    currentStep = currentStep + 1;         
    if (currentStep > 3) {
      currentStep = 0;
    }
  //determines which note is played in each different sequence
    if (stepState[0][currentStep]  == true) {
      //makes sure note is off before turning it on
      usbMIDI.sendNoteOff(60, 0, 1);
      usbMIDI.sendNoteOn(60, 127, 1);

    }
    if (stepState[1][currentStep] == true) {
      usbMIDI.sendNoteOff(62, 0, 1);
      usbMIDI.sendNoteOn(62, 127, 1);
    }
    if (stepState[2][currentStep] == true) {
      usbMIDI.sendNoteOff(64, 0, 1);
      usbMIDI.sendNoteOn(64, 127, 1);
    }
//sets last step time
    lastStepTime = millis();               //set lastStepTime to the current time

  }

  //digitalWrite(ledPin[currentStep], HIGH); //turn on the new led

}

//function determines if/when buttons are pressed 
void checkButtons() {
  for (x = 0; x < 4; x++) {
    lastButtonState[x] = buttonState[x];
    buttonState[x] = digitalRead(buttonPin[x]);

    if (buttonState[x] == HIGH && lastButtonState[x] == LOW) {
      if (stepState[currentSequence][x] == false) {
        stepState[currentSequence][x] = true;
      } else if (stepState[currentSequence][x] == true) {
        stepState[currentSequence][x] = false;
      }
    }
  }
}

//function turns leds on or off following the sequence
void setLeds() {
  for (x = 0; x < 4; x++) {
    if (x == currentStep) {
      analogWrite(ledPin[x], 255);
      //     usbMIDI.sendNoteOn(60, 127, 1);
    }
    else if (stepState[currentSequence][x] == HIGH) {
      analogWrite(ledPin[x], 20);
    }
    else {
      analogWrite (ledPin[x], 0);
      //     usbMIDI.sendNoteOff(60, 0, 1);
    }
  }
}
//this is an old setLeds function from lab 6
//void setLeds() {
//  for (x = 0; x < 4; x++) {
//    if (stepState[0][x] == true) {
//      //digitalWrite(ledPin[x], HIGH);
//    } else if (stepState[0][x] == false) {
//      //digitalWrite(ledPin[x], LOW);
//    }
//  }
//}
