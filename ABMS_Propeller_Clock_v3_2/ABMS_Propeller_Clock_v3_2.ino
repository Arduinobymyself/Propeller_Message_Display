/*
#####################################################################################
#   File:               ABMS_Propeller_Clock_v3_2.ino
#   Processor:          Arduino UNO, MEGA ou Teensy++ 2.0      
#   Language:           Wiring / C /Processing /Fritzing / Arduino IDE          
#           
#   Objectives:         To demonstrating the Persistence Of Vision (POV) effect by creating
#                       a LED gadget showing rotative messages  
#                     
#   Behavior:           By sinchronized rotating the LEDs will show a message
#                       using POV
#                           
#   Download:           https://github.com/Arduinobymyself/Propeller_Message_Display
#     
#   Author:             Marcelo Moraes 
#   Date:               02/01/17  
#   place:              Brazil, Sorocaba City 
#         
#####################################################################################
 
This project contains public domain code.
The modification is allowed without notice.
  
*/
  
// defining the ASCII alphabet characters from the font.h library
#include "font.h"

// macros from DateTime.h 
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  


  
  
// define the Arduino LED pins
const int LEDpins[] = {2,3,4,5,6,7,8,9,10,11}; //only 8 pins, 
                                               //pins 2 and 11 is for the inner and outter ring
  
int rows = 8;		        // Total LEDs in a row
const int charHeight = 8;
const int charWidth = 5;
  
const int _povDelay = 10; //persistence of vision need 1/10 of second
const int _delay = 150; //delay for letters

//***************************************************
//put your message here
char textString[] = "Clock ";
//***************************************************

// sensor setup
const int sensorPin = 12;  // define the Arduino sensor pin
                           // the sensor is a Reed Switch with pull down resistor 
                           // connected to the +5Vdc
                             
int sensVal;  // variable to store the value coming from the sensor (not used)
  
int i;
int State = 0;
int lastState = 0;

//These three variables defines the begining hour:minute:second
int _seconds = 00;
int _minutes = 25;
int _hours = 16;

long previousMillis = 0;
long currentMillis = 0;
long interval = 1000;

void setup(){
  Serial.begin(9600);
  //reed switch sensor pin connected at pin 12 and is an input
  pinMode(sensorPin,INPUT);
  // all other pins are outputs (2,3,4,5,6,7,8,9,10,11)
  // the stick has 10 LEDs (8 for letters and 2 for the inner and outter ring)
  for (i = 0; i <=11; i++){
    pinMode(LEDpins[i],OUTPUT);    
  }  
}//end of setup

  
void loop(){
  long val = millis()/1000;
  //int days = elapsedDays(val);
  //int hours = numberOfHours(val);
  //int minutes = numberOfMinutes(val);
  //_seconds = numberOfSeconds(val);

  currentMillis = millis();

  if(currentMillis - previousMillis >= 1000){
    previousMillis = currentMillis;
    _seconds = _seconds + 1;
  }
  
  incrementTime();

  String timeString = String(fixZero(_hours))+" : "+String(fixZero(_minutes))+" : "+String(fixZero(_seconds));
  String tmpStr = textString+timeString;
  //Serial.println(tmpStr);
  int strLen = tmpStr.length();
  
  digitalWrite(2,HIGH);
  digitalWrite(11,HIGH);

  State = digitalRead(sensorPin); //get the sensor state
  if(State != lastState){//if sensor state changed
    if(State == HIGH){//if sensor state is high
      delayMicroseconds(_povDelay); //POV delay 1/10 of second
      //writes the message
      for (int k=strLen-1; k>-1; k--){
        printNormalLetter(tmpStr[k]);
      }
    }
    lastState = State;
  }
  //delayMicroseconds(_povDelay); //POV delay 1/10 of second
}


String fixZero(int i){
  String ret;
  if(i < 10) ret += "0";
  ret += i;
  return ret;
}


void incrementTime(){
 if(_seconds >= 59){
    _seconds = 0;
    _minutes = _minutes+1;
    if(_minutes >= 59){
      _minutes = 0;
      _hours = _hours+1;
      if(_hours >= 23){          
        _hours = 0;
      }
    }
  }
}


//normal and forwards printted letters  
void printNormalLetter(char ch){
  // make sure the character is within the alphabet bounds (defined by the font.h file)
  // if it's not, make it a blank character
  if (ch < 32 || ch > 126){
    ch = 32;
  }
  // subtract the space character (converts the ASCII number to the font index number)
  ch -= 32;
  // step through each byte of the character array
  for (int i=charWidth-1; i>-1; i--) {//width = 5 so i=4 to 0
    byte b = font[ch][i];
    for (int j=0; j<charHeight; j++) {//height = 8 so j=0 to 7
      digitalWrite(LEDpins[j+1], bitRead(b,j));
    }
    delayMicroseconds(_delay);
  }
  //clear the LEDs
  for (i = 0; i < rows; i++){
    digitalWrite(LEDpins[i+1] , LOW);
  }
  // space between letters
  delayMicroseconds(_delay);
}  

//inverted (flipped) and backwards printed letters
void printInvertedLetter(char ch){
  // make sure the character is within the alphabet bounds (defined by the font.h file)
  // if it's not, make it a blank character
  if (ch < 32 || ch > 126){
    ch = 32;
  }
  // subtract the space character (converts the ASCII number to the font index number)
  ch -= 32;
  // step through each byte of the character array
  for (int i=0; i<charWidth; i++) {
    byte b = font[ch][i];
    for (int j=0; j<charHeight; j++) {
       digitalWrite(LEDpins[j+1], bitRead(b, 7-j));
     }
     delayMicroseconds(_delay);
   }
   //clear the LEDs
   for (i = 0; i < rows; i++){
    digitalWrite(LEDpins[i+1] , LOW);
   }
   // space between letters
   delayMicroseconds(_delay);
}
