/*
 * @brief: Transmitter-retriever interface for Low-level Asynchronous communication protocol (I2C)
 * @file: comm_protocol.h
 * @details: Transmitter and retriever interface for Asynchronous communication protocol between the master and slave devices
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#ifndef COMM_PROTOCOL_H_
#define COMM_PROTOCOL_H_

#include <atmel_start.h>
#include "config.h"
#include "ext_comm_protocol/include/msg_layer.h"

#define MAX_BUFFER_SIZE 255U //maximum size of the data buffer used for transmitting and retrieving the message packet
#define MIN_BUFFER_SIZE 0U //minimum size of the data buffer used for transmitting and retrieving the message packet

typedef struct packetClass_ packet_; //packet for transmitting and retrieving data

/* data buffers to transmit and retrieve the data */
struct packetClass_{
	uint8_t outputBuffer [MAX_BUFFER_SIZE]; //output packet: transmit data buffer
	uint8_t inputBuffer [MAX_BUFFER_SIZE]; //input packet: retrieve data buffer
};

/* communication interfacing status */
typedef enum
{
	COMM_ERROR = 0, //communication error
	COMM_OK_TX_COMPLETE = 1, //communication success, transmission completed
	COMM_OK_RX_COMPLETE = 2, //communication success, retrieving completed
	COMM_INVALID_DATA_PACKET = 3, //invalid and corrupted message packet 
	COMM_INVALID_COMMAND = 4, //command ID is invalid
	COMM_INSUFFICIENT_BUFFER_SIZE = 5, //no space in the data buffer
	COMM_TX_IN_PROCESS = 6, //transmission is pending
	COMM_RX_IN_PROCESS = 7, //retrieving is pending
	
}commStatus_;

/*
* Function to transmit the request message from master to slave 
* @param request, pointer to the request message
* @param slaveAddress, address of the slave microcontroller
* @return the communication status
*/
commStatus_ comm_m_transmit_msg_(requestMsg_ *request, int16_t slaveAddress);

/* 
* Function to retrieve the response message from slave to master
* @param response, pointer to the response message
* @param slaveAddress, address of the slave microcontroller
* @return the communication status
*/
commStatus_ comm_m_retrieve_msg_(responseMsg_ *response, int16_t slaveAddress);

#endif