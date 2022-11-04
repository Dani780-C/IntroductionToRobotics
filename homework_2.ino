#define PHASE_1 1
#define PHASE_2 2
#define PHASE_3 3
#define PHASE_4 4

unsigned long carGreenLedPeriodPhase2 = 8000;
unsigned long carYellowLedPeriodPhase2 = 12000;
unsigned long timeForPhaseThree = 8000;
unsigned long timeForPhaseFour = 4000;

int carRedLedPin = 12;
int carYellowLedPin = 11;
int carGreenLedPin = 10;

int pedestrianRedLedPin = 9;
int pedestrianGreenLedPin = 8;

int buzzerPin = 7;
int buttonPin = 2;
int buzzerTone = 350;

int buttonState = LOW;
int lastButtonState = LOW;

int lastDebounceTime = 0;
int debounceDelay = 20;

byte phase = PHASE_1;
unsigned long phaseSwitchTime = 0;
unsigned long soundTime = 0;
unsigned long blinkTime = 0;
unsigned int frequency = 450;
unsigned long duration = 500;
int soundOffPeriod = 1000;
int blinkOnPeriod = 500;


void setup(){

  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(carRedLedPin, OUTPUT);
  pinMode(carYellowLedPin, OUTPUT);
  pinMode(carGreenLedPin, OUTPUT);
  pinMode(pedestrianRedLedPin, OUTPUT);
  pinMode(pedestrianGreenLedPin, OUTPUT);

}


void loop(){

  int reading = digitalRead(buttonPin);

  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay && phase == PHASE_1) {

    if(reading != buttonState) {
      buttonState = reading;
      
      if(buttonState == LOW) {
        phase = PHASE_2;
        phaseSwitchTime = millis();
      }
  
    }
  
  }

  if(phase == PHASE_1) {
    
    digitalWrite(carGreenLedPin, HIGH);
    digitalWrite(carRedLedPin, LOW);
    digitalWrite(pedestrianRedLedPin, HIGH);
    digitalWrite(pedestrianGreenLedPin, LOW);
  
  }

  if(phase == PHASE_2) {
    
      if(millis() - phaseSwitchTime < carGreenLedPeriodPhase2) {
        digitalWrite(carGreenLedPin, HIGH);
      }
      else if(millis() - phaseSwitchTime < carYellowLedPeriodPhase2) {
        digitalWrite(carYellowLedPin, HIGH);
        digitalWrite(carGreenLedPin, LOW);
      }
      else {
        phaseSwitchTime = millis();
        phase = PHASE_3;
      }

  }

  if(phase == PHASE_3) {

    if(millis() - phaseSwitchTime < timeForPhaseThree) {
        
      if(soundTime < millis()) {
        tone(buzzerPin, frequency, duration);
        soundTime = millis() + soundOffPeriod;
      }

      digitalWrite(carYellowLedPin, LOW);
      digitalWrite(carRedLedPin, HIGH);  
      digitalWrite(pedestrianGreenLedPin, HIGH);      
      digitalWrite(pedestrianRedLedPin, LOW);
    
    } else {
      phaseSwitchTime = millis();
      soundTime = 0;
      phase = PHASE_4;
    }

  }


  if(phase == PHASE_4) {
    
    if(millis() - phaseSwitchTime < timeForPhaseFour) {
  
      if(soundTime < millis()) {
        tone(buzzerPin, frequency, duration);
        soundTime = millis() + soundOffPeriod / 2;
      }
      
      if(blinkTime < millis()) {
        digitalWrite(pedestrianGreenLedPin, HIGH);
        blinkTime = millis() + blinkOnPeriod;
      }
      
      if(blinkTime - blinkOnPeriod / 2 < millis()) {
        digitalWrite(pedestrianGreenLedPin, LOW);
      }

    } else {
      phase = PHASE_1;
      soundTime = 0;
      blinkTime = 0;
    }
    
  }

  lastButtonState = reading;
  
}