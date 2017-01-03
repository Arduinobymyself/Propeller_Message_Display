/*
#####################################################################################
#   File:               ABMS_Propeller_Clock_v3_3.ino
#   Processor:          Arduino UNO, MEGA ou Teensy++ 2.0      
#   Language:           Wiring / C /Processing /Fritzing / Arduino IDE          
#           
#   Objectives:         To demonstrating the Persistence Of Vision (POV) effect by creating
#                       a LED gadget showing rotative messages  
#                     
#   Behavior:           By sinchronized rotating the LEDs will show a message
#                       using POV (shows a message and digital clock
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
//defining the time library to get hours, minutes, seconds and date
#include <Time.h>
  
// define the Arduino LED pins
const int LEDpins[] = {2,3,4,5,6,7,8,9,10,11}; //only 8 pins, 
                                               //pins 2 and 11 is for the inner and outter ring

// Total LED's in a row
int rows= 8;

const int charHeight = 8;
const int charWidth = 5;
  
int povDelay = 10;    //persistence of vision need 1/10 of second
int charDelay = 400;  //delay for letters

int State = 0;
int lastState = 0;
int i;
  
// sensor setup
int sensorPin = 12;  // define the Arduino sensor pin
                           // the sensor is a Reed Switch with pull down resistor 
                           // connected to the +5Vdc

//***************************************************
//put your message here
char textString[] = "ABMS Clock";
//***************************************************


String tmp_str;
  
void setup(){
  //setting time and date initial values
  setTime(22,50,30,2,1,2013);

  //reed switch sensor pin connected at pin 12 and is an input
  pinMode(sensorPin,INPUT);

  // all other pins are outputs (2,3,4,5,6,7,8,9,10,11)
  // the stick has 10 LEDs (8 for letters and 2 for the inner and outter ring)
  for (i = 0; i <= 11; i++){
    pinMode(LEDpins[i], OUTPUT);    
  }  
}
  
void loop(){
  tmp_str=Clock();
  tmp_str=tmp_str+" ";
  
  digitalWrite(2,HIGH);
  digitalWrite(11,HIGH);

  State = digitalRead(sensorPin); //get the sensor state
  if(State != lastState){//if sensor state changed
    if(State == HIGH){//if sensor state is high
      delayMicroseconds(povDelay); //POV delay 1/10 of second
      //printting the clock
      for (int k=0; k<tmp_str.length(); k++){
        printInvertedLetter(tmp_str.charAt(k));
      }
      //delayMicroseconds(povDelay); //POV delay 1/10 of second
      //printting the text
      for(int k=sizeof(textString)-1; k>-1; k--){
        printNormalLetter(textString[k]);
      }
    }
    lastState = State;
  }
}

//function that calculates the time to show in the display
//and mkes the number < zero adjustment
String Clock(){
  String result;
  String str1;
  String str2;
  String str3;
  if (hour() < 10)
    str1 = "0"+String(hour());
  else
    str1 = String(hour());
  if (minute() < 10)
    str2 = "0"+String(minute());
  else
    str2 = String(minute());
  if (second() < 10)
    str3 = "0"+String(second());
  else
    str3 = String(second());
    result = str1+":"+str2+":"+str3;
  return result;
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
  for (int i=charWidth-1; i>-1; i--) {
    byte b = font[ch][i];
    // ventilator draai tegen de klok in
    for (int j=0; j<charHeight; j++) {
      digitalWrite(LEDpins[j+1], bitRead(b,j));
    }
    delayMicroseconds(charDelay);
  }
  //clear the LEDs
  for (i = 0; i < rows; i++){
    digitalWrite(LEDpins[i+1] , LOW);
  }
  // space between letters
  delayMicroseconds(charDelay);
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
    // ventilator draai tegen de klok in
    for (int j=0; j<charHeight; j++) {
      digitalWrite(LEDpins[j+1], bitRead(b, 7-j));
    }
    delayMicroseconds(charDelay);
  }
  //clear the LEDs
  for (i = 0; i < rows; i++){
    digitalWrite(LEDpins[i+1] , LOW);
  }
  // space between letters
  delayMicroseconds(charDelay);
}
