#include "MagLib.h"

MagLib::MagLib()
{
	Serial.begin(9600);
	Wire.begin();

#ifdef CORE_TEENSY
	Serial.println("Using teensy");
#endif // CORE_TEENSY

}

MagLib::~MagLib()
{

}

void MagLib::initSingleNode(uint16_t address, char *buffer, char zyxt)
{
	_device1.init(receiveBuffer, address);
	_device1.configure(receiveBuffer);
	_device1.startBurstMode(receiveBuffer, zyxt);

	buffer[0] = receiveBuffer[0];	// Status byte
}

void MagLib::readSingleNode(char *buffer, char zyxt)
{
	unsigned long time = millis();
	int i;

	buffer[0] = (time & 0xFF00) >> 8;	// T msb
	buffer[1] = (time & 0xFF);			// T lsb

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (i = 2; i < 8; i++) buffer[i] = receiveBuffer[i + 1];
}

void MagLib::initFourNode(uint32_t addressPackage, char *_receiveBuffer, char zyxt)
{
	Serial.begin(9600);
	Wire.begin();

	changeI2CBus(0);

	_Address1 = addressPackage & 0xFF;
	_Address2 = (addressPackage & 0xFF00) >> 8;
	_Address3 = (addressPackage & 0xFF0000) >> 16;
	_Address4 = (addressPackage & 0xFF000000) >> 24;

	_device1.init(receiveBuffer, _Address1);
	_device1.configure(receiveBuffer);
	_device1.startBurstMode(receiveBuffer, zyxt);

	_device2.init(receiveBuffer, _Address2);
	_device2.configure(receiveBuffer);
	_device2.startBurstMode(receiveBuffer, zyxt);

	_device3.init(receiveBuffer, _Address3);
	_device3.configure(receiveBuffer);
	_device3.startBurstMode(receiveBuffer, zyxt);

	_device4.init(receiveBuffer, _Address4);
	_device4.configure(receiveBuffer);
	_device4.startBurstMode(receiveBuffer, zyxt);
}

void MagLib::readFourNodes(char *buffer, char zyxt)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];
}
void MagLib::init16Nodes(uint32_t addressPackage, char *buffer, char zyxt, int *mux)
{
	Serial.begin(9600);
	Wire.begin();

	changeI2CBus(0);

	char _buffer[1024];
	char* format = "Init 16 nodes on MUX pins: %i %i\n";

	sprintf(_buffer, format, mux[0], mux[1]);
	Serial.print(buffer);

	// Set MUX lines on Arduino board
	pinMode(mux[0], OUTPUT);
	pinMode(mux[1], OUTPUT);

	// Initialise all individual devices
	setMux(LOW, LOW);
	initFourNode(addressPackage, receiveBuffer, zyxt);

	// Change mux lines and repeat
	setMux(LOW, HIGH);
	initFourNode(addressPackage, receiveBuffer, zyxt);

	setMux(HIGH, LOW);
	initFourNode(addressPackage, receiveBuffer, zyxt);

	setMux(HIGH, HIGH);
	initFourNode(addressPackage, receiveBuffer, zyxt);
}

void MagLib::read16Nodes(char *buffer, char zyxt)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb

	setMux(LOW, LOW);

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 24] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 30] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 36] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 42] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 48] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 54] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 60] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 66] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);

	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 72] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 78] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 84] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 90] = receiveBuffer[i + 1];
}

void MagLib::init32Nodes(uint32_t addressPackage, char *receiveBuffer, char zyxt, int *mux)
{
	changeI2CBus(0);

	// Init 16 nodes as normal
	init16Nodes(addressPackage, receiveBuffer, zyxt, mux);

	// Change to next I2C Bus
	changeI2CBus(1);

	// Init remaining 16 nodes
	init16Nodes(addressPackage, receiveBuffer, zyxt, mux);
}

void MagLib::read32Nodes(char *buffer, char zyxt)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb
	
	changeI2CBus(0);

	setMux(LOW, LOW);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 24] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 30] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 36] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 42] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 48] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 54] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 60] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 66] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 72] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 78] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 84] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 90] = receiveBuffer[i + 1];

	changeI2CBus(1);

	setMux(LOW, LOW);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 96] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 102] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 108] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 114] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 120] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 126] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 132] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 138] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 144] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 150] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 156] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 162] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);
	_device1.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 168] = receiveBuffer[i + 1];

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++)	buffer[i + 174] = receiveBuffer[i + 1];

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 180] = receiveBuffer[i + 1];

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) buffer[i + 184] = receiveBuffer[i + 1];
}

void MagLib::printRawData(char *buffer, int format, int size)
{
	switch (format) {
		case BIN:
			//Serial.printf("Size of buffer: %u\n", sizeof(buffer));
			for (int i = 0; i < size; i++) {
				Serial.print(buffer[i], BIN);
			}
			Serial.print("\n");
			break;

		case HEX:
			for (int i = 0; i < size; i++) {
				Serial.print(buffer[i], HEX);
				//Serial.print(" ");
			}	// for loop
			Serial.print("\n");
			break;

		default:
			break;
	}	// switch (format)

}	// printRawData()

void MagLib::printASCIIData(char *buffer, int size)
{
	int ID = buffer[0];
	//int status = buffer[1];

	float Bx = (buffer[2] * 256) + buffer[3] * 0.00805f;
	float By = (buffer[4] * 256) + buffer[5] * 0.00805f;
	float Bz = (buffer[6] * 256) + buffer[7] * 0.02936f;

	Serial.printf("ID: %d, x: %.2f, y: %.2f, z: %.2f\n", ID, Bx, By, Bz);
}

void MagLib::initSDCard(int chipSelect)
{
	if (!SD.begin(chipSelect)) {
		Serial.print("SD Card init failed.");
		SDCard = false;
		return;
	}

	SDCard = true;

	char fileTitle[1024];
	const char *format = "data_%i.txt";
	int count = 0;

	sprintf(fileTitle, format, count);
	
	while (1) {
		if (SD.exists(fileTitle)) {
			count++;
			sprintf(fileTitle, format, count);
		}
		else break;
	}

	file = SD.open(fileTitle, FILE_WRITE);

	//Serial.println("SD Card init complete.");
}

void MagLib::printToSDCard(char *buffer, int size)
{
	if (!SDCard) {
		Serial.println("SD Card error.");
		return;
	}

	for (int i = 0; i < size; i++) {
		file.printf("%x\t", buffer[i]);
	}
	file.printf("\n");
	
}	// printToSDCard()

void MagLib::closeSDCard()
{
	file.close();

	Serial.println("SD Card closed.");
}

void MagLib::setMux(int S1, int S0)
{
	/*
	Serial.print("Changing mux: S0 = ");
	Serial.print(S0);
	Serial.print(", S1 = ");
	Serial.print(S1);
	Serial.println();
	*/

	delay(10);

	digitalWrite(_mux[0], S0);
	digitalWrite(_mux[1], S1);
}

void MagLib::changeI2CBus(int bus)
{
	switch (bus)
	{
	case 0:
		Wire.setSDA(SDA0);
		Wire.setSCL(SCL0);
		break;
	case 1:
		Wire.setSDA(SDA1);
		Wire.setSCL(SCL1);
		break;
	case 2:
		Wire.setSDA(SDA2);
		Wire.setSCL(SCL2);
		break;
	case 3:
		Wire.setSDA(SDA3);
		Wire.setSCL(SCL3);
		break;
	default:
		break;
	}
}

void MagLib::TimeMeasurement(float TimeTaken)
{
	Serial.println(TimeTaken, DEC);
}