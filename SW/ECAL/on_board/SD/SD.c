/*
 * @brief: SD card Driver
 * @file: SD.c
 * @details: Driver for SD card
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#include "ECAL/on_board/SD/SD.h"

FATFS FatFs;			/* FatFs work area needed for each volume */
FIL dataFile;			/* File object needed for each open file */
FIL imuFile;			/* File object needed for each open file */
TCHAR dataFileName[DATA_FILE_NAME_STRING_SIZE+1];
TCHAR imuFileName[IMU_FILE_NAME_STRING_SIZE+1];
 
/************************************************************************************************************/
volatile bool SD_insertion_timeoutFlag = false;

/************************************************************************************************************/
void config_SDCard_tasks_(){
	
	SD_insertion_timeout_task_config_();
}
/************************************************************************************************************/
void SD_insertion_timeout_task_config_(){
	
	SD_insertion_timeout_task.task = (task_) &SD_insertion_timeout_;
	SD_insertion_timeout_task.param = NULL;
	SD_insertion_timeout_task.param_2 = NULL;
	SD_insertion_timeout_task.time_expire = 10000; //ms
	SD_insertion_timeout_task.time_after_fire = 0;
}
/************************************************************************************************************/
void SD_insertion_timeout_(void* param, void* param_2){
	
	 SD_insertion_timeoutFlag = true;
}

/************************************************************************************************************/
uint8_t init_SD_(){
	
	UINT bw;
	FRESULT fr;
	gpio_set_pin_level(SD_OFF,false);
	
	bool noCardInserted = gpio_get_pin_level(SD_CD);
	scheduler_add_task_(&SD_insertion_timeout_task);

	while(noCardInserted){
		
		//DEBUG_print("no SD Card inserted \n");
// 		gpio_set_pin_level(LED_G,false);
// 		gpio_set_pin_level(LED_R,true);
		noCardInserted = gpio_get_pin_level(SD_CD);

		if(SD_insertion_timeoutFlag == true){
			SD_insertion_timeoutFlag = false;
			break;
		}
	};
	if(!noCardInserted)
	//	DEBUG_print("SD Card inserted \n");
	
	if(SD_insertion_timeoutFlag == true){
	//	DEBUG_print("no SD Card inserted \n");
		SD_insertion_timeoutFlag = false;	
	}
	scheduler_remove_task_(&SD_insertion_timeout_task);

	int8_t numberOfSensorVariables = NUMBER_OF_SENSOR_BYTE_VARIABLES;
	int32_t numberOfIMUSamples = IMU_SAMPLES;
	f_mount(&FatFs, "", 0);	/* Give a work area to the default drive */
	uint16_t fileCounter = 0;
	bool newFile = false;
		
	do{
		fileCounter++;
		TCHAR *p_dataFileName = dataFileName;
		char charDataFileName[DATA_FILE_NAME_STRING_SIZE+1], *p_charDataFilename = charDataFileName;
		snprintf(p_charDataFilename, DATA_FILE_NAME_STRING_SIZE+1, "DATA%03i.BIN", fileCounter);
		while(*p_charDataFilename){
			*p_dataFileName++ = (TCHAR)*p_charDataFilename++;
		}
			
		*p_dataFileName = 0;
		fr = f_open(&dataFile, dataFileName, FA_WRITE | FA_CREATE_NEW);	/* Create a file */
			
	}while (FR_EXIST == fr && fileCounter < 999);
		
	if(fr == FR_OK) {
		
		f_write(&dataFile, &numberOfSensorVariables, 1, &bw);	/* Write data to the file */
		//DEBUG_print("writing dataFile OK \n");
		fr = f_close(&dataFile);	/* Close the file */
	}
		
	if(fr == FR_OK && bw == 4)
	{
		TCHAR *p_IMUFileName = imuFileName;
		char charIMUFileName[IMU_FILE_NAME_STRING_SIZE+1], *p_charIMUFilename = charIMUFileName;
		snprintf(p_charIMUFilename, IMU_FILE_NAME_STRING_SIZE+1, "IMU%03i.BIN", fileCounter);
			
		while(*p_charIMUFilename){
			*p_IMUFileName++ = (TCHAR)*p_charIMUFilename++;
		}
		*p_IMUFileName = 0;
		fr = f_open(&imuFile, imuFileName, FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */	
	}
	if(fr == FR_OK) {
		
		f_write(&imuFile, &numberOfIMUSamples, 4, &bw);	/* Write data to the file */
		fr = f_close(&imuFile);	/* Close the file */
		//DEBUG_print("writing imuFile OK \n");
	}
	return !(fr == FR_OK);
}
/************************************************************************************************************/

void save_sensor_data_to_SD_(uint16_t *count){
	
	DEBUG_print("\nSensor data to SD Card\n");

	sensor_dataset_ set;	
	UINT bw;
	FRESULT fr;
	gpio_set_pin_level(SD_OFF,false);
// 	gpio_set_pin_level(LED_G,true);
// 	gpio_set_pin_level(LED_R,true);
	
	fr = f_open_append(&dataFile, dataFileName);
	char text[10];
				
// 	DEBUG_print("FR result: ");
// 	sprintf(text, "%i \t", fr);
// 	DEBUG_print(text);
// 				
	
	if (fr == FR_OK) {
		
		for(uint16_t k = 0 ; k < *count; k++){
			
			if(dequeue_data_(&set) == RET_SUCCESS){
								
				f_write(&dataFile, &set.active_sensor_idx, 1, &bw);	/* Write data to the file */
				f_write(&dataFile, &set.data_type, 1, &bw);	/* Write data to the file */
				f_write(&dataFile, &set.data_size, 1, &bw);	/* Write data to the file */
				f_write(&dataFile, &set.timestamp, 8, &bw);	/* Write data to the file */
				f_write(&dataFile, &set.data, set.data_size, &bw);	/* Write data to the file */
				
// 				#ifdef DEBUG_PRINT_H_
// 				char textBuffer[100];
// 				DEBUG_print("\n======================\n");
// 				DEBUG_print("Sensor Data Readings: \n");
// 				
// 				DEBUG_print("Sensor Index: ");
// 				sprintf(textBuffer, "%i \t", set.active_sensor_idx);
// 				DEBUG_print(textBuffer);
// 				
// 				DEBUG_print("Data Size: ");
// 				sprintf(textBuffer, "%i \t", set.data_size);
// 				DEBUG_print(textBuffer);
// 				
// 				DEBUG_print("Data Type: ");
// 				sprintf(textBuffer, "%i \t", set.data_type);
// 				DEBUG_print(textBuffer);
// 				
// 				DEBUG_print("Timestamp: ");
// 				for(int i=0; i < EIGHT_BYTES; i++){
// 					sprintf(textBuffer, "%i\t", set.timestamp[i]);
// 					DEBUG_print(textBuffer);
// 				}				
// 				
// 				DEBUG_print("Data: ");
// 				for(int i=0; i < set.data_size; i++){
// 					sprintf(textBuffer, "%i \t", set.data[i]);
// 					DEBUG_print(textBuffer);
// 				}
// 				DEBUG_print("\n======================\n");
// 				#endif
				
			}
		}
		fr = f_close(&dataFile);	/* Close the file */
	}
	else
	{
		//DEBUG_print("\nError: Sensor data is not saved\n");
	}
}

void save_IMU_data_to_SD_(IMU_DataSet *d){
	
	DEBUG_print("\nIMU data to SD Card\n");
	UINT bw;
	FRESULT fr;
	
	gpio_set_pin_level(SD_OFF,false);
	fr = f_open_append(&imuFile, imuFileName);
	
// 	char text[10];		
// 	DEBUG_print("FR result: ");
// 	sprintf(text, "%i \t", fr);
// 	DEBUG_print(text);
// 	
	if(fr == FR_OK)
	{	
		f_write(&imuFile, &d->IMU_acc, 4*3*IMU_SAMPLES, &bw);	/* Write data to the file */
		f_write(&imuFile, &d->IMU_gyro, 4*3*IMU_SAMPLES, &bw);	/* Write data to the file */
		fr = f_close(&imuFile);	/* Close the file */
		//DEBUG_print("\nIMU data is successfully saved\n");
	}
	else
	{
		//DEBUG_print("\nError: IMU data is not saved\n");
	}
}