# Introduction to Robotics (2022 - 2023)

Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

## Homework 1

### Tasks:
  * Components: RBG LED (1 minimum), potentiometers (3 minimum),
resistors and wires (per logic).  
  * Technical Task: Use a separat potentiometer in controlling each of the
color of the RGB led (Red, Green and Blue). The control must be done
with digital electronics (aka you must read the value of the potentiome-
ter with Arduino, and write a mapped value to each of the pins connected
to the led.  
  * Coding task: Coding style is of utmost importance. You must have a
perfectly clean code in order to receive the maximum grade. Keep in mind
that magic numbers are not accepted, although you might see them in the
lab (as a trade-off for speed). Remember to be consistent in your style,
check the style guide and the provided style document and use correct
spacing.  

 [Code](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/homework_1.ino) is right here!  
 A [video](https://www.youtube.com/shorts/NGv-BCZm_9E) of this build with its full functionality can be found on YouTube! 

What this home project looks like:  

 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_1_01.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_1_02.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_1_03.jpeg)  
 
 
## Homework 2

### Tasks:  
  * Components: 5 LEDs, 1 button, 1 buzzer, resistors and wires (per logic).  
  * General description: Building the traffic lights for a crosswalk. You will use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). See the states it needs to go through. If anything is not clear, ask. Also, see the uploaded video (the intervals are different, but the states flow is the same). It’s a traffic lights system for people and cars - don’t  overthink it. The system has the following states:  
    1. `State 1` (default, reinstated after state 4 ends): green light for cars, red light for people, no sounds. Duration: indefinite, changed  by pressing the button.  
    2. `State 2` (initiated by counting down 8 seconds after a button press): the light should be yellow for cars, red for people and no  sounds. Duration: 3 seconds.  
    3. `State 3` (initiated after state 2 ends): red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.  
    4. `State 4` (initiated after state 3 ends): red for cars, blinking green for people and a beeping sound from the buzzer, at a constant in-terval, faster than the beeping in state 3. This state should last 4 seconds.  
    Be careful: pressing the button in any state other than state 1 should NOT yield any actions.  
    
[Code](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/homework_2.ino) is right here!  
A [video](https://www.youtube.com/watch?v=SQXnwjndEao) of this build with its full functionality can be found on YouTube!  

What this home project looks like:  

 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_2_01.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_2_02.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_2_03.jpeg)  
 
 
## Homework 3

### Tasks:  
  * Components: 1 7-segment display, 1 joystick, resistors and wires (per
logic).  
  * General description: Use of the joystick to control the position of
the segment and ”draw” on the display. The movement between segments
is natural (meaning they jump from the current position
only to neighbors, but without passing through ”walls”.   

    The system has the following states:  
    1. `State 1` (default, but also initiated after a button press in State 2): Current position blinking. Can use the joystick to move from
one position to neighbors. Short pressing the button toggles state 2. Long pressing the button in state 1 resets the entire display by
turning all the segments OFF and moving the current position to the
decimal point.  
    2. `State 2` (initiated after a button press in State 1): The current
segment stops blinking, adopting the state of the segment before
selection (ON or OFF). Toggling the X (or Y, you chose) axis should
change the segment state from ON to OFF or from OFF to ON.
Clicking the joystick should save the segment state and exit back to
state 1.  

[Code](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/homework_3.ino) is right here!  
A [video](https://youtube.com/shorts/00I67F5Xatk) of this build with its full functionality can be found on YouTube!  

What this home project looks like:  

 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_3_01.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_3_02.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_3_03.jpeg)  
 

## Homework 4

### Tasks:  
  * Components: a joystick, a 4 digit 7-segment display, a 74hc595 shift
register.  
  * General Description: Use the joystick to move through the 4 digit 7
segment displays digits, press the button to lock in on the current digit
and use the other axis to increment or decrement the number. Keep the
button pressed to reset all the digit values and the current position to the
first digit in the first state.   

    The system has the following states:  
    1. `State 1`: you can use a joystick axis to cycle through the 4 digits;
using the other axis does nothing. A blinking decimal point shows
the current digit position. When pressing the button, you lock in on
the selected digit and enter the second state.  
    2. `State 2`: in this state, the decimal point stays always on, no
longer blinking and you can no longer use the axis to cycle through
the 4 digits. Instead, using the other axis, you can increment on
decrement the number on the current digit IN HEX (aka from 0
to F, as in the lab). Pressing the button again returns you to the
previous state. Also, keep in mind that when changing the number,
you must increment it for each joystick movement - it should not
work continuosly increment if you keep the joystick in one position
(aka with joyMoved).  
    3. Reset: toggled by long pressing the button only in the first state.
When resetting, all the digits go back to 0 and the current position
is set to the first (rightmost) digit, in the first state.  

[Code](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/homework_4.ino) is right here!  
A [video](https://www.youtube.com/shorts/x7hwTcd5USA) of this build with its full functionality can be found on YouTube!  

What this home project looks like:  

 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_4_01.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_4_02.jpeg)  
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_4_03.jpeg)  
 

## Homework 5  

[Code](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/homework_5.ino) is right here!  
A [video](https://www.youtube.com/watch?v=TlQCE4Fu_DY) of this build with its full functionality can be found on YouTube!  

What this home project looks like:  

 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_5_01.jpeg) 
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_5_02.jpeg) 
 
 ![Image](https://github.com/Dani780-C/IntroductionToRobotics/blob/main/images/homework_1/homework_5_03.jpeg) 
