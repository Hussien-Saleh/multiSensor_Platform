/*
 * @brief: GPS Erinome Driver
 * @file: GPS_Erinome.h
 * @details: Driver for GPS Erinome
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#ifndef GPS_ERINOME_H_
#define GPS_ERINOME_H_

#include <atmel_start.h>
#include "config.h"
#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"

scheduled_task_description_ GPS_update_task;

/************************************************************************************************************/
/*											Function Prototypes												*/
/************************************************************************************************************/
/************************************************************************************************************/
/*										Scheduled Tasks Configurations										*/
/************************************************************************************************************/
void config_GPS_tasks_();
void task_config_GPS_update_();
/************************************************************************************************************/
/*									GPS Scheduled Callback Functions 										*/
/************************************************************************************************************/
void GPS_update_cb_(void* param, void* param_2);

/*
	 Received callback function is called when new bit on USART_GNSS is
	 available, sets gps_rx_flag -> needs to be reset after bit is stored  
*/   
void rx_cb_USART_GNSS_(const struct usart_async_descriptor *const io_descr);
/************************************************************************************************************/
/* 
	 Transmit callback function is called when message is fully transmitted
	 sets gps_tx_flag -> needs to be reset after bit is stored          
*/ 

void tx_cb_USART_GNSS_(const struct usart_async_descriptor *const io_descr);
/************************************************************************************************************/

/* 
	Function to write char array to GPS Module
	@param print_arr pointer to the print array					
*/           
void GPS_write_(uint8_t * print_arr);
/************************************************************************************************************/
/*  
	Function to read single character from GPS Module
	@param print_char 
    @return the char read												
*/
uint8_t GPS_read_char_(uint8_t * print_char);
/************************************************************************************************************/

/*  
	Function to read GPS data until find string is found
	E.g. searches for "GNGNS,"
	 
	@param buf: Character Buffer to store GPS Data
	@param find: Character Array with search string
	@param len: length of the search string
	
	@return 0 if "find" string was found and maximum search iterations "it" is not reached
*/
uint8_t gotoStringPosition_(uint8_t * buf, uint8_t * find, uint8_t len);
/************************************************************************************************************/
/*
	Function to read GPS Data until next "," = 0x2c
	Converts Data before the "," to float
	
	@param buf: Character Buffer to store GPS Data
	@param ip: float pointer

	
	@return 0 if a float was found -> when (buf == end) is false	
		
*/
uint8_t readlatlongGPS_(uint8_t * buf, int32_t * ip);
/************************************************************************************************************/
uint8_t readINTGPS_(uint8_t * buf, int32_t * ip);
/************************************************************************************************************/
/* 
	Function to read North South East West character
	reads one char and checks if it is a ","
	
	- if it is a "," returns error
	- if it is not a "," stores +1 or -1 to cp depending on NSWE
	reads next character but doe snot store it (do skip the next ",")
	
	@param buf: Character Buffer to store GPS Data
	@param cp: character pointer

	
	@return 0 if "N" "S" "W" or "E" was read		
*/

uint8_t read_NSWE_sign_GPS_(uint8_t * buf, int8_t * cp);
/************************************************************************************************************/
/* 
    Function that reads one char and checks if it is a ","
	-if it is a "," returns error
	-if it is not a "," stores character to cp
	reads next character but does not store it (do skip the next ",")

	@param buf: Character Buffer to store GPS Data
	@param cp: character pointer

	
	@return 0 if *cp is "N" "S" "W" or "E"		

*/
uint8_t checkStsGPS_(uint8_t * buf);
/************************************************************************************************************/
/* 	
	Function to configure the GPS Connection
	@return 
*/

uint8_t setup_GPS_(void);
/************************************************************************************************************/
/*  
	erinome_gps_dataset pData: Data structure pointer to store GPS data

	Searches for "GNGNS," in GPS Stream
	When found, stores Time, Lat, Long, Hemisphere, Meridian Values and Status
	Status is 1 if updated; Status is 0 if not updated
	Returns 0 if GPS Time was updated
	Returns Error if GNGNS was not found
	
*/
uint8_t update_GPS_();
/************************************************************************************************************/

/*bool ret_gpsSensor_status();*/
#endif