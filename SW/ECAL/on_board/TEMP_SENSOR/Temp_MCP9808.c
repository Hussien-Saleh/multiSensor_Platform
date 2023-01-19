/*
 * @brief: Temperature Sensor MCP9809 Driver
 * @file: Temp_MCP9809.c
 * @details: Driver for temperature sensor
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#include "ECAL/on_board/TEMP_SENSOR/Temp_MCP9808.h"

// static volatile uint8_t mcp9809_measuringCounter = 0; //measuring counter before MCU goes to sleep
// static volatile  bool mcp9809_measuringDone = false;

volatile bool TB_sts; //Board Temperature Status

void config_TempSensor_tasks_(){
	task_config_TempSensor_update_();
}
/************************************************************************************************************/
void task_config_TempSensor_update_(){
	TempSensor_update_task.task = (task_) &TempSensor_update_cb_;
	TempSensor_update_task.param = NULL;
	TempSensor_update_task.param_2 = NULL;
	TempSensor_update_task.time_expire = 15*SECOND_TO_MILLI;//1*MINUTE_TO_MILLI; //ms
	TempSensor_update_task.time_after_fire = 15*SECOND_TO_MILLI;//1*MINUTE_TO_MILLI;
}

/************************************************************************************************************/
void TempSensor_update_cb_(void* param, void* param_2){
	
	//if((mcp9809_measuringDone == false) && (mcp9809_measuringCounter <= MCP9809_MAX_MEASURING_COUNTER)){
	
		mcp9809_lowpower(DISABLE);
		update_BoardTemp_();
	//}	
}
/************************************************************************************************************/
uint8_t Temp_read_reg_(uint8_t reg, uint8_t *bufp, uint16_t len){
	
	i2c_m_sync_set_slaveaddr(&I2C_INT, MCP9809_I2C_AD, I2C_M_SEVEN);
	return i2c_m_sync_cmd_read(&I2C_INT, reg, bufp, len);
}
/************************************************************************************************************/
uint8_t setup_BoardTemp_(){
		
	uint8_t temp_buffer[2];
	Temp_read_reg_(MCP9809_WHO_AM_I, temp_buffer, 1);
	if (temp_buffer[0] !=  MCP9809_DEVICE_ID){
		return ERROR_MCP9809_SENSOR_NOT_FOUND;
	} 
	else{
		return 0;
	}
}
/************************************************************************************************************/
void update_BoardTemp_(){
	
	DEBUG_print("\n\nTemperature Update.\n");
	uint8_t TA[2] = {0, 0}; //Upper and lower bytes of the ambient temperature register
	float Tf;
	int16_t TB;
	TB_sts= !Temp_read_reg_(MCP9809_TEMPERATURE_REG_TA, TA, 2);

	if(TB_sts){
		//First check flag bits
		//if ((TA[0] && 0x80)== 0x80) {//TA? TCRIT}
		//if ((TA[0] && 0x40)== 0x40) {//TA > TUPPER}
		//if ((TA[0] && 0x20)== 0x20) {//TA < TLOWER}
		TA[0] = TA[0] & 0x1F; //Clear flag bits
		// if ambient temperature is below 0�C
		if ((TA[0] & 0x10) == 0x10){ 
			
			TA[0] = TA[0] & 0x0F; //Clear SIGN bit
			Tf = (float)256.0 - (float)(TA[0] * 16.0 + TA[1] / 16.0);
			TB = (int16_t)(Tf*100);	
		} 
		// if ambient temperature is above 0�C
		else{ 
			Tf = (float)(TA[0] * 16.0 + TA[1] / 16.0);
			TB = (int16_t)(Tf*100);
		}
	}
	
// 	if(mcp9809_measuringCounter == MCP9809_MAX_MEASURING_COUNTER-1){
// 		
// 		mcp9809_measuringDone = true;
// 		mcp9809_measuringCounter = 0;
// 	}
// 	else{
		sensor_dataset_ data_set = {0}; //temporary dataset to store the data
		data_set.active_sensor_idx = ONBOARD_TEMP_SENSOR_ID;
		data_set.data_size = ONBOARD_TEMP_SENSOR_DATA_SIZE;
		data_set.data_type = ONBOARD_TEMP_SENSOR_DATA_TYPE;
		uint64_t stamp =  scheduler_get_time_();
		
// 		char text[20];
// 		DEBUG_print("System Timestamp: ");
// 		sprintf(text, "%i \t", stamp);
// 		DEBUG_print(text);
// 		DEBUG_print("Temperature: ");
// 		sprintf(text, "%i \t", TB);
// 		DEBUG_print(text);
		
		memcpy(data_set.timestamp, &stamp, EIGHT_BYTES);
		memcpy(data_set.data, &TB, TWO_BYTES);
		enqueue_data_((void*) &data_set);
// 		mcp9809_measuringCounter++;
// 		mcp9809_measuringDone = false;
		
	//}
	mcp9809_lowpower(ENABLE);
}

void mcp9809_lowpower(uint8_t status){
	
	uint16_t buff;
	i2c_m_sync_set_slaveaddr(&I2C_INT, MCP9809_I2C_AD, I2C_M_SEVEN);
	if(status == 1){
		buff = 0x0100;
	}else{	
		buff = 0x0000;
	}
	i2c_m_sync_cmd_write(&I2C_INT, MCP9809_CONFIG_REG, &buff, 2);

}

// bool ret_tempSensor_status(){
// 	
// 	return mcp9809_measuringDone;
// }

/************************************************************************************************************/