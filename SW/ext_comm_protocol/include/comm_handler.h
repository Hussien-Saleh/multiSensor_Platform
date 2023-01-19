/*
 * @brief: External Communication Interface Handling framework
 * @file: comm_handler.h
 * @details: communication framework handling the communication interface between the master and slave devices
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#ifndef COMM_HANDLER_H_
#define COMM_HANDLER_H_

#include <atmel_start.h>
#include "config.h"
#include "ext_comm_protocol/include/comm_protocol.h"

typedef struct sensorClass_ sensor_; //handler for the active object slave sensor
typedef struct mcuClass_ mcu_; //handler for the active object slave mcu

/* handler structure for continuous measurement storing and updating of the sensor status */
struct sensorClass_{
	
	uint8_t sensorIDNumber; //sensor index
	bool isSensorActivated; //flag to check if sensor is activated
	bool isMaxMeasureTimeChecked; //flag to check if max measurement time is checked
	bool isMeasurementTriggered; //flag to check if the measurement is triggered
	bool isMeasurementRetrieved; //flag to check if the measurement is retrieved
	bool isMeasurementReady;	
	uint8_t maxMeasurementTime;
	uint8_t measurementTimestamp[EIGHT_BYTES]; //storage for the retrieved measurement timestamp
	uint8_t measurementType;
	uint8_t measurementSize; //storage for the measurement data size
	uint8_t measurementData[MAX_SENSOR_DATA_SIZE]; //storage for the measurement data content
};

/*  handler structure for continuous updating of the microcontroller unit status */
struct mcuClass_{
	
	int16_t slaveAddress; //sensor microcontroller unit slave address
	uint8_t totalSensorsAvailable; //storage for the total connected sensors
	bool numberSensorsChecked; //flag to check if the number of sensors checked
	bool isMcuReady; //flag to check if the microcontroller unit is ready
	bool isMcuReset;
	bool isMcuAsleep; //flag to check if the microcontroller unit is asleep
	uint8_t sleepMode; //requested sleep option from master to slave
	uint8_t sleepHours; //requested sleep time in hours from master to slave
	uint8_t sleepMinutes; //requested sleep time in minutes from master to slave
	uint8_t sleepSeconds; //requested sleep time in seconds from master to slave
	sensor_ sensors[MAX_NUM_SENSORS];
};


/*******************************************************************************************************/
/*	   Communication Handlers for the communication between master and slave microcontroller units	   */
/*******************************************************************************************************/

/* Handling Constructor to initialize the microcontroller unit parameters
*  @param me, pointer to slave microcontroller unit
*  @param addr, slave address for the microcontroller unit
*  @param sleepMode, the desired sleep option for the microcontroller unit
*  @param sleepHrs, the desired sleeping hours for the microcontroller unit
*  @param sleepMins, the desired sleeping minutes for the microcontroller unit
*  @param sleepSecs, the desired sleeping seconds for the microcontroller unit
*/
void comm_handler_mcu_ctor_ (mcu_ *me, int16_t addr, uint8_t sleepOption, uint8_t sleepHrs, uint8_t sleepMins, uint8_t sleepSecs);				  

/* Function to get the number of sensors on the slave microcontroller unit
*  @param me, pointer to slave microcontroller unit
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_get_num_sensors_on_mcu_(mcu_ *me);

/* Function to reset the slave microcontroller unit
*  @param me, pointer to slave microcontroller unit
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_mcu_reset_(mcu_ *me);

/* Function to set the slave microcontroller unit into ready mode
*  @param me, pointer to slave microcontroller unit
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_mcu_be_ready_(mcu_ *me);

/* Function to set the slave microcontroller unit into sleep mode
*  @param me, pointer to slave microcontroller unit
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_mcu_sleep_(mcu_ *me);

/* Handling constructor to initialize the sensor parameters
*  @param me, pointer to slave sensor
*  @param sensorNumber, the sensor number
*/

void comm_handler_sensor_ctor_(mcu_*me, uint8_t sensorNumber);

/* Function to activate the sensor
*  @param me, pointer to slave microcontroller unit
*  @param sensorNumber, the sensor number
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/

uint8_t comm_handler_activate_sensor_(mcu_ *me, uint8_t sensorNumber);

/* Function to get the maximum measurement time for sensor
*  @param me, pointer to slave microcontroller unit
*  @param sensorNumber, the sensor number
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_sensor_get_max_measuring_time_(mcu_ *me, uint8_t sensorNumber);

/* Function to trigger the sensor measurement
*  @param me, pointer to slave microcontroller unit
*  @param sensorNumber, the sensor number
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error

*/
uint8_t comm_handler_sensor_trigger_measurement_(mcu_ *me, uint8_t sensorNumber);

/* Function to put the check if the sensor measurement is ready
*  @param me, pointer to slave microcontroller unit
*  @param sensorNumber, the sensor number
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/

uint8_t comm_handler_is_sensor_measurement_ready_(mcu_ *me, uint8_t sensorNumber);

/* Function to retrieve measurement from the channel
*  @param me, pointer to external sensor MCU unit
*  @param sensorNumber, the sensor number
*  @return (RET_SUCCESS) for process success or (RET_ERROR) for process error
*/
uint8_t comm_handler_sensor_retrieve_measurement_(mcu_ *me, uint8_t sensorNumber);

#endif 