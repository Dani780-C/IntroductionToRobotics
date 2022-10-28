const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;

const int redAnalogInPin = A0;
const int greenAnalogInPin = A1;
const int blueAnalogInPin = A2;

int redAnalogInValue = 0;
int greenAnalogInValue = 0;
int blueAnalogInValue = 0;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;

const int MAX_ANALOG_INPUT = 1023;
const int MIN_ANALOG_INPUT, MIN_ANALOG_OUTPUT = 0;
const int MAX_ANALOG_OUTPUT = 255;

void setup() {

  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

}

void loop() {

  redAnalogInValue = analogRead(redAnalogInPin);
  greenAnalogInValue = analogRead(greenAnalogInPin);
  blueAnalogInValue = analogRead(blueAnalogInPin);  
  
  redBrightness = map(redAnalogInValue, MIN_ANALOG_INPUT, MAX_ANALOG_INPUT, MIN_ANALOG_OUTPUT, MAX_ANALOG_OUTPUT); 
  greenBrightness = map(greenAnalogInValue, MIN_ANALOG_INPUT, MAX_ANALOG_INPUT, MIN_ANALOG_OUTPUT, MAX_ANALOG_OUTPUT);
  blueBrightness = map(blueAnalogInValue, MIN_ANALOG_INPUT, MAX_ANALOG_INPUT, MIN_ANALOG_OUTPUT, MAX_ANALOG_OUTPUT);  

  analogWrite(redLedPin, redBrightness);
  analogWrite(greenLedPin, greenBrightness);
  analogWrite(blueLedPin, blueBrightness);

}
