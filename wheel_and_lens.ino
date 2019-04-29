
#include <Servo.h>

//character array for our serial input command stream
char command[6];
//what we get from the serial communication from the Pi
String serialInfo;

#define PIN_HEAD1 5
#define PIN_HEAD2 6

Servo wheel;
Servo lens;

void setup() {
  lens.attach(PIN_HEAD1);
  wheel.attach(PIN_HEAD2);
  Serial.begin(9600);
}


void moveCwise(){
  for(int i=180; i>=90; i -=1){ //clockwise
     lens.write(150);
     delayMicroseconds(10000);
   }
}

void moveCCwise(){
  for(int i=0; i<90; i +=1){ //counterclockwise
     lens.write(30); 
     delayMicroseconds(10000); 
  }
}

void stayStill() {
  for (int i=0; i<180; i+=1){ //stay in place: orig 90, but was slipping
        lens.write(92); 
        delayMicroseconds(100);
    }
}

void loop() {
  if(Serial.available() > 0) {

    //Read our serial input until our end packet
    serialInfo = Serial.readStringUntil('E');
    
  }

  //Split up our received command into a character array; will need this to check which motor to command
    serialInfo.toCharArray(command, 7);

    //Get our desired position ; note: current encoding assumption is M(1/2)D(P/N)XXXE, meaning position is a degree value
     if(command[1] == '4') {    // lens 
          if(command[3] == '1') { // open and close
              moveCCwise();
              moveCwise();
            }
          else {  // lens no move
            stayStill();  
          }
      }
      
      if (command[1] == '3') { // wheeling out nerf gun and putting it back in 
        if (command[3] == '1') { // out
          wheel.write(0);
        }
        else if (command[3] == '2') { // in
          wheel.write(180);
        }
        
        
      }
     
}

