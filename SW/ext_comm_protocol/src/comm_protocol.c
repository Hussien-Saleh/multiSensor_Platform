/*
 * @brief: Transmitter-retriever interface for Low-level Asynchronous communication protocol (I2C)
 * @file: comm_protocol.c
 * @details: Transmitter and retriever interface for Asynchronous communication protocol between the master and slave devices
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/
#include "ext_comm_protocol/include/comm_protocol.h"

//static volatile packet_ pack = {0}; //pointer to the communication packet

commStatus_ comm_m_transmit_msg_(requestMsg_ *request, int16_t slaveAddress){
	
	packet_ pack = {0};
		
	//check if command ID is invalid
	if((request->cmd > MAX_MASTER_REQUEST_CMD_ID) || (request->cmd < MIN_MASTER_REQUEST_CMD_ID)){
		
		//DEBUG_print("\nERROR: Invalid request command\n");
		return COMM_INVALID_COMMAND;
	}	
	pack.outputBuffer[0] = request->msgLength; //number of data bytes following the length
	pack.outputBuffer[1] = request->cmd; //request command ID
	
	switch(request->cmd){
		
		case MASTER_REQUEST_SLAVE_MCU_ACTIVATE_SENSOR:
		case MASTER_REQUEST_SLAVE_MCU_GET_SENSOR_MAX_MEASURE_TIME:
		case MASTER_REQUEST_SLAVE_MCU_TRIGGER_SENSOR:
		case MASTER_REQUEST_SLAVE_MCU_IS_SENSOR_MEASUREMENT_READY:
		case MASTER_REQUEST_SLAVE_MCU_RETRIEVE_SENSOR_MEASUREMENT:
		
			pack.outputBuffer[2] = request->sensorID;
			break;
			
		case MASTER_REQUEST_SLAVE_MCU_MEASURING_DONE_SLEEP:
	
			pack.outputBuffer[2] = request->sleepOption;
			pack.outputBuffer[3] = request->sleepDuration_hours;
			pack.outputBuffer[4] = request->sleepDuration_minutes;
			pack.outputBuffer[5] = request->sleepDuration_seconds;
			break;
	}
	
	i2c_m_sync_set_slaveaddr(&I2C_INT, slaveAddress, I2C_M_SEVEN);
	int32_t numBytesWritten = io_write(I2C_INT_io, pack.outputBuffer, (uint16_t)(pack.outputBuffer[0] + ONE_BYTE));
	
	if((numBytesWritten == MIN_BUFFER_SIZE) || (numBytesWritten != (request->msgLength + ONE_BYTE))){
		//DEBUG_print("\nERROR: Invalid data packet request\n");
		return COMM_INVALID_DATA_PACKET;
	}
	if(numBytesWritten > MAX_BUFFER_SIZE){
		//DEBUG_print("\nERROR: Invalid data packet request, buffer quota exceeded\n");
		return COMM_INSUFFICIENT_BUFFER_SIZE;
	}
	//check if the number of bytes written is the same as the requested message packet entire length
	if(numBytesWritten == (request->msgLength + ONE_BYTE)){
		//DEBUG_print("\nRequest Message transmitted successfully\n");
		return COMM_OK_TX_COMPLETE;
	}
	else{	
		//DEBUG_print("\nERROR: Request Message did not transmit\n");
		return COMM_ERROR;	
	}
}
/*******************************************************************************************************/
commStatus_ comm_m_retrieve_msg_(responseMsg_*response, int16_t slaveAddress){
	
	packet_ pack = {0};
		
	i2c_m_sync_set_slaveaddr(&I2C_INT, slaveAddress, I2C_M_SEVEN);
	int32_t numBytesRead = io_read(I2C_INT_io, pack.inputBuffer, (uint16_t) MAX_BUFFER_SIZE);	
		
	//temporary buffer to store and interpret the retrieved data
	uint8_t *buffer = pack.inputBuffer;
	//store message length
	response->msgLength = *buffer;
	
	if(response->msgLength <= MIN_BUFFER_SIZE){
		return COMM_INVALID_DATA_PACKET;
	}
	if(response->msgLength >= MAX_BUFFER_SIZE){
		return COMM_INSUFFICIENT_BUFFER_SIZE;
	}
	//store command ID
	response->cmd = *(buffer+1); 		
	//store the return code
	response->returnCode = *(buffer+2);
	//check if the response command ID is valid
	if((response->cmd > MAX_SLAVE_RESPONSE_CMD_ID) || (response->cmd < MIN_SLAVE_RESPONSE_CMD_ID)){
		//DEBUG_print("\nERROR: Invalid response command\n");
		return COMM_INVALID_COMMAND;
	}	
	switch(response->returnCode){
		
		case MSG_RET_FUNC_NOT_SUPPORTED:
		
			//DEBUG_print("\nERROR: Response message: Function is not supported\n");
			return COMM_INVALID_COMMAND;
			
		case MSG_RET_SENSOR_NOT_AVAILABLE:
			
			//DEBUG_print("\nERROR: Response message: Sensor is not available\n");
			return COMM_INVALID_COMMAND;
			
		case MSG_RET_INVALID_PACKET:
	
			//DEBUG_print("\nERROR: Response message: Invalid packet\n");
			return COMM_INVALID_DATA_PACKET;
		
		case MSG_RET_NO_MEASUREMENT_AVAILABLE:
		
			//DEBUG_print("\nERROR: Response message: No measurement available\n");
			return COMM_ERROR;
			
		case MSG_RET_SENSOR_BUSY_ONGOING_MEASUREMENT:
		
			//DEBUG_print("\nERROR: Response message: Sensor Busy Ongoing Measurement\n");
			return COMM_ERROR;
			
		case MSG_RET_ERR:
			
			//DEBUG_print("\nERROR: Response message: Message is not understood\n");
			return COMM_ERROR;
	}
	
	switch(response->cmd){
			
		case SLAVE_RESPONSE_MASTER_MCU_GET_NUMBER_SENSORS:
			
			response->numConnectedSensors = *(buffer+3);
			break;
			
		case SLAVE_RESPONSE_MASTER_MCU_ACTIVATE_SENSOR:
		case SLAVE_RESPONSE_MASTER_MCU_IS_SENSOR_MEASUREMENT_READY:
		case SLAVE_RESPONSE_MASTER_MCU_TRIGGER_SENSOR:
		
			response->sensorID= *(buffer+3);
			break;
		
		case SLAVE_RESPONSE_MASTER_MCU_GET_SENSOR_MAX_MEASURE_TIME:
			
			response->sensorID= *(buffer+3);
			response->maxTime = *(buffer+4);
			//memcpy(response->maxTime, buffer + 4, response->msgLength - THREE_BYTES);
			break;
		
		case SLAVE_RESPONSE_MASTER_MCU_RETRIEVE_SENSOR_MEASUREMENT:
			
			response->sensorID = *(buffer + 3);
			response->dataType = *(buffer + 4);		
			memcpy(response->dataTimestamp, buffer + 5, EIGHT_BYTES);
			response->dataSize = response->msgLength - TWELVE_BYTES;		
			memcpy(response->data, buffer + 13, response->dataSize);			
			break;
	}
	//check if the number of bytes read verifies the packet message packet length
	if((numBytesRead >= (response->msgLength + ONE_BYTE)) && (response->returnCode == MSG_RET_OK)){
		//DEBUG_print("\nResponse Message is retrieved successfully\n");
		return COMM_OK_RX_COMPLETE;
	}
	else{
		//DEBUG_print("\nERROR: Response Message is not retrieved\n");
		return COMM_ERROR;
	}	
}
/*******************************************************************************************************/

