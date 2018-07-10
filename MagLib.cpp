#include "MagLib.h"

MagLib::MagLib()
{

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
	for (int i = 2; i < 9; i++) {
		buffer[i] = receiveBuffer[i + 1];
	}

	_device2.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) {
		buffer[i + 6] = receiveBuffer[i + 1];
	}

	_device3.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) {
		buffer[i + 12] = receiveBuffer[i + 1];
	}

	_device4.ReadMeasurement(receiveBuffer, zyxt);
	for (int i = 2; i < 9; i++) {
		buffer[i + 18] = receiveBuffer[i + 1];
	}
}
void MagLib::init16Nodes(uint32_t addressPackage, char *receiveBuffer, char zyxt, int *mux)
{
	
}

void MagLib::read16Nodes(char *buffer, char zyxt)
{
	
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

void MagLib::setMux(int *muxBus, int index)
{
	switch (index)
	{
	case 1:
		digitalWrite(_mux1[0], muxBus[0]);
		digitalWrite(_mux1[1], muxBus[1]);
		break;
	default:
		break;
	}
}
