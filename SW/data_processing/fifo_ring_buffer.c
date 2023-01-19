/*
 * @brief: FIFO Ring Buffer
 * @file: fifo_ring_buffer.c
 * @details: First-in-First-out ring buffer used as queue management for data processing
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#include "data_processing/fifo_ring_buffer.h"

static ringBuffer_ ringBuf;
scheduled_task_description_ save_data_to_SD_task;

/*******************************************************************************************************/
uint8_t buffer_init_(uint8_t numItems, uint8_t itemSize, void *buffer){
	
	//check the validity of the initialized ring buffer
	if((buffer != NULL) && (itemSize > 0)){
		//check if the size of ring buffer is a power of 2
		if(((numItems -1) & numItems) == 0){
			
			ringBuf.head = 0;
			ringBuf.tail = 0;
			ringBuf.numItems = numItems;
			ringBuf.itemSize = itemSize;
			ringBuf.buff = buffer;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t enqueue_data_(const void *data){
	
	//validating the ring buffer is not full
	if(is_buffer_full_(&ringBuf) == 0){
		//offset calculation to determine the start index for each item in the buffer
		const uint16_t idxOffset = (ringBuf.head & (ringBuf.numItems - 1)) * ringBuf.itemSize;
		//copy the data that is pushed from the caller to the correct location inside the buffer
		memcpy(&(ringBuf.buff[idxOffset]), data,  ringBuf.itemSize);
		//increment the head of the buffer
		ringBuf.head++;
			
		//if(ringBuf.head - ringBuf.tail <= NUM_BUFFER_ITEMS_BEFORE_DEQUEUE_TO_SD){
			
			uint16_t count = NUM_BUFFER_ITEMS_BEFORE_DEQUEUE_TO_SD;
			save_sensor_data_to_SD_(&count);

		//}	
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t dequeue_data_(void *data){
	
	//validating the ring buffer is not empty
	if(is_buffer_empty_(&ringBuf) == 0){
		
		//offset calculation to determine the start index for each item in the buffer
		const uint16_t idxOffset = (ringBuf.tail & (ringBuf.numItems -1)) * ringBuf.itemSize;
		//copy the item that is popped outside the buffer back to the data argument of the caller
		memcpy(data, &(ringBuf.buff[idxOffset]), ringBuf.itemSize);
		//increment the tail of the buffer
		ringBuf.tail++; 
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t is_buffer_full_(ringBuffer_ *me){
	
	return((me->head - me->tail) == me->numItems) ? 1 : 0;
}
/*******************************************************************************************************/
uint8_t is_buffer_empty_(ringBuffer_ *me){
	
	return(me->head == me->tail) ? 1 : 0;
}
/*******************************************************************************************************/
