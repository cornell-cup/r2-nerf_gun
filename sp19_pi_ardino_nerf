#include <SPI.h>
#include <Servo.h>

#define MOTOR1_PIN 5
#define MOTOR2_PIN 6

boolean isInitAngle = true;
float initAngle = 0.00;

//int cnt = 0;
//float angles[3];

//character array for our serial input command stream
char command[6];
//what we get from the serial communication from the Pi
String serialInfo;

float posDesired1; // tilt position
float posDesired2; // pan position

Servo tilt;
Servo pan; 
//int speedDeg = 20;
int stayTime = 50000; //in microseconds

int relayPin = 3;

// Default pins from library are
// MOSI = 11; MISO = 12; CLK = 13
const int CS = 10; // Chip select

// commands from datasheet
const byte NOP = 0x00; // NO oPeration (NOP)
const byte READ_POS = 0x10; // Read position
const byte ZERO_PT = 0x70; // Zero set

// Tilt motor degree
// vars for getting positions (12-bit, so need to separate into 2 holders)
uint16_t ABSposition = 0;
uint16_t ABSposition_last = 0;
uint8_t pos_array[2];
float deg = 0.00;

// Pan motor degree
float currentPos = 90;
int speedDeg = 40;
int delayTime = 2000; // in microseconds (max 16000)

void setup()
{
  tilt.attach(MOTOR1_PIN); //5
  pan.attach(MOTOR2_PIN); //6
  //pinMode(relayPin, OUTPUT); //3
  pinMode( CS, OUTPUT ); // Slave Select
  digitalWrite( CS, HIGH ); // deactivate SPI device
  SPI.begin();
  SPI.setBitOrder( MSBFIRST );
  SPI.setDataMode( SPI_MODE0 );
  SPI.setClockDivider( SPI_CLOCK_DIV32 ); // sets SPI clock to 1/32 freq of sys CLK (ie, 16/32 MHz)
  Serial.begin( 9600 );
  Serial.println( "STARTING" );
  Serial.flush();
  delay( 2000 );
  SPI.end();

}

// ----------------- SPI FUNCTIONS ----------------------

uint8_t SPI_T ( uint8_t msg )    // Repetive SPI transmit sequence
{
   digitalWrite( CS, LOW );     // active SPI device
   uint8_t msg_temp = SPI.transfer( msg );    // send and recieve
   digitalWrite( CS, HIGH );    // deselect SPI device
   return( msg_temp );      // return recieved byte
}

void print_pos ( uint8_t position_array[] ) {
  position_array[0] &=~ 0xF0; // mask out first 4 bits (12-bit position) from two 8 bit #s
  ABSposition = pos_array[0] << 8;    // shift MSB to correct ABSposition in ABSposition message
  ABSposition += pos_array[1];    // add LSB to ABSposition message to complete message

  //Serial.print("ABS: ");
  //Serial.println(ABSposition);
  if ( ABSposition != ABSposition_last) //&& ( abs( ABSposition * 0.08789 - ABSposition_last * 0.08789 ) < 10 ) ) // if nothing has changed dont waste time sending position
  {
    ABSposition_last = ABSposition;    // set last position to current position
    deg = ABSposition;
    deg = deg * 0.08789;    // aprox 360/4096, 4096 because position is 12-bit (2^12 = 4096)

    // set inital position and angle array 
    
    if (isInitAngle) {
      initAngle = deg;
   
      isInitAngle = false;
    }
    
    //Serial.print("DEG: ");
    //S
    erial.println(deg);     // send position in degrees
  }   
  //pinMode (PWMPin, OUTPUT);
}

// -------------- END SPI FUNCTIONS ---------------------

// ------------ MOTOR MOVE FUNCT ------------------------
//-------------- PANNING --------------------------------
void setPos(float pos){
  float cwiseTime = 2 * abs((pos - 95)/0.0178);
  //Serial.println(cwiseTime);
  float ccwiseTime = 2 * abs((pos - 94.8)/(-0.0231));
  //Serial.println(ccwiseTime);

  if (89 <= pos && pos <= 91 || pos == currentPos) {
    //Serial.println("STAYING STILL1");
    stayStill();
  }
  else if (0 <= pos && pos < 89){// move cwise from 90
    //Serial.println("CCWISE");
    moveCCwisepan(ccwiseTime);
  }
  else if (180 >= pos && pos > 91){// move ccwise from 90
    //Serial.println("CWISE");
    moveCwisepan(cwiseTime);
  }
  else { //not in range, stay still
    //Serial.println("STAYING STILL2");
    stayStillpan();
  }
  currentPos = pos;
}

void moveTo(float angle){
  //Serial.println("RESET");
  reset(currentPos);
  //Serial.println("MOVE");
  setPos(angle);  
}

void reset(float pos){
  float comp = abs(90-pos);
  if (pos > 90) {
    //want to move clockwise back to 90
    setPos(90+comp);
  }
  if (pos < 90){
    setPos(comp);
  }
  else {
    setPos(90);
  }
}

void stayStillpan(){
 
  for (int i=0; i<180; i+=1){ //stay in place: orig 90, but was slipping
      pan.write(92); 
      delayMicroseconds(stayTime);
   }
}

void stayStillpan(float stayTime){
  for (int i=0; i<180; i+=1){ //stay in place: orig 90, but was slipping
      pan.write(92); 
      delayMicroseconds(stayTime);
   }
}

void moveCwisepan( float cwiseTime){

  for(int i=180; i>=90; i -=1){ //clockwise

     pan.write(speedDeg + 90);
     
     delayMicroseconds(cwiseTime);
   }
}

void moveCCwisepan(float ccwiseTime){

  for(int i=0; i<90; i +=1){ //counterclockwise

     pan.write(speedDeg); 
     delayMicroseconds(ccwiseTime); 
    }
}


//-------------- TILT --------------------------------
void moveCwise(){
  for(int i=180; i>=90; i -=1){ //clockwise
     tilt.write(110);
     //delayMicroseconds(1000);
   }
}

void moveCCwise(){
  for(int i=0; i<90; i +=1){ //counterclockwise
     tilt.write(70); 
     //delayMicroseconds(1000); 
  }
}


void stayStill(){
  for (int i=0; i<180; i+=1){ //stay in place: orig 90, but was slipping
      tilt.write(90); 
      //
      //delayMicroseconds(stayTime);
   }
}

void move(float pos){
  // for tilt
   if (deg > pos+2){
     moveCwise();
   }
   else if (deg < pos-2){
    moveCCwise();
   }
   else {
    stayStill();
   }
    // going through angles array
    /*
    cnt += 1;
    if (cnt > 2) cnt = 0;
   }
   */
  // end for tilt
  
}

// ------------ END MOTOR MOVE FUNCT --------------------


void loop()
{
  //if(Serial.available() > 0) {

    //Read our serial input until our end packet
    //serialInfo = Serial.readStringUntil('E');
    
  //}
  serialInfo = "M2DP180";
  //Split up our received command into a character array; will need this to check which motor to command
   serialInfo.toCharArray(command, 7);

    //Get our desired position ; note: current encoding assumption is M(1/2)D(P/N)XXXE, meaning position is a degree value
     if(command[1] == '1') { // tilt
          if(command[2] == 'D') {
            posDesired1 = (float)serialInfo.substring(4).toInt();
            //posDesired1 = convertMotor1Pos(posDesired1);
            if(command[3] == 'N') {
              posDesired1 = -1*posDesired1;
            }
          }
      }
      else if(command[1] == '2') { //pan 
        if(command[2] == 'D'){
            posDesired2 = (float)serialInfo.substring(4).toInt();
            //posDesired2 = convertMotor2Pos(posDesired2);
            if(command[3] == 'N') {
              posDesired2 = -1*posDesired2;
            }
        }
      }

   //----------- TILT --------------
   /*
      if (posDesired1 != 0) {
        while (deg != initAngle + posDesired1){
          // To read pos, datasheet lays out steps
          // 1. Master ends READ_POS. Encoder responds with an idle character
          // 2. Continue sending NO_OPERATION while encoder response is 0xA5
          // 3. If response is 0x10 (READ_POS), send NO_OPERATION & get MSB pos (lower 4 are upper 4 of 12 bit pos)
          // 4. Send 2nd NO_OPERATION cmd & get LSB pos (lower 8 bits of 12 bit pos)
           uint8_t recieved = 0xA5;    //just a temp variable
           ABSposition = 0;    //reset position variable
        
           SPI.begin();    // start transmition
           digitalWrite( CS, LOW ); // active up SPI device
           SPI_T( READ_POS );   // issue read command 
           recieved = SPI_T( NOP );    // issue NOP to check if encoder is ready to send
           while ( recieved != READ_POS )    // loop while encoder is not ready to send
           {
             recieved = SPI_T( NOP );    // check again if encoder is still working 
             delay(2);    // wait a bit (2 milliseconds)
           }
           pos_array[0] = SPI_T( READ_POS );    // recieve MSB
           pos_array[1] = SPI_T( READ_POS );    // recieve LSB
        
           digitalWrite( CS, HIGH );  // make sure SPI is deactivated  
        
           SPI.end();    // end transmition
           
           print_pos ( pos_array );
  
          if (!isInitAngle) {
            move(initAngle + posDesired1);
           }
           //move(posDesired1);
           
           delayMicroseconds(20);    // datasheet says 20us b/w reads is good
        }
      }
      */
   //---------------- PAN ------------------
     if (currentPos != posDesired2){
        moveTo(posDesired2);
        Serial.println("done pan");
     }
     else { stayStillpan(100); }
}

/*
void loop() //fire
{
  digitalWrite(relayPin, LOW);
  Serial.print("low");
  delay(5000);
  digitalWrite(relayPin, HIGH);
  Serial.println("high");
  delay(1000);
}
*/
/*
void loop() // pan
{
  stayStill();
  moveTo(0);
  stayStillpan(1000);
  moveTo(180);
  stayStillpan(1000);
  moveTo(180);
  stayStillpan(1000);
  moveTo(0);
  stayStillpan(1000);
}
*/

/*
void loop() // tilt
{ 
  //Serial.print("INIT: ");
  //Serial.println(initAngle);
  // To read pos, datasheet lays out steps
  // 1. Master ends READ_POS. Encoder responds with an idle character
  // 2. Continue sending NO_OPERATION while encoder response is 0xA5
  // 3. If response is 0x10 (READ_POS), send NO_OPERATION & get MSB pos (lower 4 are upper 4 of 12 bit pos)
  // 4. Send 2nd NO_OPERATION cmd & get LSB pos (lower 8 bits of 12 bit pos)
   uint8_t recieved = 0xA5;    //just a temp variable
   ABSposition = 0;    //reset position variable

   SPI.begin();    // start transmition
   digitalWrite( CS, LOW ); // active up SPI device
   SPI_T( READ_POS );   // issue read command 
   recieved = SPI_T( NOP );    // issue NOP to check if encoder is ready to send
   while ( recieved != READ_POS )    // loop while encoder is not ready to send
   {
     recieved = SPI_T( NOP );    // check again if encoder is still working 
     delay(2);    // wait a bit (2 milliseconds)
   }
   pos_array[0] = SPI_T( READ_POS );    // recieve MSB
   pos_array[1] = SPI_T( READ_POS );    // recieve LSB

   digitalWrite( CS, HIGH );  // make sure SPI is deactivated  

   SPI.end();    // end transmition
   
   print_pos ( pos_array );
   
   //if (!isInitAngle) {
     //move(angles[cnt]);
     
   //}

   move(posDesired1);
   delayMicroseconds(20);    // datasheet says 20us b/w reads is good

  //Firing
  
  //digitalWrite(relayPin,LOW);
  //delay(2000);
  //digitalWrite(relayPin, HIGH);
  //delay(100);
  
}
*/
