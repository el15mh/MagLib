#include <i2c_t3.h>

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

char buffer[NODE_SINGLE];

void setup() {

  Serial.begin(9600);
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.setDefaultTimeout(200000);
  
  // put your setup code here, to run once: 
  device.initSingleNode(I2C_Address, buffer, 0xF);

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly
  device.readSingleNode(buffer, 0xF);
  device.printRawData(buffer, HEX, NODE_SINGLE);

  delay(200);
}
