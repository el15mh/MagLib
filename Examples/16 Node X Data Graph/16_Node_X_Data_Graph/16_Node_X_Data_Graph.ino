#include <MagLib.h>

#define addressPacket 0x0C0D0E0F

MagLib device;

char buffer[NODE_16];

float xValues[16], oldValues[16];

int mux[2] = {30, 32};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Wire.begin();

  device.init16Nodes(addressPacket, buffer, 0xF, mux);

  //Serial.println("Reading data...");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  device.read16Nodes(buffer, 0xF);

  xValues[0] = (buffer[2]*256 + buffer[3]) * 0.00805;
  xValues[1] = (buffer[8]*256 + buffer[9]) * 0.00805;
  xValues[2] = (buffer[14]*256 + buffer[15]) * 0.00805;
  xValues[3] = (buffer[20]*256 + buffer[21]) * 0.00805;
  xValues[4] = (buffer[26]*256 + buffer[27]) * 0.00805;
  xValues[5] = (buffer[32]*256 + buffer[33]) * 0.00805;
  xValues[6] = (buffer[38]*256 + buffer[39]) * 0.00805;
  xValues[7] = (buffer[44]*256 + buffer[45]) * 0.00805;
  xValues[8] = (buffer[50]*256 + buffer[51]) * 0.00805;
  xValues[9] = (buffer[56]*256 + buffer[57]) * 0.00805;
  xValues[10] = (buffer[62]*256 + buffer[63]) * 0.00805;
  xValues[11] = (buffer[68]*256 + buffer[69]) * 0.00805;
  xValues[12] = (buffer[74]*256 + buffer[75]) * 0.00805;
  xValues[13] = (buffer[80]*256 + buffer[81]) * 0.00805;
  xValues[14] = (buffer[86]*256 + buffer[87]) * 0.00805;
  xValues[15] = (buffer[92]*256 + buffer[93]) * 0.00805;

  for (int i = 0; i < 16; i++) {
    if (xValues[i] > 100) xValues[i] = oldValues[i];
    oldValues[i] = xValues[i];
  }

  for (int i = 0; i < 15; i++) {
    Serial.print(xValues[i]);
    Serial.print(", ");
  }
  Serial.print(xValues[15]);
  Serial.println();

  delay(50);
}
