/*
 * EXAMPLE
 * 
 * Simple program to demonstrate sending data over bluetooth.
 * Tested using Teensy 3.6 with HC06 bluetooth module.
 * 
 * Author: Max Houghton
 * 12/07/18
 */

#include <SoftwareSerial.h>

// HC06 - RX->0, TX->1
SoftwareSerial BT(1, 0);

void setup() {
  BT.begin(9600);
}

void loop() {
  int value = random(0x00, 0xFF);
  BT.println(value);

  delay(20);
}

