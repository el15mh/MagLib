#include <MagLib.h>

#define addressPacket 0x0C0D0E0F

MagLib device;

char buffer[NODE_16];
int mux[2] = {30, 32};

void setup() {
  // put your setup code here, to run once:

  device.init16Nodes(addressPacket, buffer, 0xF, mux);
}

void loop() {
  // put your main code here, to run repeatedly:
  device.read16Nodes(buffer, 0xF);
  device.printRawData(buffer, HEX, NODE_16);

  delay(50);
}
