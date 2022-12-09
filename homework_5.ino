#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

#define NONE 0
#define PRESSED 1
#define UP 2
#define DOWN 3
#define LEFT 4
#define RIGHT 5

long time_no_scroll = 700;
long block_begin;
byte light_power = 63;
byte matrix_intensity = 0;
const byte matrixSize = 8;

byte customChar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte emptyChar[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

byte index_cursor = 13;
bool block_on_enter_name = false;
bool block_on_diff = false;
bool block_on_lcd_contrast = false;
bool block_on_lcd_light = false;
bool block_on_matrix = false;

bool enter_button = false;
char name[3] = {'A', 'A', 'A'};
byte difficulty = 0;

String message_options[] = {"LET'S SNAKE!", "PRESS!"};
String game_menu_options[] = {"START GAME", "HIGHSCORE", "SETTINGS", "ABOUT", "HOW TO PLAY"};
String play_game_options[] = {"INFO", "LIVES"};
String settings_options[] = {"Enter name", "Difficulty", "LCD contrast", "LCD light", "Matrix light", "Sound", "BACK"};
String highscore_options[] = {"DAN", "ZOU", "ELI", "CAC", "MAC", "BACK"};
String about_options[] = {"Game name", "Github", "Made by", "BACK"};
String howtoplay_options[] = {"Simple", "BACK"};

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte switchPin = 2;
const byte lcd_light = 3;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

void start_game() {
  
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); // turns on LED at col, row
      
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); // turns off LED at col, row
      
    }
  }

}


const int XaxisPin = A1;
const int YaxisPin = A0;

class Joystick {

  private:
    int xValue;
    int yValue;
    static const int minThreshold = 300;
    static const int maxThreshold = 700;
    static const long debounceDelay = 50; 
    bool moveX;
    bool moveY;
    byte lastSwitchState;
    long lastDebounceTime;
    byte lastSteadyState;

  public:

    Joystick(const int XaxisPin, const int YaxisPin, const byte switchPin) {
      pinMode(XaxisPin, INPUT);
      pinMode(YaxisPin, INPUT);
      pinMode(switchPin, INPUT_PULLUP);
      moveX = true;
      moveY = true;
      lastSteadyState = HIGH;
      lastSwitchState = LOW;
      lastDebounceTime = 0;
    }

    bool move_UP(bool wanna_be_calibrated = false) {

      xValue = analogRead(XaxisPin);

      if( xValue > minThreshold + 100 && xValue < maxThreshold - 100)
          moveX = true;
      
      if( moveX == true ) {
        
        if( xValue < minThreshold ) {
          moveX = false;
          return true;
        }
      }
      return false;      
    }

    bool move_DOWN(bool wanna_be_calibrated = false) {
      
      xValue = analogRead(XaxisPin);

      if( xValue > minThreshold + 100 && xValue < maxThreshold - 100)
          moveX = true;
      
      if( moveX == true ) {
        
        if( xValue > maxThreshold ) {
          moveX = false;
          return true;
        }
      }
      return false;
    }

    bool move_LEFT(bool wanna_be_calibrated = false) {
      
      yValue = analogRead(YaxisPin);

      if( yValue > minThreshold + 100 && yValue < maxThreshold - 100)
          moveY = true;
      
      if( moveY == true ) {
        
        if( yValue > maxThreshold ) {
          moveY = false;
          return true;
        }
      }
      return false;
    }

    bool move_RIGHT(bool wanna_be_calibrated = false) {
      
      yValue = analogRead(YaxisPin);

      if( yValue > minThreshold + 100 && yValue < maxThreshold - 100)
          moveY = true;
      
      if( moveY == true ) {
        
        if( yValue < minThreshold ) {
          moveY = false;
          return true;
        }
      }
      return false;
    }

    bool pressButton() {

      bool pressed = false;
      byte currentSwitchState = digitalRead(switchPin);
  
      if( currentSwitchState != lastSwitchState ) {
        lastDebounceTime = millis();
        lastSwitchState = currentSwitchState;
      }

      if( millis() - lastDebounceTime > debounceDelay ) {
        
        if( lastSteadyState == HIGH && currentSwitchState == LOW ) {
          pressed = true;
        }
        lastSteadyState = currentSwitchState;
      }

      return pressed;
      
    }

} joystick(XaxisPin, YaxisPin, switchPin);

class Menu {
  private:
    Menu* previousMenu;      
    byte numberOfOptions;
    String* options;
    byte actualOptionIndex;
    Menu* nextMenu;

  public:

    Menu(byte numberOfOptions, String arr[]) {
      this->numberOfOptions = numberOfOptions;
      options = new String[numberOfOptions];
      actualOptionIndex = 0;

      for(int i = 0; i < numberOfOptions; i++) {
        options[i] = arr[i];
      }
    }

    void setActualOptionIndex(byte number) {
      actualOptionIndex = number;
    }

    void setPreviousMenu(Menu* menu) {
      previousMenu = menu;
    }

    void setNextMenu(Menu* menu) {
      nextMenu = menu;
    }

    void print(){
      for(int i = 0; i < numberOfOptions; i++) {
        Serial.println(options[i]);
      }
    }

    Menu* getNextMenu(){
      return nextMenu;
    }

    Menu* getPreviousMenu(){
      return previousMenu;
    }

    void setFirstRow(String option) {
      lcd.setCursor(0,0);
      lcd.print(option);      
    }

    void setSecondRow(String option) {
      lcd.setCursor(0,1);
      lcd.print(option);      
    }

    void setInitialMessage() {
      lcd.clear();
      setFirstRow(options[0]);
      setSecondRow(options[1]);
      actualOptionIndex = 1;      
    }

    void setRows(bool button = false, char ch = '#') {
        
        if (button == true && ch == 'A'){
          lcd.clear();
          if (this->actualOptionIndex == 0)
            setFirstRow(this->options[0] + ": SNAKE");
          if (this->actualOptionIndex == 1)
            setFirstRow(this->options[1] + " Dani780-C");
          if (this->actualOptionIndex == 2)
            setFirstRow("by Cirlan Daniel");
          setSecondRow("> BACK <");
        }
        else if (button == true && ch == 'H'){
          lcd.clear();
          setFirstRow("It's just snake!");
          setSecondRow("> BACK <");
        }
        else{
          if (button == true){
            lcd.clear();
            if(this->actualOptionIndex == 0) {
              block_on_enter_name = true;
              String n = "";
              for(int i = 0; i < 3; i++)
                n += name[i];

              Serial.println("Enter name: " + n);
              setFirstRow("Enter name: " + n);
              setSecondRow("> BACK <");
              lcd.setCursor(12, 0);
              index_cursor = 12;
            }
            else if(this->actualOptionIndex == 1) {
              block_on_diff = true;
              setFirstRow("Set diff. -    +");
              index_cursor = 11 + difficulty;
              lcd.setCursor(11, 0);

              for(int i = 11; i < 11 + difficulty; i++) {
                lcd.write(byte(1));
              }

              for(int i = 11 + difficulty; i < 15; i++) {
                lcd.write(byte(0));
              }              

              setSecondRow("> BACK <");
              lcd.setCursor(11 + difficulty, 0);                          
              //Serial.println("Difficulty: ");
            }
            else if(this->actualOptionIndex == 2) {
              //lcd_contrast();
              block_on_lcd_contrast = true;
              block_begin = millis();
              lcd.setCursor(17, 0);
              setFirstRow("Use potentiometer for LCD contrast");            
              Serial.println("LCD Contrast: ");
              setSecondRow("> PRESS TO GO BACK <");
            }
            else if(this->actualOptionIndex == 3) {
              //lcd_brightness();
              block_on_lcd_light = true;
              
              setFirstRow("  -  @@@@@@  +  ");
              
              index_cursor = 5;
              lcd.setCursor(5, 0);

              for(int val = 63; val < light_power; val += 32) {
                lcd.write(byte(1));
              }

              for(int val = light_power; val < 255; val += 32) {
                lcd.write(byte(0));
              }              

              setSecondRow("> BACK <");
              lcd.setCursor(5 + (light_power - 63) / 32, 0);

              Serial.println("LCD light: ");
            }
            else if(this->actualOptionIndex == 4) {
              
              block_on_matrix = true;
              //matrix_brightness();

              setFirstRow("  -  @@@@@@  +  ");
              
              index_cursor = 5;
              lcd.setCursor(5, 0);

              for(int i = 5; i < 5 + matrix_intensity; i++) {
                lcd.write(byte(1));
              }

              for(int i = 5 + matrix_intensity; i < 11; i++) {
                lcd.write(byte(0));
              }            

              setSecondRow("> BACK <");
              lcd.setCursor(5 + matrix_intensity, 0);

              Serial.println("Matrix brightness: ");
            }
            else if(this->actualOptionIndex == 5) {
              //sound();            
              Serial.println("Sound: ");
              setSecondRow("> BACK <");
            }
            //"Enter name", "Difficulty", "LCD contrast", "LCD brightness", "Matrix brightness", "Sound"
          }
          else{
            lcd.clear();
            if (actualOptionIndex == numberOfOptions - 1) {
              setFirstRow("> " + options[actualOptionIndex] + " <");
              setSecondRow("");          
            }
            else{
              if (actualOptionIndex % 2 == 0) {
                setFirstRow("> " + options[actualOptionIndex] + " <");
                setSecondRow(options[actualOptionIndex + 1]);
              }
              else{
                setFirstRow(options[actualOptionIndex - 1]);
                setSecondRow("> " + options[actualOptionIndex] + " <");
              }      
            }
          }
        }
    }

    void moveRowsUP() {
      if(actualOptionIndex > 0) {
        lcd.clear();
        if (actualOptionIndex / 2 == (actualOptionIndex - 1) / 2) {
          setFirstRow("> " + options[actualOptionIndex - 1] + " <");
          setSecondRow(options[actualOptionIndex]);
        }
        else{
          setFirstRow(options[actualOptionIndex - 2]);
          setSecondRow("> " + options[actualOptionIndex - 1] + " <");
        }
        
        actualOptionIndex--;
      }
    }

    void moveRowsDOWN() {
      if(actualOptionIndex < numberOfOptions - 1){
        lcd.clear();
        if (actualOptionIndex / 2 == (actualOptionIndex + 1) / 2) {
          setFirstRow(options[actualOptionIndex]);
          setSecondRow("> " + options[actualOptionIndex + 1] + " <");
        }
        else{
          setFirstRow("> " + options[actualOptionIndex + 1] + " <");
          if(actualOptionIndex < numberOfOptions - 2)
            setSecondRow(options[actualOptionIndex + 2]);
          else
            setSecondRow("");
        }

        actualOptionIndex++;
      }
    
    }

    bool back() {
      if(actualOptionIndex == numberOfOptions - 1 && options[numberOfOptions - 1] == "BACK")
        return true;
      return false;      
    }

    ~Menu() {
      delete[] options;
    }

} message(2, message_options),
  game_menu(5, game_menu_options),
  play_game(2, play_game_options), 
  settings(7, settings_options),
  highscore(6, highscore_options),
  about(4, about_options),
  howtoplay(2, howtoplay_options);

Menu* currentMenu;

void setPreviousAndNextMenu() {

  message.setPreviousMenu(nullptr);
  message.setNextMenu(&game_menu);

  play_game.setPreviousMenu(nullptr);
  play_game.setNextMenu(&game_menu);

  game_menu.setPreviousMenu(nullptr);
  game_menu.setNextMenu(&play_game);

  settings.setPreviousMenu(&game_menu);
  settings.setNextMenu(nullptr);

  highscore.setPreviousMenu(&game_menu);
  highscore.setNextMenu(&game_menu);

  about.setPreviousMenu(&game_menu);
  about.setNextMenu(nullptr);

  howtoplay.setPreviousMenu(&game_menu);
  howtoplay.setNextMenu(nullptr);

  currentMenu = &message;
  currentMenu->setInitialMessage();  
  //currentMenu->setRows();

  // Serial.println(message == message);
  // Serial.println(message == game_menu);
  // Serial.println(game_menu == message);
  // Serial.println(game_menu == game_menu);

}

byte followMove() {

  if(joystick.pressButton() == true) {
    return PRESSED;
  }

  if(joystick.move_UP() == true) {
    return UP;
  }

  if(joystick.move_DOWN() == true) {
    return DOWN;
  }

  if(joystick.move_LEFT() == true) {
    return LEFT;
  }

  if(joystick.move_RIGHT() == true) {
    return RIGHT;
  }

  return NONE;  

}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  setPreviousAndNextMenu();
  lcd.createChar(1, customChar);
  lcd.createChar(0, emptyChar);
  pinMode(lcd_light, OUTPUT);
  analogWrite(lcd_light, light_power);
  //currentMenu = currentMenu->getNextMenu();
  //currentMenu->print();
  lc.shutdown(0, false);
  lc.setIntensity(0, matrix_intensity);
  lc.clearDisplay(0);
}

void loop() {
  
  byte move = followMove();

  if (block_on_enter_name == true) {
    
    lcd.cursor();
    if(move == PRESSED){
      block_on_enter_name = false;
      lcd.noCursor();
    }
    else if (move == LEFT) {
      if(index_cursor > 12)
        index_cursor--;
      lcd.setCursor(index_cursor, 0);
    }
    else if (move == RIGHT) {
      if(index_cursor < 14)
        index_cursor++;
      lcd.setCursor(index_cursor, 0);
    }
    else if (move == UP) {
      if(name[index_cursor - 12] == 'Z')
        name[index_cursor - 12] = 'A';
      else
        name[index_cursor - 12]++;
      lcd.print(name[index_cursor - 12]);
      lcd.setCursor(index_cursor, 0);
    }
    else if (move == DOWN) {
      if(name[index_cursor - 12] == 'A')
        name[index_cursor - 12] = 'Z';
      else
        name[index_cursor - 12]--;
      lcd.print(name[index_cursor - 12]);
      lcd.setCursor(index_cursor, 0);
    }
  } 
  else if(block_on_diff == true) {
    
    if(move == PRESSED){
      block_on_diff = false;
    }
    else if (move == LEFT) {
      if(difficulty > 0){
        difficulty--;
        index_cursor = 11 + difficulty;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(0));
      }
    }
    else if (move == RIGHT) {
      if(difficulty < 4) {
        index_cursor = 11 + difficulty;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(1));
        difficulty++;
      }
    }
  }
  else if(block_on_lcd_contrast == true) {
    if(move == PRESSED){
      block_on_lcd_contrast = false;
    }
    else{
      if(millis() > block_begin + time_no_scroll){
        lcd.scrollDisplayLeft();
        block_begin = millis();
      }       
    }
  }
  else if(block_on_lcd_light == true) {
    
    if(move == PRESSED){
      block_on_lcd_light = false;
    }
    else if (move == LEFT) {
      if(light_power > 63){
        light_power -= 32;
        index_cursor = 5 + (light_power - 63) / 32;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(0));
      }
    }
    else if (move == RIGHT) {
      if(light_power < 255) {
        index_cursor = 5 + (light_power - 63 ) / 32;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(1));
        light_power += 32;
      }
    }
    analogWrite(lcd_light, light_power % 256);
  }
  else if(block_on_matrix == true) {
    
    if(move == PRESSED){
      block_on_matrix = false;
    }
    else if (move == LEFT) {
      if(matrix_intensity > 0){
        matrix_intensity--;
        index_cursor = 5 + matrix_intensity;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(0));
      }
    }
    else if (move == RIGHT) {
      if(matrix_intensity < 6) {
        index_cursor = 5 + matrix_intensity;
        lcd.setCursor(index_cursor, 0);
        lcd.write(byte(1));
        matrix_intensity++;
      }
    }
    if(matrix_intensity == 6)
      lc.setIntensity(0, 15);  
    else 
      lc.setIntensity(0, (matrix_intensity * 3) % 16);
  }

  if (move == PRESSED && block_on_enter_name == false && block_on_diff == false && block_on_lcd_contrast == false && block_on_lcd_light == false && block_on_matrix == false) {

    if (enter_button == true) {
      enter_button = false;
      currentMenu->setRows();      
    }
    else{
      
      if(currentMenu->back() == true) {
        currentMenu->setActualOptionIndex(0);
        currentMenu = currentMenu->getPreviousMenu();
      }
      else{

        if(currentMenu == &about) {
          currentMenu->setRows(true, 'A');
          enter_button = true;
        }
        else if(currentMenu == &howtoplay){
          currentMenu->setRows(true, 'H');
          enter_button = true;         
        }
        else if (currentMenu->getNextMenu() == nullptr){
          currentMenu->setRows(true);
          enter_button = true;             
        }
        else
          currentMenu = currentMenu->getNextMenu();
      }

      if(currentMenu == &play_game) {
        Serial.println("Let's play");
        currentMenu = &play_game;
      }
      if (enter_button == false){
        currentMenu->print();
        currentMenu->setRows();
      }
    }
    //currentMenu->print();
  }
  else if(move == UP && currentMenu != &message && enter_button == false) {
    currentMenu->moveRowsUP();
    if (currentMenu == &game_menu) {
      Menu* actual_next = currentMenu->getNextMenu();
      if(actual_next == &howtoplay) currentMenu->setNextMenu(&about);
      else if(actual_next == &about) currentMenu->setNextMenu(&settings);
      else if(actual_next == &settings) currentMenu->setNextMenu(&highscore);
      else if(actual_next == &highscore) currentMenu->setNextMenu(&play_game);
    }
    //(currentMenu->getNextMenu())->print();
  }
  else if(move == DOWN && currentMenu != &message && enter_button == false) {
    currentMenu->moveRowsDOWN();
    Menu* actual_next = currentMenu->getNextMenu();
    if(currentMenu == &game_menu){
      if(actual_next == &play_game) currentMenu->setNextMenu(&highscore);
      else if(actual_next == &highscore) currentMenu->setNextMenu(&settings);
      else if(actual_next == &settings) currentMenu->setNextMenu(&about);
      else if(actual_next == &about) currentMenu->setNextMenu(&howtoplay);
    }
    
    //(currentMenu->getNextMenu())->print();
  }

  start_game();

}
