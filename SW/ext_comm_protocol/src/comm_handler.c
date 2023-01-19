/*
 * @brief: External Communication Interface Handling framework
 * @file: comm_handler.c
 * @details: communication framework handling the communication interface between the master and slave devices
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#include "ext_comm_protocol/include/comm_handler.h"

/*******************************************************************************************************/
/*	   Communication Handlers for the communication between master and slave microcontroller units	   */
/*******************************************************************************************************/

void comm_handler_mcu_ctor_ (mcu_ *me, int16_t addr, uint8_t sleepOption, uint8_t sleepHrs, uint8_t sleepMins, uint8_t sleepSecs){
	
	me->slaveAddress = addr; //slave address of slave microcontroller unit used on the bus
	me->sleepMode= sleepOption; //sleep modes: power-down or idle
	me->sleepHours = sleepHrs;
	me->sleepMinutes = sleepMins;
	me->sleepSeconds = sleepSecs;
	me->totalSensorsAvailable = 0;
	me->numberSensorsChecked = false;
	me->isMcuReady = false;
	me->isMcuAsleep = false;
	me->isMcuReset = false;
}
/*******************************************************************************************************/
uint8_t comm_handler_get_num_sensors_on_mcu_(mcu_ *me){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//initialize request message parameters
	request.msgLength = ONE_BYTE;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_GET_NUMBER_SENSORS;
	
	//check if request transmission is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response retrieving is completed
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//if the return status is successful, store the number of channels available on the unit
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_GET_NUMBER_SENSORS){
				me->totalSensorsAvailable = response.numConnectedSensors;
				if(me->totalSensorsAvailable > 0){		
					me->numberSensorsChecked = true;
					return RET_SUCCESS;
				}			
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_mcu_reset_(mcu_ *me){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//initialize request message parameters
	request.msgLength = ONE_BYTE;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_RESET;
	//check if request transmission is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response retrieving is completed
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//if the return status is successful, set the mcu reset flag
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_RESET){
				me->isMcuReset = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_mcu_be_ready_(mcu_ *me){

	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	//initialize request message parameters
	request.msgLength = ONE_BYTE;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_BE_READY_TO_MEASURE;
	
	//check if request transmission is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response retrieving is completed
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
		    //if the return status is successful, set the mcu is ready flag
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_BE_READY_TO_MEASURE){
				me->isMcuReady = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_mcu_sleep_(mcu_ *me){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//initialize request message parameters
	request.msgLength = FIVE_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_MEASURING_DONE_SLEEP;
	request.sleepOption = me->sleepMode;
	request.sleepDuration_hours = me->sleepHours;
	request.sleepDuration_minutes = me->sleepMinutes;
	request.sleepDuration_seconds = me->sleepSeconds;
	
	//check if transmission is completed with the request parameters
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if retrieving is completed and store the response
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_MEASURING_DONE_SLEEP){	
				me->isMcuAsleep = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
void comm_handler_sensor_ctor_(mcu_* me, uint8_t sensorNumber){
	
	me->sensors[sensorNumber].sensorIDNumber = sensorNumber; 
	me->sensors[sensorNumber].isSensorActivated = false;
	me->sensors[sensorNumber].isMaxMeasureTimeChecked = false;
	me->sensors[sensorNumber].isMeasurementTriggered = false;
	me->sensors[sensorNumber].isMeasurementReady = false; 
	me->sensors[sensorNumber].isMeasurementRetrieved = false;
	me->sensors[sensorNumber].measurementType = 0;
	me->sensors[sensorNumber].measurementSize = 0;
	me->sensors[sensorNumber].maxMeasurementTime = 0;
	memset(me->sensors[sensorNumber].measurementData, 0, MAX_SENSOR_DATA_SIZE);
	memset(me->sensors[sensorNumber].measurementTimestamp, 0, EIGHT_BYTES); //initialize timestamp to zeros at the creation of channel
}
/*******************************************************************************************************/
uint8_t comm_handler_activate_sensor_(mcu_ * me, uint8_t sensorNumber){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//initialize request message parameters
	request.msgLength = TWO_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_ACTIVATE_SENSOR;
	request.sensorID = sensorNumber;

	//check if transmission of the request parameters is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if the response retrieving is completed
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_ACTIVATE_SENSOR){
				me->sensors[sensorNumber].sensorIDNumber = response.sensorID;
				me->sensors[sensorNumber].isSensorActivated = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;	
}
/*******************************************************************************************************/
uint8_t comm_handler_sensor_get_max_measuring_time_(mcu_ * me, uint8_t sensorNumber){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//request message parameters
	request.msgLength = TWO_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_GET_SENSOR_MAX_MEASURE_TIME;
	request.sensorID = sensorNumber;
	
	//check if transmission is completed with the requested message
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response message is retrieve and save the response
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_GET_SENSOR_MAX_MEASURE_TIME){
				if(response.maxTime <= 255){
					
					me->sensors[sensorNumber].sensorIDNumber = response.sensorID;
					me->sensors[sensorNumber].maxMeasurementTime = response.maxTime;
					//memcpy(me->sensors[sensorNumber].maxMeasurementTime, response.maxTime, response.msgLength - THREE_BYTES);
					me->sensors[sensorNumber].isMaxMeasureTimeChecked = true;
					return RET_SUCCESS;
				}
				
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_sensor_trigger_measurement_(mcu_ *me, uint8_t sensorNumber){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//request message parameters
	request.msgLength = TWO_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_TRIGGER_SENSOR;
	request.sensorID = sensorNumber;
	
	//check if transmission is completed with the requested message
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response message is retrieve and save the response
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_TRIGGER_SENSOR){
				me->sensors[sensorNumber].sensorIDNumber = response.sensorID;
				me->sensors[sensorNumber].isMeasurementTriggered = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_is_sensor_measurement_ready_(mcu_ * me, uint8_t sensorNumber){
	
	requestMsg_ request;//temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
		
	//request message parameters
	request.msgLength = TWO_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_IS_SENSOR_MEASUREMENT_READY;
	request.sensorID = sensorNumber;
		
	//check if transmission of the request is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if the response retrieving is completed and store the response
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_IS_SENSOR_MEASUREMENT_READY){
				me->sensors[sensorNumber].sensorIDNumber = response.sensorID;
				me->sensors[sensorNumber].isMeasurementReady = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t comm_handler_sensor_retrieve_measurement_(mcu_ *me, uint8_t sensorNumber){
	
	requestMsg_ request; //temporary buffer to transmit the master request
	responseMsg_ response; //temporary buffer to retrieve the slave response
	
	//request message parameters
	request.msgLength = TWO_BYTES;
	request.cmd = MASTER_REQUEST_SLAVE_MCU_RETRIEVE_SENSOR_MEASUREMENT;
	request.sensorID = sensorNumber;
	
	//check if transmission of request message is completed
	if(comm_m_transmit_msg_(&request, me->slaveAddress) == COMM_OK_TX_COMPLETE){
		//check if response is retrieved and store the response data
		if(comm_m_retrieve_msg_(&response, me->slaveAddress) == COMM_OK_RX_COMPLETE){
			//check if retrieving is completed and store the response
			if(response.cmd == SLAVE_RESPONSE_MASTER_MCU_RETRIEVE_SENSOR_MEASUREMENT){
				//store the sensor channel index back to the caller
				me->sensors[sensorNumber].sensorIDNumber = response.sensorID;
				//store the measurement type back to the caller
				me->sensors[sensorNumber].measurementType = response.dataType;
				//store the sensor channel measurement timestamp back to the caller
				memcpy(me->sensors[sensorNumber].measurementTimestamp, response.dataTimestamp, EIGHT_BYTES);
				//store the measurement size back to the caller
				me->sensors[sensorNumber].measurementSize = response.dataSize;
				//store the measurement data back to the caller
				memcpy(me->sensors[sensorNumber].measurementData, response.data, response.dataSize);
				me->sensors[sensorNumber].isMeasurementRetrieved = true;
				return RET_SUCCESS;
			}
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
