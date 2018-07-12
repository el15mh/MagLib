/*
 * EXAMPLE
 * 
 * Simple program to demonstrate 2 way communication with client app over bluetooth.
 * Tested using Teensy 3.6 with HC06 bluetooth module.
 * 
 * Author: Max Houghton
 * 12/07/18
 */

#include <SoftwareSerial.h>

SoftwareSerial BTserial(0, 1); // RX | TX

char c = ' ';
int led = 13;

void setup()  {
  Serial.begin(9600);
  
  // HC-05 default serial speed for commincation mode is 9600
  BTserial.begin(9600); 

  pinMode(led, OUTPUT);
}

void loop() {
if (BTserial.available())
  {  
      c = BTserial.read();
      Serial.write(c);
  
      if (c == '1')       digitalWrite(led, HIGH);
      else if (c == '0')  digitalWrite(led, LOW);
  }
  
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
  {
      c =  Serial.read();
      BTserial.write(c);  
  }
}
