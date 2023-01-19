/*
 * @brief: Temperature Sensor MCP9809 Driver
 * @file: Temp_MCP9809.h
 * @details: Driver for temperature sensor
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#ifndef TEMP_MCP9808_H_
#define TEMP_MCP9808_H_

#include <atmel_start.h>
#include "config.h"
#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"

scheduled_task_description_ TempSensor_update_task;

/************************************************************************************************************/
/*								 Temperature Sensor MCP9808 register pointers						        */
/************************************************************************************************************/
#define MCP9809_WHO_AM_I 0x07U
#define MCP9809_DEVICE_ID 0x04U
#define MCP9809_TEMPERATURE_REG_TA 0x05U

#define MCP9809_CONFIG_REG 0x01U
/************************************************************************************************************/
/*											Function Prototypes												*/
/************************************************************************************************************/

void config_TempSensor_tasks_();
/************************************************************************************************************/
void task_config_TempSensor_update_();
/************************************************************************************************************/
void TempSensor_update_cb_(void* param, void* param_2);
/************************************************************************************************************/
/* 
	Function to read register value from Temperature Sensor 
	@param reg: register ID pointer
	@param bufp: pointer to the data storage buffer
	@param len: length of data to read in bytes
	@return the register value from I2C slave                                                          
*/
uint8_t Temp_read_reg_(uint8_t reg, uint8_t *bufp, uint16_t len);
/************************************************************************************************************/
/*  
	Function to initialize on board temperature sensor and heck if the device ID address is correct
	@return error message if the sensor is not found otherwise 0
*/
uint8_t setup_BoardTemp_();
/************************************************************************************************************/
/*  
	Function to read ambient temperature value from temperature register and 
	convert temperature to decimal value based on data sheet MCP9808				
*/
void update_BoardTemp_();
/************************************************************************************************************/


void mcp9809_lowpower(uint8_t status);
// bool ret_tempSensor_status();

#endif /* TEMP_MCP9808_H_ */