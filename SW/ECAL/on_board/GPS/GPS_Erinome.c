/*
 * @brief: GPS Erinome Driver
 * @file: GPS_Erinome.c
 * @details: Driver for GPS Erinome
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/
#include "ECAL/on_board/GPS/GPS_Erinome.h"

#define GPS_WAIT 200
//volatile uint8_t forTestingOnlyString[] = "$GNRMC,153810.000,A,6489.0799,N,16202.6500,W,0.00,0.00,300421�,A*??";
//uint8_t forTestingOnlyInt = 0;

//static volatile uint8_t gps_measuringCounter = 0; //measuring counter before MCU goes to sleep
//static volatile bool gps_measuringDone = false;

struct io_descriptor *UART_GNSS_io;
volatile bool gps_rx_flag = false;
volatile bool gps_tx_flag = false;

//volatile bool GPS_update_flag = false;

volatile uint8_t GPS_position_sts;
volatile uint8_t GPS_time_sts;
volatile uint8_t GPS_date_sts;

/************************************************************************************************************/
void config_GPS_tasks_(){
	task_config_GPS_update_();
}
/************************************************************************************************************/
void task_config_GPS_update_(){
	GPS_update_task.task = (task_) &GPS_update_cb_;
	GPS_update_task.param = NULL;
	GPS_update_task.param_2 = NULL;
	GPS_update_task.time_expire = 25*SECOND_TO_MILLI; //ms
	GPS_update_task.time_after_fire = 25*SECOND_TO_MILLI; //ms
	
}
/************************************************************************************************************/
void GPS_update_cb_(void* param, void* param_2){
	
	//if((gps_measuringCounter < GPS_MAX_MEASURING_COUNTER) && (gps_measuringDone == false)){	
		//enable full power mode and disable hibernation
		DEBUG_print("\n\nGPS -> full power\n");

		gpio_set_pin_level(GNSS_ON, true);
		update_GPS_();
	//}
}
/************************************************************************************************************/
void rx_cb_USART_GNSS_(const struct usart_async_descriptor *const io_descr){
	/* Transfer completed */
	gps_rx_flag = true;
}
/************************************************************************************************************/
void tx_cb_USART_GNSS_(const struct usart_async_descriptor *const io_descr){
	/* Transfer completed */
	gps_tx_flag = true;
}
/************************************************************************************************************/
void GPS_write_(uint8_t * print_arr){
	gps_tx_flag = false;
	while (io_write(UART_GNSS_io, print_arr, strlen(print_arr)) != strlen(print_arr));
	while (!gps_tx_flag);
}
/************************************************************************************************************/
uint8_t GPS_read_char_(uint8_t * print_char){
	
	//print_char[0] = forTestingOnlyString[forTestingOnlyInt];
	//forTestingOnlyInt++;
	//if(forTestingOnlyInt > 67)
	//{ 
		//forTestingOnlyInt = 0;
	//} 
	//DEBUG_print_char(print_char);

	//while (1) { 
		
		while(!gps_rx_flag); 
		if (io_read(UART_GNSS_io, print_char, 1) == 1){
			DEBUG_print_char(print_char);
			return 0;	
		}else {	
			gps_rx_flag = 0;
		}
	//}
}
/************************************************************************************************************/
uint8_t gotoStringPosition_(uint8_t * buf, uint8_t * find, uint8_t len){
	
	memset(buf, 0, len);
	uint16_t i_char = 0;
	uint16_t it = 0;
	
	while(i_char < len && it < GPS_CHAR_ITERATION_LIMIT){
		
		it++;
		GPS_read_char_(&buf[i_char]);
		
		if (buf[i_char] == find[i_char]){
			i_char++;
		}
		else{	
			i_char = 0;
		}
	}
	return (strncmp(buf,find,len) && it < GPS_CHAR_ITERATION_LIMIT);
}

/************************************************************************************************************/
uint8_t readlatlongGPS_(uint8_t * buf, int32_t * ip){
	
	memset(buf, 0, GPS_MAX_BUF_LENGTH);
	uint16_t i_char = 0;
	uint16_t i_ip = 0;
	int32_t ip_arr[] = {0,0};
	char *end;

	while(i_char < GPS_MAX_BUF_LENGTH && i_ip < 2){
		
		GPS_read_char_(&buf[i_char]);
		
		// 2c is comma
		if (buf[i_char] == 0x2c){
			
			ip_arr[1] = (int32_t)strtol(buf, &end, 10);
			*ip = ip_arr[0] * 10000 + ip_arr[1];
			return (buf == end);
		}
		
		// 2e is dot
		if (buf[i_char] == 0x2e){
			
			ip_arr[0] = (int32_t)strtol(buf, &end, 10);
			i_ip++;
			memset(buf, 0, GPS_MAX_BUF_LENGTH);
			i_char = 0;
			continue;
		}
		
		i_char++;
	}
	return ERROR_GPS_NO_NUM_FOUND;
}
/************************************************************************************************************/
uint8_t readINTGPS_(uint8_t * buf, int32_t * ip){
	
	memset(buf, 0, GPS_MAX_BUF_LENGTH);
	uint16_t i_char = 0;
	char * end;

	while(i_char < GPS_MAX_BUF_LENGTH){
		
		GPS_read_char_(&buf[i_char]);
		if(buf[i_char] == 0x2c){
			*ip = (int32_t)strtol(buf, &end, 10);
			return (buf == end);
		}
		i_char++;
	}
	return ERROR_GPS_NO_NUM_FOUND;
}
/************************************************************************************************************/
uint8_t read_NSWE_sign_GPS_(uint8_t * buf, int8_t * cp){
	
	memset(buf, 0, GPS_MAX_BUF_LENGTH);
	GPS_read_char_(buf);
	
	if(buf[0] == 0x2c){
		
		return ERROR_GPS_NO_NUM_FOUND;
	
	}else{
		
		 //N, S, W,E in hex conversion format
		if (buf[0] == 0x57 || buf[0] == 0x53){
			*cp = -1;
		}
		else{
			
			if (buf[0] == 0x4e || buf[0] == 0x45){
				*cp = 1;
			} 
			else {
				*cp = 0;
				// TODO: what happens if char is not N E W S?
			}
		}
		
		GPS_read_char_(&buf[1]); // Read next ","
		return (!*cp);
	}
	return ERROR_GPS_NO_NUM_FOUND;
}
/************************************************************************************************************/
uint8_t checkStsGPS_(uint8_t * buf){
	char cp;
	GPS_read_char_(buf);
	if(buf[0] == 0x2c){
		return ERROR_GPS_NO_NUM_FOUND;
	}else{
		cp = buf[0];
		GPS_read_char_(&buf[1]);
		return !(cp == 0x41 || cp == 0x56); // check if valid -> "A" TODO: handle not valid -> "V"
	}
	return ERROR_GPS_NO_NUM_FOUND;
}
/************************************************************************************************************/
uint8_t setup_GPS_(void){	

	usart_async_register_callback(&USART_GNSS, USART_ASYNC_RXC_CB, rx_cb_USART_GNSS_);
	usart_async_register_callback(&USART_GNSS, USART_ASYNC_TXC_CB, tx_cb_USART_GNSS_);
	usart_async_get_io_descriptor(&USART_GNSS, &UART_GNSS_io);
	usart_async_enable(&USART_GNSS);
	gpio_set_pin_level(GNSS_ON, true);
	gpio_set_pin_level(GNSS_RESET, false);
	
	uint8_t gps_buffer[GPS_MAX_BUF_LENGTH];
	//factory reset
	GPS_write_("$PSRF101,0,0,0,0,0,0,12,8*1C\r\n");
	//warm start with initialization data
	//GPS_write_("$PSRF101,-2686700,-4304200,3851624,96000,497260,921,12,3*1C\r\n");
	//hot start
	//GPS_write_("$PSRF101,0,0,0,0,0,0,12,1*15\r\n"); 
 	//delay_ms(GPS_WAIT);

	usart_async_flush_rx_buffer(&UART_GNSS_io); // flushing the buffer
	gps_rx_flag = false;
	
	if(gotoStringPosition_(gps_buffer, "PSRF150,2", 9) == 0){  // Receiver initialization is complete in full power state (after restart or hibernate). Ok to Send
  		GPS_write_("$PSRF103,0,0,0,1*24\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
  		GPS_write_("$PSRF103,1,0,0,1*25\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
  		GPS_write_("$PSRF103,2,0,0,1*26\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
  		GPS_write_("$PSRF103,3,0,0,1*27\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
  		GPS_write_("$PSRF103,4,0,0,1*20\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
//  		GPS_write_("$PSRF103,5,0,0,1*21\r\n"); // turn off
// 		delay_ms(GPS_WAIT);
// 		GPS_write_("$PSRF103,6,0,0,1*22\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
//  		GPS_write_("$PSRF103,7,0,0,1*23\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
// 		GPS_write_("$PSRF103,8,0,0,1*2C\r\n"); // turn off
// 		delay_ms(GPS_WAIT);
//  		GPS_write_("$PSRF103,9,0,0,1*2D\r\n"); // turn off
//  		delay_ms(GPS_WAIT);
		GPS_write_("$PSRF103,1,1,0,1*24\r\n"); // call GLL once
		
		if(gotoStringPosition_(gps_buffer, "GNGLL,", 6) == 0){  // Receiver initialization is complete in full power state (after restart or hibernate). Ok to Send
			return 0;
		}
	}
	return ERROR_GPS_GNGNS_NOT_FOUND;
}
/************************************************************************************************************/
uint8_t update_GPS_(){
	
	
	//delay_ms(GPS_WAIT);
	DEBUG_print("\n\nGPS Update...\n");
	sensor_dataset_ data_set = {0}; //temporary dataset to store the data
	uint8_t gps_buffer[GPS_MAX_BUF_LENGTH];
	usart_async_flush_rx_buffer(&UART_GNSS_io);
	gps_rx_flag = false;
	
	// Get Position Date and Time
	// FIND $GNRMC,153810.000,A,5357.5148,N,996.2749,E,0.00,0.00,300421�,A*??
	int32_t GPS_UNIX_Timestamp;
	int32_t GPS_latitude;
	int32_t GPS_longitude;
	int32_t temp_Time;
	int32_t temp_Date;
	int32_t temp_lat;
	int32_t temp_long;
	int8_t temp_NS_sign;
	int8_t temp_EW_sign;
	uint8_t ret_sts;
   	   
	 GPS_write_("$PSRF103,4,1,0,1*21\r\n"); // call RMC once
	 if(gotoStringPosition_(gps_buffer, "GNRMC,", 6) == 0){
		   // Read Time Value after first "," after GNRMC
		   GPS_time_sts = !readINTGPS_(gps_buffer, &temp_Time);
		   // Check if Time Value is Valid -> A or V
		   ret_sts = !checkStsGPS_(gps_buffer);  
		   GPS_time_sts = (GPS_time_sts && ret_sts);  
		   // read longitude position
		   GPS_position_sts = !readlatlongGPS_(gps_buffer, &temp_lat);   
		   // read North South Character
		   ret_sts = !read_NSWE_sign_GPS_(gps_buffer, &temp_NS_sign);
		   GPS_position_sts = (GPS_position_sts && ret_sts); 
		    // read Latitude position
		   ret_sts = !readlatlongGPS_(gps_buffer, &temp_long); 
		   GPS_position_sts = (GPS_position_sts && ret_sts);
		   
		   // read West East Character
		   ret_sts = !read_NSWE_sign_GPS_(gps_buffer, &temp_EW_sign);	   
		   GPS_position_sts = (GPS_position_sts && ret_sts);
		   
		   gotoStringPosition_(gps_buffer, ",", 1);
		   gotoStringPosition_(gps_buffer, ",", 1);
		   GPS_date_sts =  !readINTGPS_(gps_buffer, &temp_Date);
		  
		   // Write Time Data if status is valid
		   if (!GPS_time_sts){
			   //GPS_UNIX_Timestamp = 0;

		   }
		   else{

			   struct tm time_info;
			   time_info.tm_hour = (int)temp_Time/10000;
			   time_info.tm_min = ((int)temp_Time - (time_info.tm_hour*10000)) / 100;
			   time_info.tm_sec = ((int)temp_Time - (time_info.tm_hour*10000) - (time_info.tm_min*100));
			   
			   if (GPS_date_sts){
				   
				   time_info.tm_mday = (int)temp_Date/10000;
				   time_info.tm_mon = (((int)temp_Date - (time_info.tm_mday*10000)) / 100)-1;
				   time_info.tm_year = (100 + ((int)temp_Date - (time_info.tm_mday*10000) - ((time_info.tm_mon+1)*100)));
			   }
			   else{
				   
				   time_info.tm_mday = 0;
				   time_info.tm_mon = 0;
				   time_info.tm_year = 0;
			   }
			   time_info.tm_isdst = 0;
			   GPS_UNIX_Timestamp = (int32_t)mktime(&time_info);
		   }
		   
		   // Write Position Data if status is valid
		   if (!GPS_position_sts){
			   
			  			// keep last position?
						 // keep last position?
		   }
		   else{
			   
			   GPS_latitude = temp_lat * temp_NS_sign;
			   GPS_longitude = temp_long * temp_EW_sign;
			   
		   }
		   
// 		   if(gps_measuringCounter == GPS_MAX_MEASURING_COUNTER-1){
// 			   
// 			   gps_measuringDone = true;
// 			   gps_measuringCounter = 0;
// 		   }
	//	   else{
			   data_set.active_sensor_idx = ONBOARD_GPS_SENSOR_ID;
			   data_set.data_size = ONBOARD_GPS_DATA_SIZE;
			   data_set.data_type = ONBOARD_GPS_DATA_TYPE;
			   
			   uint64_t stamp =  scheduler_get_time_();
			   memcpy(data_set.timestamp, &stamp, EIGHT_BYTES);
			   memcpy(data_set.data, &GPS_UNIX_Timestamp, FOUR_BYTES);
			   memcpy(&data_set.data[4], &GPS_longitude, FOUR_BYTES);
			   memcpy(&data_set.data[8], &GPS_latitude, FOUR_BYTES);
			   
// 			   char text[30];
// 			   DEBUG_print("System Timestamp: ");
// 			   sprintf(text, "%i \t", stamp);
// 			   DEBUG_print(text);
// 			   DEBUG_print("Longitude: ");
// 			   sprintf(text, "%i \t", GPS_longitude);
// 			   DEBUG_print(text);
// 			   DEBUG_print("Latitude: ");
// 			   sprintf(text, "%i \t", GPS_latitude);
// 			   DEBUG_print(text);
// 			   DEBUG_print("UNIX_timestamp: ");
// 			   sprintf(text, "%i \t", GPS_UNIX_Timestamp);
// 			   DEBUG_print(text);
// 			   
			   //enqueue retrieved data
			   enqueue_data_((void*) &data_set);
			   
			//   gps_measuringCounter++;
			 //  gps_measuringDone = false;
	//	   }
	   
		//turn off system and hibernate
		gpio_set_pin_level(GNSS_ON, false);
		//GPS_write_("$PSRF117,16*0B\r\n");
 	  	//while(gotoStringPosition_(gps_buffer, "PSRF150,0", 9) != 0);
		   //{
 		   	DEBUG_print("\n\nGPS -> hibernate mode\n");
 	   	//}
		delay_ms(500);
			

		return 0;	
	}
	else{
		return ERROR_GPS_GNGNS_NOT_FOUND;
	}	
}


// bool ret_gpsSensor_status(){
	
//	return gps_measuringDone;
//}