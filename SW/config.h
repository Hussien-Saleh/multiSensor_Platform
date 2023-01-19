/*
 * @brief: User Configuration File
 * @file: config.h
 * @details: Configuration file to set user parameters
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#ifndef CONFIG_H_
#define CONFIG_H_

// Includes
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include <stdint.h>
#include <time.h>

#include <atmel_start.h>

#include "Serial_Debug/DEBUG_print.h"
/*******************************************************************************************************/
/*											Debug Configuration										   */
/*******************************************************************************************************/

#ifdef DEBUG_PRINT_H_
		#define DEBUG_print_setup() _DEBUG_print_setup()
		#define DEBUG_print_len(print_arr, str_len) _DEBUG_print_len(print_arr, str_len)
		#define DEBUG_print(print_arr) _DEBUG_print(print_arr)
		#define DEBUG_print_char(print_char) _DEBUG_print_char(print_char)
#else
		#define DEBUG_print_setup()
		#define DEBUG_print_len(print_arr, str_len)
		#define DEBUG_print(print_arr)
		#define DEBUG_print_char(print_char)
#endif

/*******************************************************************************************************/
/*											 Common Definitions											*/
/*******************************************************************************************************/
#define ONE_BYTE 1U
#define TWO_BYTES 2U
#define THREE_BYTES 3U
#define FOUR_BYTES 4U
#define FIVE_BYTES 5U
#define SIX_BYTES 6U
#define SEVEN_BYTES 7U
#define EIGHT_BYTES 8U
#define NINE_BYTES 9U
#define TEN_BYTES 10U
#define ELEVEN_BYTES 11U
#define TWELVE_BYTES 12U
#define THIRTEEN_BYTES 13U
#define FOURTEEN_BYTES 14U
#define FIFTEEN_BYTES 15U
#define SIXTEEN_BYTES 16U
#define SEVENTEEN_BYTES 17U
#define EIGHTEEN_BYTES 18U
#define NINETEEN_BYTES 19U
#define TWENTY_BYTES 20U

#define ENABLE 1U
#define DISABLE 0U
#define RET_SUCCESS 1U
#define RET_ERROR 0U
#define MASK_CHECK(X, Y) ((X & Y) == Y)

#define HOUR_TO_MILLI 3600000
#define MINUTE_TO_MILLI 60000
#define SECOND_TO_MILLI 1000

#define GENERIC_DATA_TYPE 0 //generic data type for onboard sensors

/*******************************************************************************************************/
/*									External Microcontroller Configuration							   */
/*******************************************************************************************************/

#define SLAVE_MCU_ADDRESS 0x06

#define MAX_NUM_SENSORS 4 //maximum number of sensors on slave board
#define MAX_SENSOR_DATA_SIZE TWELVE_BYTES

/* Sensors IDs */
enum sensorsIDs_{
	SENSOR_0 = 0,
	SENSOR_1 = 1,
	SENSOR_2 = 2,
	SENSOR_3 = 3,
	// and so on
};

//1: sensor is selected to be triggered, 0 is not selected
#define SELECTED 1
#define NOT_SELECTED 0
#define NUM_SELECTED_SENSORS 4 //has to be within the maximum number of available sensors
#define SENSOR_0_SELECTION_STATUS   SELECTED
#define SENSOR_1_SELECTION_STATUS   SELECTED 
#define SENSOR_2_SELECTION_STATUS   SELECTED 
#define SENSOR_3_SELECTION_STATUS   SELECTED 

typedef enum{
	
	SENSOR_0_MAX_MEASURE_COUNT = 3,
	SENSOR_1_MAX_MEASURE_COUNT = 6,
	SENSOR_2_MAX_MEASURE_COUNT = 7,
	SENSOR_3_MAX_MEASURE_COUNT = 4,
	// and so on
	
}sensorsMeasuringCounter_beforeMCUSleep;

//predefined time periods for triggering external sensors
typedef enum{
	
	SENSOR_0_TRIGGER_TIME = 9,
	SENSOR_1_TRIGGER_TIME = 6,
	SENSOR_2_TRIGGER_TIME = 4,
	SENSOR_3_TRIGGER_TIME = 7,
	// and so on
	
}sensorsTriggerTime_seconds;

//choice of sleep time of external slave microcontroller
#define SLEEP_HOURS 0
#define SLEEP_MINUTES 3
#define SLEEP_SECONDS 0

enum sleepModes_{
	SLEEP_IDLE_MODE = 1,
	SLEEP_POWERDOWN_MODE = 2,
};

enum timeUnits_{
	HOURS = 1,
	MINUTES = 2,
	SECONDS = 3,
	MILLIS = 4,
	MICROS = 5,
};

/*Dataset to retrieve sensor data content*/
typedef struct sensor_dataset_class_ sensor_dataset_;
struct sensor_dataset_class_{
	
	uint8_t active_sensor_idx; //1 byte
	uint8_t data_type; //1 byte
	uint8_t data_size; // 1 byte
	uint8_t timestamp[EIGHT_BYTES]; //8 bytes
	int8_t data[MAX_SENSOR_DATA_SIZE]; // varying number of bytes (for now.. 12 bytes max)
};
/*******************************************************************************************************/
/*										 Ring Buffer Configuration									   */
/*******************************************************************************************************/
#define MAX_DATASETS 16 //must be power of 2, maximum number of structure datasets in the ring buffer

#define TWENTY_FIVE_PERCENT 25U
#define FIFTY_PERCENT 50U
#define SEVENTY_FIVE_PERCENT 75U
 
#define ONE_PERCENT_OF_BUFFER 1U
#define TWENTY_FIVE_PERCENT_OF_BUFFER ((MAX_DATASETS * TWENTY_FIVE_PERCENT)/100)
#define FIFTY_PERCENT_OF_BUFFER ((MAX_DATASETS * FIFTY_PERCENT)/100)
#define SEVENTY_FIVE_PERCENT_OF_BUFFER ((MAX_DATASETS * SEVENTY_FIVE_PERCENT)/100)

#define NUM_BUFFER_ITEMS_BEFORE_DEQUEUE_TO_SD  ONE_PERCENT_OF_BUFFER

/*******************************************************************************************************/
/*										 SD Card Configuration									       */
/*******************************************************************************************************/
#define SAVE_TO_SD
#define NUMBER_OF_SENSOR_BYTE_VARIABLES  (MAX_SENSOR_DATA_SIZE + ELEVEN_BYTES)		// Defines the number of sensor variable bytes on SD Card
#define DATA_FILE_NAME_STRING_SIZE 11			// "DATA001.bin" = 11 chars
#define IMU_FILE_NAME_STRING_SIZE 10		// "IMU001.bin" = 10 chars

/*******************************************************************************************************/
/*										 IMU Configuration										       */
/*******************************************************************************************************/
#define IMU_SAMPLES 100 //
#define ONBOARD_IMU_SENSOR_ID 11

typedef struct IMU_DataSet{
	
	int32_t IMU_acc[IMU_SAMPLES][3];
	int32_t IMU_gyro[IMU_SAMPLES][3];
	
}IMU_DataSet;

/*******************************************************************************************************/
/*								  Temperature Sensor Configuration								       */
/*******************************************************************************************************/
#define MCP9809_I2C_AD 0x18U
#define ONBOARD_TEMP_SENSOR_ID 12
#define ONBOARD_TEMP_SENSOR_DATA_SIZE TWO_BYTES
#define ONBOARD_TEMP_SENSOR_DATA_TYPE GENERIC_DATA_TYPE

#define MCP9809_MAX_MEASURING_COUNTER 10


/*******************************************************************************************************/
/*										 GPS Configuration										       */
/*******************************************************************************************************/
#define ONBOARD_GPS_SENSOR_ID 13
#define ONBOARD_GPS_DATA_SIZE TWELVE_BYTES
#define ONBOARD_GPS_DATA_TYPE GENERIC_DATA_TYPE
#define GPS_MAX_BUF_LENGTH 15				// Size of GPS character buffer -> max length of receivable value
#define GPS_CHAR_ITERATION_LIMIT 500		// Number of char iterations in search for string in stream

#define GPS_MAX_MEASURING_COUNTER 5
/*******************************************************************************************************/
/*										  Error Code definitions								       */
/*******************************************************************************************************/

//GPS Errors
#define ERROR_GPS_GNGNS_NOT_FOUND 10
#define ERROR_GPS_NO_NUM_FOUND 11

//Board temperature Sensor Errors
#define ERROR_MCP9809_SENSOR_NOT_FOUND 30

//IMU Errors
#define ICM20648_ERROR_INVALID_DEVICE_ID 40


extern struct io_descriptor *I2C_INT_io;

#endif 