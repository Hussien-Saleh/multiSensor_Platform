/*
 * @brief: Communication Messages Presentation Layer
 * @file: msg_layer.h
 * @details: "Request and Response Message Packets", A presentation layer for the interface between master and slave devices
 *			 to communicate with each other in meaningful way where the slaves respond according to the master requests.
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#ifndef MSG_LAYER_H_
#define MSG_LAYER_H_

#include <atmel_start.h>
#include "config.h"

typedef struct requestMsgClass_ requestMsg_; //request message packet structure
typedef struct responseMsgClass_ responseMsg_; //response message packet structure

/*
* Request and Response Command Types -> Interface Commands
* Integral part of the message packet required for the communication between the master and slave devices
*/

enum requestResponse_commandType_
{	
	// request messages sent from the master, the slave acts based on these commands from the master
	//..
	MIN_MASTER_REQUEST_CMD_ID = 0, //minimum master request command ID value: used for error handling of command ID values
	//..
	MASTER_REQUEST_SLAVE_MCU_GET_NUMBER_SENSORS = 1, //master request slave mcu to check the info of sensors
	MASTER_REQUEST_SLAVE_MCU_RESET = 2, //master request slave mcu unit to reset
	MASTER_REQUEST_SLAVE_MCU_BE_READY_TO_MEASURE = 3, //master request slave mcu to be ready to measure
	MASTER_REQUEST_SLAVE_MCU_ACTIVATE_SENSOR = 4, //master request slave mcu to activate sensor
	MASTER_REQUEST_SLAVE_MCU_GET_SENSOR_MAX_MEASURE_TIME = 5, //master request slave mcu to get maximum measurement time
	MASTER_REQUEST_SLAVE_MCU_TRIGGER_SENSOR = 6, //master request slave mcu to trigger sensor 
	MASTER_REQUEST_SLAVE_MCU_IS_SENSOR_MEASUREMENT_READY = 7, 
	MASTER_REQUEST_SLAVE_MCU_RETRIEVE_SENSOR_MEASUREMENT = 8, //master request slave mcu to retrieve sensor measurement
	MASTER_REQUEST_SLAVE_MCU_MEASURING_DONE_SLEEP = 9, //master request slave mcu to be done with measuring and go to sleep
	
	MAX_MASTER_REQUEST_CMD_ID = 10, //maximum master request command ID value: used for error handling of command ID values
	//..
/******************************************************************************************************************/
	
	//response messages sent from the sensor slave, responses have the most significant bit set
	//..
	MIN_SLAVE_RESPONSE_CMD_ID = 20, //minimum slave response command ID value: used for error handling of command ID values
	//..
	SLAVE_RESPONSE_MASTER_MCU_GET_NUMBER_SENSORS = 21, //slave responds to master "mcu to check the info of sensors"
	SLAVE_RESPONSE_MASTER_MCU_RESET = 22, //slave responds to master "mcu reset" 
	SLAVE_RESPONSE_MASTER_MCU_BE_READY_TO_MEASURE = 23, //slave responds to master "mcu to be ready to measure"
	SLAVE_RESPONSE_MASTER_MCU_ACTIVATE_SENSOR = 24, //slave responds to master "mcu to activate sensor"
	SLAVE_RESPONSE_MASTER_MCU_GET_SENSOR_MAX_MEASURE_TIME = 25, //slave responds to master "mcu to get maximum measurement time"
	SLAVE_RESPONSE_MASTER_MCU_TRIGGER_SENSOR = 26, //slave responds to master "mcu to trigger sensor"
	SLAVE_RESPONSE_MASTER_MCU_IS_SENSOR_MEASUREMENT_READY = 27,
	SLAVE_RESPONSE_MASTER_MCU_RETRIEVE_SENSOR_MEASUREMENT = 28, //slave responds to master "mcu to retrieve sensor measurement"
	SLAVE_RESPONSE_MASTER_MCU_MEASURING_DONE_SLEEP = 29, //slave responds to master "mcu to be done with measuring and go to sleep"
	
	MAX_SLAVE_RESPONSE_CMD_ID = 30, //maximum slave response command ID value: used for error handling of command ID values
	//..
};


enum measurementType_ {
	GENERIC = 0x00,
	VOLTAGE = 0x01, // Voltage Measurement (V), 3 bytes
	CURRENT = 0x02, // current measurement in (A), from yA(10^-24) to YA(10^24), 3 bytes
	BATTERY_LEVEL = 0x03, //remaining charge in any sort of energy storage (%), 1 byte
	LUMINANCE = 0x04, //Lux measurement from 0 to 167772 lux, in (lx), 3 bytes
	RELATIVE_HUMIDITY = 0x05, //relative humidity from 0 to 100% , (%), 1 byte
	PRESSURE = 0x06, //PRESSURE in yPa(10^-24) to YPa(10^24) (Pa), 3 bytes
	DISTANCE = 0x07, // Distance measurement from yM(10^-24) to YM(10^24), (m), 3 bytes
	GAS_CONCENTRATION = 0x08, //Concentration in ?g/m^3 , 3 bytes
	ACCELERATION = 0x09, //positive and negative acceleration in m/s^2, 3 bytes
	RADIOACTIVITY = 0x0A, //Radioactivity from yBq(10^-24) to YBq(10^24), (Bq), 3 bytes
	ANGULAR_VELOCITY = 0x0B, //signed from yRad/s to YRad/s, in (Rad/s), 3 bytes
	AREA = 0x0C, //unsigned in m^2, 3 bytes
	ELECTRIC_FIELD_STRENGTH = 0x0D, //unsigned in V/m, 3 bytes
	ENERGY = 0x0E, //unsigned in (J), 3 bytes
	FORCE = 0x0F, //signed in (N), 3 bytes
	FREQUENCY = 0x10, //signed in (Hz),3 bytes
	HEAT_FLUX_DENSITY = 0x11, //unsigned in (W/m^2), 3 bytes
	IMPULSE = 0x12, //signed (Ns), 3 bytes
	MAGNETIC_FIELD_STRENGTH = 0x13, // signed (A/M), 3 bytes
	MASS = 0x14, //unsigned (g), 3 bytes
	MASS_DENSITY = 0x15, //unsigned (g/m^3), 3 bytes
	MASS_FLOW = 0x16, //Signed (g/s), 3 bytes
	POWER = 0x17, //Signed (W), 3 bytes
	RESISTANCE = 0x18, //Signed (?)ohms, 3 bytes
	MAGNETIC_FLUX_DENSITY = 0x19, // Signed (T), 3 bytes
	TIME = 0x1A, //Signed (S), 3 bytes
	TORQUE = 0x1B, //Signed (Nm), 3 bytes
	VELOCITY = 0x1C, //Signed (m/s), 3 bytes
	VOLUME = 0x1D, // Unsigned m^3 , 3 bytes
	VOLUMETRIC_FLOW = 0x1E, //signed (m^3/ s), 3 bytes
	ANGLE = 0x1F, //Angle (0 - 360°), 2 bytes
	CONCENTRATION_OF_RADIOACTIVE_MATERIALS = 0x20, //Unsigned (Bq/m^3), 3 bytes
	RADIATION_DOSE = 0x21, //Signed (Sv), 3 bytes
	POSITION = 0x22, //Latitude and Longitude as decimal number, 8 bytes
	CURRENT_TIME = 0x23, //Unix Timestamp, 4 bytes
	TEMPERATURE = 0x24, //Temperature Measurement from -273.0 to 3276.7(°C), 2 bytes
	// and so on
};

/* Possible return codes response messages coming from the slave devices */
enum return_msgCodes_
{
	MSG_RET_OK = 0x00,//! OK ... command is understood and the slave microcontroller has successfully responded to the master request command with the right information
	MSG_RET_ERR = 0x01, //! A generic error occurred
	MSG_RET_SENSOR_NOT_AVAILABLE = 0x02,
	MSG_RET_NO_MEASUREMENT_AVAILABLE = 0x03,
	MSG_RET_SENSOR_BUSY_ONGOING_MEASUREMENT = 0x04,
	MSG_RET_INVALID_PACKET = 0x05,  //! If command is malformed, e.g. length = 0, invalid code.
	MSG_RET_FUNC_NOT_SUPPORTED = 0x06, //!The functionality is not supported by that implementation
};

/* Request packet message -> storage for the requested instructions */
struct requestMsgClass_
{
	uint8_t msgLength; //request message length (number of bytes following the message length(first byte) -> entire message packet length = (msg_length + 1) bytes
	uint8_t cmd; //request command ID value
	uint8_t sensorID; //requested sensor ID
	
	uint8_t sleepOption; //sleep option for the slave sensor unit (idle or power down modes)
	uint8_t sleepDuration_hours; //sleep time in hours 
	uint8_t sleepDuration_minutes; //sleep time in minutes
	uint8_t sleepDuration_seconds; //sleep time in seconds
};

/* Response packet message -> storage for the requested instructions */

struct responseMsgClass_
{	
	uint8_t msgLength; //response message length (number of bytes following the message length(first byte) -> entire message packet length = (msg_length + 1) bytes
	uint8_t cmd; //response command ID value
	uint8_t sensorID; //responded sensor ID
	uint8_t returnCode; //status return code from the slave microcontroller
	uint8_t numConnectedSensors; //number of sensors available on the slave mcu
	uint8_t dataType;
	uint8_t dataSize; //size of measurement data
	uint8_t data[MAX_SENSOR_DATA_SIZE]; //measurement data content
	uint8_t maxTime; //maximum measurement time for a sensor in milliseconds
	uint8_t dataTimestamp[EIGHT_BYTES]; // eight bytes: system timestamp milliseconds
};

#endif


