#define PHASE_ONE 1
#define PHASE_TWO 2
#define RIGHTMOST_DISPLAY 3
#define LEFTMOST_DISPLAY 0

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register
const int switchPin = 2;

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;
const int numberOfDisplays = 4;

const int encodingsNumber = 16;
const int XaxisPin = A0;
const int YaxisPin = A1;
byte currentPhase = PHASE_ONE;

int readX;
int readY;

const int minThreshold = 300;
const int maxThreshold = 700;
const int minCalibrationThreshold = 400;
const int maxCalibrationThreshold = 600;
long blinkTime = 400;
byte dpLedState = HIGH;

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
bool moveX = false;
bool moveY = false;
int currentDisplay = 3;

int digitOnDisplay[numberOfDisplays] = {
  0, 0, 0, 0
};

int displayDigits[numberOfDisplays] = {
  segD1, segD2, segD3, segD4
};

int byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};


void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  for (int i = 0; i < numberOfDisplays; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);
}


void loop() {
  
  currentSwitchState = digitalRead(switchPin);
  
  if( currentSwitchState != lastSwitchState ) {
    lastDebounceTime = millis();
    lastSwitchState = currentSwitchState;
  }

  if( millis() - lastDebounceTime > debounceDelay ) {
    
   if( lastSteadyState == HIGH && currentSwitchState == LOW ) {
      
      if( currentPhase == PHASE_ONE ) {

        currentPhase = PHASE_TWO;
        lastPressTime = lastDebounceTime;
        released = false;
        dpLedState = HIGH;

      }else {
        currentPhase = PHASE_ONE;    
      }
      
    }else if( lastSteadyState == LOW && currentSwitchState == HIGH ) {
      released = true;
      lastReleaseTime = lastDebounceTime;
    }

    lastSteadyState = currentSwitchState;
  }

  if( currentPhase == PHASE_ONE ) {
    
    readX = analogRead(XaxisPin);

    if( readX > minCalibrationThreshold && readX < maxCalibrationThreshold )
        moveX = true;
    
    if( moveX == true ) {
      
      if( readX > maxThreshold ) {
        currentDisplay = ( currentDisplay + 1 ) % numberOfDisplays;
        moveX = false;
      
      }else if( readX < minThreshold ) {
        if( currentDisplay == LEFTMOST_DISPLAY ) {
          currentDisplay = RIGHTMOST_DISPLAY;
        }else {
          currentDisplay = currentDisplay - 1;
        }
        moveX = false;
      }
    }

    defaultState(currentDisplay);

  }else if( currentPhase == PHASE_TWO ) {

    writeOnDisplay(currentDisplay);
       
    if( lastReleaseTime - lastPressTime > resetTime || (released == false && millis() - lastPressTime >= resetTime) ) {
      resetDisplay();        
    
    }else if( released == true and lastReleaseTime - lastPressTime < resetTime ) {
      
      readY = analogRead(YaxisPin);

      if( readY > minCalibrationThreshold && readY < maxCalibrationThreshold )
        moveY = true;
    
      if( moveY == true ) {
      
        if( readY > maxThreshold ) {
          digitOnDisplay[currentDisplay] = (digitOnDisplay[currentDisplay] + 1) % 16;
          moveY = false;
      
        }else if( readY < minThreshold ) {
          if( digitOnDisplay[currentDisplay] == 0 ) {
            digitOnDisplay[currentDisplay] = 15;
          }else {
            digitOnDisplay[currentDisplay] = digitOnDisplay[currentDisplay] - 1;
          }
          moveY = false;
        }

      }
    }
  }

}

void resetDisplay() {
  
  for(int i = 0; i < numberOfDisplays; i++) {
    digitOnDisplay[i] = 0;  
  }

  currentDisplay = RIGHTMOST_DISPLAY;
  released = false;
  currentPhase = PHASE_ONE;
  dpLedState = LOW;
}

void defaultState(int currentDisplay) {

  writeOnDisplay(currentDisplay);

  long currentTime = millis();

  if( currentTime - lastLedStateChangeTime > blinkTime ) {
    dpLedState = !dpLedState;
    lastLedStateChangeTime = currentTime;
  }

}

void activateDisplay(int displayIndex) {

  for(int i = 0; i < numberOfDisplays; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayIndex], LOW);
}

void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void writeOnDisplay(int currentDisplay) {

  int displayIndex = 0;
  int currentDigit;

  for(int i = RIGHTMOST_DISPLAY; i >= LEFTMOST_DISPLAY; i--) {

    currentDigit = digitOnDisplay[i];
    
    if(i == currentDisplay) 
      writeReg(byteEncodings[currentDigit] + dpLedState);
    else
      writeReg(byteEncodings[currentDigit]);
    
    activateDisplay(displayIndex);
    displayIndex++;
    delay(5);
  }
  
}