#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define DP 7

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define PHASE_ONE 1
#define PHASE_TWO 2

#define DIRECTIONS 4

const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int segmentSize = 8;
byte currentPhase = PHASE_ONE;

int readX;
int readY;
int currentSegment = DP;

const int minThreshold = 300;
const int maxThreshold = 700;
const int minCalibrationThreshold = 400;
const int maxCalibrationThreshold = 600;
long blinkTime = 300;
byte ledState = HIGH;

int lastSteadyState = HIGH;

long resetTime = 3000;
int currentSwitchState;
int lastSwitchState = LOW;
long debounceDelay = 50; 
long lastDebounceTime = 0;

long lastLedStateChangeTime = 0;
long lastPressTime;
long lastReleaseTime;
bool released = false;
bool move = false;


byte segments[segmentSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte movement[segmentSize][DIRECTIONS] = {
  { A, B, G, F },
  { A, B, G, F },
  { G, DP, D, E },
  { G, C, D, E },
  { G, C, D, E },
  { A, B, G, F },
  { A, B, D, F },
  { C, DP, DP, D }
};

const int segmentPinMap[segmentSize] = {
  4, 5, 6, 7, 8, 9, 10, 11
};

int segmentStates[segmentSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  for(int i = 0; i < segmentSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  Serial.begin(9600);
}


void loop(){

  currentSwitchState = digitalRead(pinSW);

  if( currentSwitchState != lastSwitchState ) {
    lastDebounceTime = millis();
    lastSwitchState = currentSwitchState;
  }

  if( millis() - lastDebounceTime > debounceDelay ) {
    
   if ( lastSteadyState == HIGH && currentSwitchState == LOW ) {
      
      if( currentPhase == PHASE_ONE ) {

        digitalWrite(segmentPinMap[currentSegment], segmentStates[currentSegment]);
        currentPhase = PHASE_TWO;
        lastPressTime = lastDebounceTime;
        released = false;

      }else {
        currentPhase = PHASE_ONE;    
      }
    }else if ( lastSteadyState == LOW && currentSwitchState == HIGH ) {
      released = true;
      lastReleaseTime = lastDebounceTime;
    }

    lastSteadyState = currentSwitchState;
  }

  if( currentPhase == PHASE_ONE ) {
    
    readX = analogRead(pinX);
    readY = analogRead(pinY);

    if ( readX > minCalibrationThreshold && readX < maxCalibrationThreshold && readY > minCalibrationThreshold && readY < maxCalibrationThreshold )
        move = true; 
    
    if( move == true ) {
      
      if( readX < minThreshold ) {
        currentSegment = movement[currentSegment][LEFT];
        move = false;
      
      }else if( readX > maxThreshold ) {
        currentSegment = movement[currentSegment][RIGHT];
        move = false;
      }

      if( readY < minThreshold ) {
        currentSegment = movement[currentSegment][DOWN];
        move = false;
      
      }else if( readY > maxThreshold ) {
        currentSegment = movement[currentSegment][UP];
        move = false;
      }
    }

    defaultState();

  }else if( currentPhase == PHASE_TWO ) {

    if( lastReleaseTime - lastPressTime > resetTime || (released == false && millis() - lastPressTime >= resetTime) ) {
      resetDisplay();        
    
    }else if ( released == true and lastReleaseTime - lastPressTime < resetTime ) {
      
      readY = analogRead(pinY);

      if( readY > maxThreshold ) {
        segmentStates[currentSegment] = HIGH;
        digitalWrite(segmentPinMap[currentSegment], HIGH);       
      
      }else if( readY < minThreshold ) {
        segmentStates[currentSegment] = LOW;
        digitalWrite(segmentPinMap[currentSegment], LOW);        
      }
    }
  }
  
  displaySegments();
}

void resetDisplay() {
  
  for(int i = 0; i < segmentSize; i++) {
    digitalWrite(segmentPinMap[i], LOW);
    segmentStates[i] = LOW;
  }
  currentSegment = DP;
  released = false;
  currentPhase = PHASE_ONE;

}

void displaySegments(){

  for(int i = 0; i < segmentSize; i++) {
    if( i != currentSegment )
      digitalWrite(segmentPinMap[i], segmentStates[i]);
  }

}

void defaultState() {

  long currentTime = millis();

  if( currentTime - lastLedStateChangeTime > blinkTime ) {
    digitalWrite(segmentPinMap[currentSegment], ledState);
    ledState = !ledState;
    lastLedStateChangeTime = currentTime;
  }

}
               