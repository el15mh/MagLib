#ifndef MAGLIB_H
#define MAGLIB_H

#include <cstdint>
#include <SD.h>
#include <SPI.h>
#include "MLX90393.h"

#define BINARY 			0
#define HEXADECIMAL 	1
#define NODE_SINGLE   	8	// 2 + 6
#define NODE_FOUR     	26	// 2 + 6*4
#define NODE_16       	98	// 2 + 6*16
#define NODE_32       	194	// 2 + 6*32
#define NODE_64       	386	// 2 + 6*64

#ifdef __AVR_ATmega328__	// ARDUINO UNO
	#define SDA0		20
	#define SCL0		21
#endif

#ifdef __SAM3X8E__			// ARDUINO DUE
	#define SDA0		20
	#define SCL0		21
#endif

#ifdef CORE_TEENSY			// TEENSY BOARDS
	#define SDA0			18
	#define SCL0			19
	#define SDA1			38
	#define SCL1			37
	#define SDA2			4
	#define SCL2			3
	#define SDA3			56
	#define SCL3			57
#endif

/**	@file MagLib.h
	@brief Class for integration with arrays of MagOne sensors on the Arduino platform.
	@author Max Houghton
	@data 10/07/2018
*/
class MagLib
{
public:

	/**	Default constructor. Instanciates MagLib object.
	*/
	MagLib();

	/**	Default deconstructor
	*/
	~MagLib();

/* ********** SINGLE SENSOR CONTROL ********** */

	/**	Initialise single node sensor
	 	@param address I2C Address for sensor
	 	@param receiveBuffer Buffer to hold status byte
	 	@param zytx Selection byte to specify axes to read (0xE -> XYZ)
	 */
	void initSingleNode(uint16_t address, char *buffer, char zyxt);

	/** Read data current measured by the device.
		@param receiveBuffer Pointer to data packet (9 bytes -> Status + 2*(T+X+Y+Z)).
		@param zyxt Byte to specify which axes are to be read (1110 -> reading Z, Y and X).
	*/
	void readSingleNode(char *buffer, char zyxt);

/* ********** FOUR SENSOR CONTROL ********** */

	/** Initialise four node sensor array
	 	@param addressPackage I2C Addresses for sensors; 4x 8bit package
	 	@param receiveBuffer Buffer to hold status byte
	 	@param zytx Selection byte to specify axes to read (0xE -> XYZ)
	 */
	void initFourNode(uint32_t addressPackage, char *receiveBuffer, char zyxt);

	/** Read data current measured by the device.
		@param buffer Pointer to data packet.
		@param zyxt Byte to specify which axes are to be read (1110 -> reading Z, Y and X).
	*/
	void readFourNodes(char *buffer, char zyxt);

/* ********** 16 NODE SENSOR CONTROL ********** */

	/*	Initialise 16 node sensor array
		@param addressPackage I2C Addresses for sensors; 4x 8bit package
		@param receiveBuffer Buffer to hold status byte
		@param zytx Selection byte to specify axes to read (0xE -> XYZ)
	*/
	void init16Nodes(uint32_t addressPackage, char *receiveBuffer, char zyxt, int *mux);

	/** Read data current measured by the device.
		@param buffer Pointer to data packet.
		@param zyxt Byte to specify which axes are to be read (1110 -> reading Z, Y and X).
	*/
	void read16Nodes(char *buffer, char zyxt);

/* ********** 32 NODE SENSOR CONTROL ********** */

	/*	Initialise 32 node sensor array
	@param addressPackage I2C Addresses for sensors; 4x 8bit package
	@param receiveBuffer Buffer to hold status byte
	@param zytx Selection byte to specify axes to read (0xE -> XYZ)
	*/
	void init32Nodes(uint32_t addressPackage, char *receiveBuffer, char zyxt, int *mux);

	/** Read data current measured by 32 nodes.
	@param buffer Pointer to data packet.
	@param zyxt Byte to specify which axes are to be read (1110 -> reading Z, Y and X).
	*/
	void read32Nodes(char *buffer, char zyxt);
	

/* ********** GLOBAL FUNCTIONS ********** */

	/** Print raw data to serial port.
		@param buffer Packet of data containing info from sensors
		@param format Specify binary (0) or hexadecimal (1) data format.
		@param size Size of data packet.
	*/
	void printRawData(char *buffer, int format, int size);

	/** Print formatted data to serial port.
		@param buffer Packet of data containing info from sensors
		@param size	Size of data packet
	*/
	void printASCIIData(char *buffer, int size);

	/**	Initialise SD Card to record data from sensors
		@param chipSelect Pins on Arduino board connected to SD Card slot.
	*/
	void initSDCard(int chipSelect);

	/** Print data in a format accessible with various Bluetooth applications.
		@param buffer Packet of data containing info from sensors.
		@param size Size of data packet
	*/
	void printToSDCard(char *buffer, int size);

	/**	Close SD Card and stop writing to file.
	 */
	void closeSDCard();

	/**	Change the I2C bus used.
		@param pinSDA Specify the pin on the Arduino for I2C SDA line.
		@param pinSDA Specify the pin on the Arduino for I2C SCL line.
	*/
	void changeI2CBus(int bus);

	/** Print time taken to get 1000 readings - 4 Node
	*/
	void TimeMeasurement(float TimeTaken);

private:

	/**	Set digital output pins on Arduino connected to Multiplexer SELECT pins
		@param muxBus Array of HIGH/LOW values for SELECT pins
	 */
	void setMux(int S1, int S0);

	char receiveBuffer[9];	/** Buffer to receive raw data from each MLX device. */

	File file;			/** File object used to write data to SD card. */

	MLX90393 _device1;	/** MLX90393 device to take readings */
	MLX90393 _device2;	/** MLX90393 device to take readings */
	MLX90393 _device3;	/** MLX90393 device to take readings */
	MLX90393 _device4;	/** MLX90393 device to take readings */

	uint8_t _Address1;	/** First I2C address used for communcations */
	uint8_t _Address2;	/** Second I2C address used for communcations */
	uint8_t _Address3;	/** Third I2C address used for communcations */
	uint8_t _Address4;	/** Fourth I2C address used for communcations */

	int _mux[2];		/** Pins specifying single multiplexer bus [S1 S0] */

	bool SDCard;		/** Variable to ensure SD card is operating normally. */

};

#endif /* MAGLIB_H */
