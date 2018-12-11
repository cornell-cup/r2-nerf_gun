/*
Last updated: 2018f
Nerf Blaster control code
File must be in folder of the same name: .../nerfblaster/nerblaster.ino
Uses a joystick to control the position of the blaster. 
A separate button on a breadboard controlled blaster firing. 
Created by Jackie Woo 
*/

#include <Servo.h>

// servos
Servo servo1;
Servo servo2;

//joystick pins (analog)
int joyX = 0;
int joyY = 1;
int switchPin = 2; // was not implemented as of 2018f
int firePin = 9; // was not implemented as of 2018f
int lastSwitchState = HIGH; // was not implemented as of 2018f

void setup() {
  // attach pan and tilt servos
  servo1.attach(3);
  servo2.attach(5);
  Serial.begin(9600);
  
}

void loop() {

  /*
  Nerf blaster firing control
  */
  
  // Joystick position control
  int joyValX = analogRead(joyX); // up and down
  int joyValY = analogRead(joyY); // left and right

  int valX = map(joyValX, 0, 1023, 105, 75); // up and down
  int valY = map(joyValY, 0, 1023, 160, 60); // left and right

  servo1.write(valX); // up and down
  Serial.write("VALX: " + valX);
  
  servo2.write(valY); //left and right
  Serial.write("VALY: " + valY);
  delay(50);
 
  /*
  Note:
  Did not directly implement code to prevent servo from going back to 
  'zero' position. However, with the continuous servos that we did use, 
  the weight of the Nerf blaster was enough to stop it from resetting. 
  As of 2018f: only the tilt servo is the correct continuous servo that
  will be used in the final design. The pan servo still needs to be replaced
  by the same servo as the tilt servo. 
  */
  delay(15);
  
}
