#include "MagLib.h"

MagLib::MagLib()
{

}

MagLib::~MagLib()
{

}

void MagLib::initCommunication(int i2cLine)
{
	Serial.begin(9600);

	switch (i2cLine) {
	case 0:
		Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
		Wire.setDefaultTimeout(200000);
		break;

	case 1:
		Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_37_38, I2C_PULLUP_EXT, 400000);
		Wire1.setDefaultTimeout(200000);
		break;

	case 2:
		Wire2.begin(I2C_MASTER, 0x00, I2C_PINS_3_4, I2C_PULLUP_EXT, 400000);
		Wire2.setDefaultTimeout(200000);
		break;

	case 3:
		Wire3.begin(I2C_MASTER, 0x00, I2C_PINS_56_57, I2C_PULLUP_EXT, 400000);
		Wire3.setDefaultTimeout(200000);
		break;

	default:
		break;
	}
}

void MagLib::initSingleNode(uint16_t address, char *buffer, char zyxt, int i2cLine)
{
	initCommunication(i2cLine);

	device1.init(receiveBuffer, address, i2cLine);
	device1.configure(receiveBuffer, i2cLine);
	device1.startBurstMode(receiveBuffer, zyxt, i2cLine);

	buffer[0] = receiveBuffer[0];	// Status byte
}

void MagLib::readSingleNode(char *buffer, char zyxt)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;	// T msb
	buffer[1] = (time & 0xFF);			// T lsb

	device1.ReadMeasurement(receiveBuffer, zyxt, 0);

	buffer[2] = receiveBuffer[3];
	buffer[3] = receiveBuffer[4];
	buffer[4] = receiveBuffer[5];
	buffer[5] = receiveBuffer[6];
	buffer[6] = receiveBuffer[7];
	buffer[7] = receiveBuffer[8];
}

void MagLib::initFourNode(uint32_t addressPackage, char *_receiveBuffer, char zyxt, int i2cLine)
{
	initCommunication(i2cLine);

	_address1 = addressPackage & 0xFF;
	_address2 = (addressPackage & 0xFF00) >> 8;
	_address3 = (addressPackage & 0xFF0000) >> 16;
	_address4 = (addressPackage & 0xFF000000) >> 24;

	device1.init(receiveBuffer, _address1, i2cLine);
	device1.configure(receiveBuffer, i2cLine);
	device1.startBurstMode(receiveBuffer, zyxt, i2cLine);

	device2.init(receiveBuffer, _address2, i2cLine);
	device2.configure(receiveBuffer, i2cLine);
	device2.startBurstMode(receiveBuffer, zyxt, i2cLine);

	device3.init(receiveBuffer, _address3, i2cLine);
	device3.configure(receiveBuffer, i2cLine);
	device3.startBurstMode(receiveBuffer, zyxt, i2cLine);

	device4.init(receiveBuffer, _address4, i2cLine);
	device4.configure(receiveBuffer, i2cLine);
	device4.startBurstMode(receiveBuffer, zyxt, i2cLine);
}

void MagLib::readFourNodes(char *buffer, char zyxt, int i2cLine)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb

	device1.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];
}

void MagLib::init16Nodes(uint32_t addressPackage, char *buffer, char zyxt, int *mux, int i2cLine)
{
	initCommunication(i2cLine);

	// Set MUX lines on Arduino board
	pinMode(mux[0], OUTPUT);
	pinMode(mux[1], OUTPUT);

	// Initialise all individual devices
	setMux(LOW, LOW);
	initFourNode(addressPackage, receiveBuffer, zyxt, i2cLine);

	// Change mux lines and repeat
	setMux(LOW, HIGH);
	initFourNode(addressPackage, receiveBuffer, zyxt, i2cLine);

	setMux(HIGH, LOW);
	initFourNode(addressPackage, receiveBuffer, zyxt, i2cLine);

	setMux(HIGH, HIGH);
	initFourNode(addressPackage, receiveBuffer, zyxt, i2cLine);
}

void MagLib::read16Nodes(char *buffer, char zyxt, int i2cLine)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb

	setMux(LOW, LOW);

	device1.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);

	device1.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 24] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++)	buffer[i + 30] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 36] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 42] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);

	device1.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 48] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++)	buffer[i + 54] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 60] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 66] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);

	device1.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 72] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++)	buffer[i + 78] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 84] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, i2cLine);
	for (int i = 2; i < 9; i++) buffer[i + 90] = receiveBuffer[i + 1];
}

void MagLib::init32Nodes(uint32_t addressPackage, char *receiveBuffer, char zyxt, int *mux)
{
	initCommunication(0);
	initCommunication(1);

	// Init first 16 nodes as normal
	init16Nodes(addressPackage, receiveBuffer, zyxt, mux, 0);

	// Init remaining 16 nodes
	init16Nodes(addressPackage, receiveBuffer, zyxt, mux, 1);
}

void MagLib::read32Nodes(char *buffer, char zyxt)
{
	unsigned long time = millis();

	buffer[0] = (time & 0xFF00) >> 8;			// T msb
	buffer[1] = (time & 0xFF);					// T lsb

	// Reading first I2C bus

	setMux(LOW, LOW);
	device1.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++)	buffer[i + 6] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 12] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 18] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);
	device1.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 24] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++)	buffer[i + 30] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 36] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 42] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);
	device1.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 48] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++)	buffer[i + 54] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 60] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 66] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);
	device1.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 72] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++)	buffer[i + 78] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 84] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 0);
	for (int i = 2; i < 9; i++) buffer[i + 90] = receiveBuffer[i + 1];

	// swap i2c lines

	setMux(LOW, LOW);
	device1.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 96] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++)	buffer[i + 102] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 108] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 114] = receiveBuffer[i + 1];

	setMux(LOW, HIGH);
	device1.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 120] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++)	buffer[i + 126] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 132] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 138] = receiveBuffer[i + 1];

	setMux(HIGH, LOW);
	device1.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 144] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++)	buffer[i + 150] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 156] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 162] = receiveBuffer[i + 1];

	setMux(HIGH, HIGH);
	device1.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 168] = receiveBuffer[i + 1];

	device2.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++)	buffer[i + 174] = receiveBuffer[i + 1];

	device3.ReadMeasurement(receiveBuffer, zyxt, 1);
	for (int i = 2; i < 9; i++) buffer[i + 180] = receiveBuffer[i + 1];

	device4.ReadMeasurement(receiveBuffer, zyxt, 1);
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
	digitalWrite(_mux[0], S0);
	digitalWrite(_mux[1], S1);
}


void MagLib::TimeMeasurement(float TimeTaken)
{
	Serial.println(TimeTaken, DEC);
}
