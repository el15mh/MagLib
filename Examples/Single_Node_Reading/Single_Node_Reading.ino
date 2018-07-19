/*  EXAMPLE
 * 
 *  Reading a single MLX90393 Device using the MagLib library.
 *  
 *  Created by Max Houghton
 *  Date: 10/07/2018
 */

#include "MagLib.h"

#define I2C_Address   0x0C

MagLib device; 

float Bx_new, Bx_old;

char buffer[NODE_SINGLE];

void setup() {
  // put your setup code here, to run once:
  device.initSingleNode(I2C_Address, buffer, 0xF);

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly
  device.readSingleNode(buffer, 0xF);
  //device.printRawData(buffer, HEX, NODE_SINGLE);

  Bx_new = (buffer[2] * 256 + buffer[3]) * 0.00805;
  float By = (buffer[4] * 256 + buffer[5]) * 0.00805;
  float Bz = (buffer[6] * 256 + buffer[7]) * 0.002936;

  if (Bx_new > 150) Bx_new = Bx_old;

  Serial.print(Bx_new);
  Serial.print("\t");
  Serial.print(By);
  Serial.print("\t");
  Serial.println(Bz);

  Bx_old = Bx_new;

  delay(20);
}
