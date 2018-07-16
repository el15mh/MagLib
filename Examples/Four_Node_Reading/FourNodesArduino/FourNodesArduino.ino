/*  EXAMPLE
 * 
 *  Reading a four MLX90393 devices using the MagLib library.
 *  
 *  Created by Belquis Haider, Max Houghton
 *  Date: 10/07/2018
 */

#include "MagLib.h"

#define NodeAddress 0x0C0D0E0F

MagLib device;

char buffer[NODE_FOUR];

void setup() {
  
  Serial.begin(115200);
  Wire.begin();
  
  device.initFourNode(NodeAddress,buffer,0xF);
}

void loop() {
    
    /* Read Measurements */
    device.readFourNodes(buffer,0xF);
    /* Print Raw Data */
    device.printRawData(buffer,BIN,26);

    delay(50);
   }

