/*
 * @brief: IMU ICM20948 Driver
 * @file: IMU_ICM20948.c
 * @details: Driver for IMU ICM20948 
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#include "ECAL/on_board/IMU/IMU_ICM20948.h"

static IMU_DataSet imuSet = {0};		// DataSet struct includes all Data -> access via pointer in function argument
static volatile bool icm20948_measuringDone = false;

volatile bool IMU_sts;

char IMU_text_buffer[80];
float IMU_ACC_Sensitivity = 2.0;
float IMU_GYRO_Sensitivity = 250.0;
float IMU_TEMP_Sensitivity = 9000.0;

uint8_t IMU_Data[IMU_SAMPLES][14];
float IMU_ACC[3];
float IMU_GYRO[3];
float IMU_TEMP;

static volatile uint16_t IMU_counter = 0;

/************************************************************************************************************/
void config_IMU_tasks_(){
	
	task_config_IMU_update_();
	task_config_IMU_sampling_update_();
}
/************************************************************************************************************/

void task_config_IMU_update_(){
	
	IMU_update_task.task = (task_) &IMU_update_cb_;
	IMU_update_task.param = NULL;
	IMU_update_task.param_2 = NULL;
	IMU_update_task.time_expire = 3*MINUTE_TO_MILLI; //ms
	IMU_update_task.time_after_fire = 3*MINUTE_TO_MILLI;
	
}
void task_config_IMU_sampling_update_(){
	
	IMU_sampling_update_task.task = (task_) &IMU_sampling_update_cb_;
	IMU_sampling_update_task.param = NULL;
	IMU_sampling_update_task.param_2 = NULL;
	IMU_sampling_update_task.time_expire = 20; //ms
	IMU_sampling_update_task.time_after_fire = 20;
	
}
/************************************************************************************************************/
void IMU_sampling_update_cb_(void *param, void* param_2){
	
	if((IMU_counter < IMU_SAMPLES) && (icm20948_measuringDone == false)){
		//disable ICM20948 sleep mode
		ICM20948_registerWrite_(ICM20948_REG_PWR_MGMT_1, (ICM20948_REG_PWR_MGMT_1 & ~(1 << ICM20948_BIT_SLEEP )));
		//read data sample
		IMU_read_();
	}
}

void IMU_update_cb_(void* param, void* param_2){
	
	scheduler_add_task_(&IMU_sampling_update_task);
	
}
/************************************************************************************************************/
void ICM20948_registerRead_(uint16_t addr, int numBytes, uint8_t *data){
	
	uint8_t regAddr;
	uint8_t bank;

	regAddr = (uint8_t) ( addr & 0x7F );
	bank = (uint8_t) ( addr >> 7 );

	ICM20948_bankSelect_( bank );
	i2c_m_sync_set_slaveaddr(&I2C_INT, (0x68U), I2C_M_SEVEN);//(0x68U>>1)
	i2c_m_sync_cmd_read(&I2C_INT, regAddr, data, numBytes);
}
/************************************************************************************************************/
void ICM20948_bankSelect_(uint8_t bank){
	
	i2c_m_sync_set_slaveaddr(&I2C_INT, (0x68U), I2C_M_SEVEN);//(0x68U>>1)
	i2c_m_sync_cmd_write(&I2C_INT, ICM20948_REG_BANK_SEL, &bank, 1);
}
/************************************************************************************************************/
void ICM20948_registerWrite_( uint16_t addr, uint8_t data ){
	
   uint8_t regAddr;
   uint8_t bank;

   regAddr = (uint8_t) ( addr & 0x7F );
   bank = (uint8_t) ( addr >> 7 );

   ICM20948_bankSelect_( bank );
   i2c_m_sync_set_slaveaddr(&I2C_INT, (0x68U), I2C_M_SEVEN);//(0x68U>>1)
   i2c_m_sync_cmd_write(&I2C_INT, regAddr, &data, 1);
   return;
}
/************************************************************************************************************/
uint32_t ICM20948_accelDataRead_(int32_t *acc){
	
   uint8_t rawData[6];
   uint8_t accelFS;
   int16_t temp;

   /* Retrieve the current resolution */
   ICM20948_accelFullScaleGet_( &accelFS );

   /* Read the six raw data registers into data array */
   ICM20948_registerRead_( ICM20948_REG_ACCEL_XOUT_H_SH, 6, &rawData[0] );
   
   /* Convert the MSB and LSB into a signed 16-bit value and multiply by the resolution to get the G value */
   temp = ( (int16_t) rawData[0] << 8 ) | rawData[1];
   acc[0] =  (int32_t)(((float)(temp * accelFS) / 32768.0) * 1000);
   temp = ( (int16_t) rawData[2] << 8 ) | rawData[3];
   acc[1] = (int32_t)(((float)(temp * accelFS) / 32768.0) * 1000);
   temp = ( (int16_t) rawData[4] << 8 ) | rawData[5];
   acc[2] = (int32_t)(((float)(temp * accelFS) / 32768.0) * 1000);
   
   return 0;
}
/************************************************************************************************************/
uint32_t ICM20948_gyroDataRead_(int32_t *gyro){

   uint8_t rawData[6];
   uint16_t gyroFS;
   int16_t temp;

   /* Retrieve the current full scale */
   ICM20948_gyroFullScaleGet_( &gyroFS );

   /* Read the six raw data registers into data array */
   ICM20948_registerRead_( ICM20948_REG_GYRO_XOUT_H_SH, 6, &rawData[0] );

   /* Convert the MSB and LSB into a signed 16-bit value and multiply by the resolution to get the dps value */
   temp = ( (int16_t) rawData[0] << 8 ) | rawData[1];
   gyro[0] = (int32_t)(((float)(temp * gyroFS) / 32768.0) * 1000);
   temp = ( (int16_t) rawData[2] << 8 ) | rawData[3];
   gyro[1] = (int32_t)(((float)(temp * gyroFS) / 32768.0) * 1000);
   temp = ( (int16_t) rawData[4] << 8 ) | rawData[5];
   gyro[2] = (int32_t)(((float)(temp * gyroFS) / 32768.0) * 1000);

   return 0;
}
/************************************************************************************************************/
uint32_t ICM20948_accelFullScaleGet_(uint8_t *accelFS){

   uint8_t reg;

   /* Read the actual acceleration full scale setting */
   ICM20948_registerRead_( ICM20948_REG_ACCEL_CONFIG, 1, &reg );
   reg &= ICM20948_MASK_ACCEL_FULLSCALE;

   /* Calculate the resolution */
   switch( reg ) {

		case ICM20948_ACCEL_FULLSCALE_2G:
			
			*accelFS = 2.0;
			 break;

		case ICM20948_ACCEL_FULLSCALE_4G:
		
			 *accelFS = 4.0;
			 break;

		case ICM20948_ACCEL_FULLSCALE_8G:
		
			 *accelFS = 8.0;
			  break;

		case ICM20948_ACCEL_FULLSCALE_16G:
		
			 *accelFS = 16.0;
			 break;
   }
   return ICM20948_OK;
}
/************************************************************************************************************/
uint32_t ICM20948_gyroFullScaleGet_( uint16_t *gyroFS ){

   uint8_t reg;

   /* Read the actual gyroscope full scale setting */
   ICM20948_registerRead_( ICM20948_REG_GYRO_CONFIG_1, 1, &reg );
   reg &= ICM20948_MASK_GYRO_FULLSCALE;

   /* Calculate the resolution */
   switch( reg ) {

		case ICM20948_GYRO_FULLSCALE_250DPS:
			*gyroFS = 250;
			break;

		case ICM20948_GYRO_FULLSCALE_500DPS:
			 *gyroFS = 500;
			break;

		case ICM20948_GYRO_FULLSCALE_1000DPS:
			*gyroFS = 1000;
			 break;

		 case ICM20948_GYRO_FULLSCALE_2000DPS:
			 *gyroFS = 2000;
			 break;
	}
   return ICM20948_OK;
}
/************************************************************************************************************/
uint32_t ICM20948_accelFullscaleSet_(uint8_t accelFs){
	
   uint8_t reg;
   accelFs &= ICM20948_MASK_ACCEL_FULLSCALE;
   ICM20948_registerRead_( ICM20948_REG_ACCEL_CONFIG, 1, &reg );
   reg &= ~(ICM20948_MASK_ACCEL_FULLSCALE);
   reg |= accelFs;
   ICM20948_registerWrite_(ICM20948_REG_ACCEL_CONFIG, reg);
   return ICM20948_OK;
}
/************************************************************************************************************/
uint32_t ICM20948_gyroFullscaleSet_( uint8_t gyroFs ){
	
   uint8_t reg;
   gyroFs &= ICM20948_MASK_GYRO_FULLSCALE;
   ICM20948_registerRead_( ICM20948_REG_GYRO_CONFIG_1, 1, &reg );
   reg &= ~( ICM20948_MASK_GYRO_FULLSCALE );
   reg |= gyroFs;
   ICM20948_registerWrite_( ICM20948_REG_GYRO_CONFIG_1, reg );
   return ICM20948_OK;
}
/************************************************************************************************************/
uint8_t IMU_setup_(){
	
	uint8_t IMU_buff;
// 	DEBUG_print("Searching IMU.....\n");
	/* Issue reset */
	ICM20948_registerWrite_(ICM20948_REG_PWR_MGMT_1, ICM20948_BIT_H_RESET);
	delay_ms(100);
	ICM20948_registerWrite_(ICM20948_REG_PWR_MGMT_1, ICM20948_BIT_CLK_PLL);
	ICM20948_registerRead_(ICM20948_REG_WHO_AM_I, 1, &IMU_buff); // Read WHO_AM_I
	return (IMU_buff !=  ICM20948_DEVICE_ID);
}
/************************************************************************************************************/	
void IMU_read_(){
	

	// Read IMU Values
	ICM20948_accelDataRead_(imuSet.IMU_acc[IMU_counter]);
	ICM20948_gyroDataRead_(imuSet.IMU_gyro[IMU_counter]);
	
	if(IMU_counter == IMU_SAMPLES - 1){		
		
		 DEBUG_print("\nIMU readings update\n");

		scheduler_remove_task_(&IMU_sampling_update_task);
		//enable ICM20948 sleep mode
		ICM20948_registerWrite_(ICM20948_REG_PWR_MGMT_1, (ICM20948_REG_PWR_MGMT_1 | (1 << ICM20948_BIT_SLEEP )));
		save_IMU_data_to_SD_(&imuSet);
		IMU_counter = 0;
		icm20948_measuringDone = true;
	}
	else{	
		icm20948_measuringDone = false;
		IMU_counter++;
	}
}

// bool ret_imuSensor_status(){
// 	
// 	return icm20948_measuringDone;
// }

/************************************************************************************************************/
