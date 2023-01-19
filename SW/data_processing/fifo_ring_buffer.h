/*
 * @brief: FIFO Ring Buffer
 * @file: fifo_ring_buffer.h
 * @details: First-in-First-out ring buffer used as queue management for data processing
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/


#ifndef FIFO_RING_BUFFER_H_
#define FIFO_RING_BUFFER_H_

#include <atmel_start.h>
#include "scheduler/scheduler.h"
#include "ECAL/on_board/SD/SD.h"
#include "config.h"

typedef struct ringBufferClass_ ringBuffer_;

/* Handler for the ring buffer */
struct ringBufferClass_{
	
	volatile uint16_t head; //head of buffer
	volatile uint16_t tail; //tail of buffer
	
	uint8_t numItems; //number of items to be allocated
	uint8_t itemSize; //size of each item
	int8_t *buff; //pointer to the buffer, the buffer is just an array of bytes
};


/*******************************************************************************************************/
/*											Function Prototypes										   */
/*******************************************************************************************************/

/* Handling Constructor to initialize the ring buffer, user must provide the memory used by the ring buffer to store the data
*  @param num_items, number of items a buffer can hold
*  @param item_size, size of each item 
*  @param buff, pointer to the buffer that will hold the data
*/
uint8_t buffer_init_(uint8_t numItems, uint8_t itemSize, void *buffer);
/*
* function to push the data into the ring buffer
*  @param data, pointer to the data to be pushed
*/
uint8_t enqueue_data_(const void *data);

/*
* function to pop the data out of the ring buffer
*  @param me, pointer to the ring buffer
*  @param data, pointer to the data to be popped
*/
uint8_t dequeue_data_(void *data);

/*
* function to check if the buffer is full
*  @param me, pointer to the ring buffer
*/
uint8_t is_buffer_full_(ringBuffer_ *me);

/*
* function to check if the buffer is empty
*  @param me, pointer to the ring buffer
*/
uint8_t is_buffer_empty_(ringBuffer_ *me);

#endif 