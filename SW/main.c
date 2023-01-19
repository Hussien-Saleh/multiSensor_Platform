/*
 * @brief: Main Application File
 * @file: main.c
 * @details: Application file to control on-board sensor modules and event-driven state-machine modular platform for external sensors
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#include <atmel_start.h>
#include "config.h" //software configuration done here
#include "Serial_Debug/DEBUG_print.h"

#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"
#include "ext_comm_protocol/include/async_fsm.h"
#include "ECAL/on_board/SD/SD.h"						// SD Card Driver using FatFs
#include "ECAL/on_board/GPS/GPS_Erinome.h"				// GPS API
#include "ECAL/on_board/IMU/IMU_ICM20948.h"				// IMU API
#include "ECAL/on_board/TEMP_SENSOR/Temp_MCP9808.h"		// OnChip Temperature Sensor API

/************************************************************************************************************/
/*											Global Variables										        */
/************************************************************************************************************/

struct io_descriptor *I2C_INT_io;
static struct timer_task sleep_timer_task;

/* external communication global variables */
static mcu_ slaveMCU = {0};
static activeObjectFsm_ fsm_activeMCU = {0};

static sensor_dataset_ sensorsDatasets_buffer[MAX_DATASETS];
	
/* general global variables */
volatile uint16_t blinkCounter = 0;		// state machine token to time measurement cycles
uint8_t blinkLED;

scheduled_task_description_ setup_n_check_onboard_connections_task;
scheduled_task_description_ blinking_task;
scheduled_task_description_ setup_completed_update_measuring_task;

//scheduled_task_description_ check_onboard_flags_task;

/************************************************************************************************************/
/*										Function Prototypes											        */
/************************************************************************************************************/

void task_config_setup_n_check_onboard_connections_();
void setup_n_check_connections_cb_(void* param, void* param_2);
void task_config_blinking_();
void blinking_timer_cb_(void* param, void*param_2);
void task_config_setup_completed_update_measuring_();
void setup_completed_update_measuring_cb_(void* param, void* param_2);

//void check_onboard_flags_cb_(void* param, void* param_2);
//void task_config_check_onboard_flags_();
//void sleep_task_cb(const struct timer_task *const timer_task);

 /************************************************************************************************************/
/*										Scheduled Tasks Configuration								        */
/************************************************************************************************************/
void task_config_setup_n_check_onboard_connections_() {
	setup_n_check_onboard_connections_task.task = (task_) &setup_n_check_connections_cb_;
	setup_n_check_onboard_connections_task.param = NULL;
	setup_n_check_onboard_connections_task.param_2 = NULL;
	setup_n_check_onboard_connections_task.time_expire = 10; //ms
	setup_n_check_onboard_connections_task.time_after_fire = 0;
}
/*******************************************************************************************************/
void task_config_blinking_(){
	blinking_task.task = (task_) &blinking_timer_cb_;
	blinking_task.param = NULL;
	blinking_task.param_2 = NULL;
	blinking_task.time_expire = 50; //ms
	blinking_task.time_after_fire = 50;
}
/*******************************************************************************************************/
void task_config_setup_completed_update_measuring_(){
	setup_completed_update_measuring_task.task = (task_) &setup_completed_update_measuring_cb_;
	setup_completed_update_measuring_task.param = NULL;
	setup_completed_update_measuring_task.param_2 = NULL;
	setup_completed_update_measuring_task.time_expire = 10; //ms
	setup_completed_update_measuring_task.time_after_fire = 0;
}

// void task_config_check_onboard_flags_() {
// 	check_onboard_flags_task.task = (task_) &check_onboard_flags_cb_;
// 	check_onboard_flags_task.param = NULL;
// 	check_onboard_flags_task.param_2 = NULL;
// 	check_onboard_flags_task.time_expire = 3*SECOND_TO_MILLI; //ms
// 	check_onboard_flags_task.time_after_fire = 3*SECOND_TO_MILLI;
// }

/************************************************************************************************************/
/*											Callback Functions  								            */
/************************************************************************************************************/

// void sleep_task_cb(const struct timer_task *const timer_task){
// 	
// 	//blinkLED = LED_G;
//	gpio_set_pin_level(LED_G,true);
// 	gpio_set_pin_level(LED_R,true);
		
//	delay_ms(100);
//	NVIC_SystemReset();
// 	
// }

// void check_onboard_flags_cb_(void* param, void* param_2){
// 	
// 	if(ret_slaveMCU_status() 
// 		&& ret_tempSensor_status() && ret_imuSensor_status()
// 		 && ret_gpsSensor_status()){
// 			
// 			DEBUG_print("\nAll measurings are done...\n");
// 
// 			sleep_timer_task.interval = ((SLEEP_SECONDS)*(SECOND_TO_MILLI)) +((SLEEP_MINUTES)*(MINUTE_TO_MILLI)) + ((SLEEP_HOURS)*(HOUR_TO_MILLI)) ;
// 			 sleep_timer_task.cb       = sleep_task_cb;
// 			 sleep_timer_task.mode    = TIMER_TASK_ONE_SHOT;
// 			 //
// 			 timer_add_task(&SLEEP_TIMER, &sleep_timer_task);
// 			 timer_start(&SLEEP_TIMER);
// 
// 			 _pmc_disable_periph_clock(ID_ADC);
// 			 _pmc_disable_periph_clock(ID_TC1_CHANNEL2);
// 			 _pmc_disable_periph_clock(ID_FLEXCOM0);
// 			 _pmc_disable_periph_clock(ID_FLEXCOM4);
// 			 _pmc_disable_periph_clock(ID_FLEXCOM7);
// 			 _pmc_disable_periph_clock(ID_PIOA);
//			 _pmc_disable_periph_clock(ID_PIOB);
// 			 sleep(0);
// 	 }
// }
void setup_n_check_connections_cb_(void* param, void* param_2){

// 	gpio_set_pin_level(LED_G,false);
// 	gpio_set_pin_level(LED_R,true);
// 	blinkLED = LED_R;
	scheduler_add_task_(&blinking_task);
	//Setup SD Card
	#ifdef SAVE_TO_SD
	
	if(init_SD_() == 1)
		DEBUG_print("SD Card initialization complete\n\n");
	else
		DEBUG_print("\nError: SD Card not found \n");
	#endif
	
	//Setup Temperature Sensor
	if(setup_BoardTemp_() == 0)
		DEBUG_print("\nTemperature Sensor initialization complete\n");
	else
		DEBUG_print("\nError: Temperature Sensor not found \n");
	//Setup IMU
	
	if(IMU_setup_() == 0)
		DEBUG_print("\nIMU initialization complete \n");
	else
		DEBUG_print("\nError: IMU not found \n");
		
	//Setup GPS Module
	if(setup_GPS_() == 0)
		DEBUG_print("\n\nGPS initialization complete\n");
	else
		 DEBUG_print("\nError: GPS not found \n");
		 
	//DEBUG_print("======================\n");
	
// 	gpio_set_pin_level(LED_R,false);
// 	gpio_set_pin_level(LED_G,true);
// 	blinkLED = LED_G;
	
	scheduler_add_task_(&setup_completed_update_measuring_task);
}
/*******************************************************************************************************/
void blinking_timer_cb_(void* param, void*param_2){
	
	blinkCounter++;
	bool LED_pin_level = gpio_get_pin_level(blinkLED);
	if (blinkCounter > 1 && LED_pin_level){
		gpio_set_pin_level(blinkLED,false);			
		blinkCounter = 0;
		
	}else if (blinkCounter > 30 && !LED_pin_level){
		gpio_set_pin_level(blinkLED,true);			
		blinkCounter = 0;
	}
}
/*******************************************************************************************************/
void setup_completed_update_measuring_cb_(void* param, void* param_2){
	
// 	gpio_set_pin_level(LED_G,true);			
// 	gpio_set_pin_level(LED_R,true);			
// 	blinkLED = LED_R;
	
	//add measuring scheduled tasks
 	scheduler_add_task_(&TempSensor_update_task);
 	scheduler_add_task_(&GPS_update_task);
 	scheduler_add_task_(&IMU_update_task);
	//scheduler_add_task_(&check_onboard_flags_task);
}

/************************************************************************************************************/
/*																											*/
/*											 Main application											  	*/
/*																										    */
/************************************************************************************************************/

int main(void){
	
	/* Initializes MCU, drivers and middle ware */
	atmel_start_init();

	/* Setup communication interfaces like UART and I2C */
	DEBUG_print_setup();
	//DEBUG_print("\n======================\n");
	DEBUG_print("Hello World...Program Start!\n");
	//DEBUG_print("======================\n");
	
	/* Setup Leds */
	gpio_set_pin_level(LEDS_ON,true);
	gpio_set_pin_level(LED_G,false);
	gpio_set_pin_level(LED_R,false);	
		
	i2c_m_sync_get_io_descriptor(&I2C_INT, &I2C_INT_io);
	i2c_m_sync_enable(&I2C_INT);
	
	/* external communication configurations */
	buffer_init_(MAX_DATASETS, sizeof(sensorsDatasets_buffer[0]), sensorsDatasets_buffer);
 	comm_handler_mcu_ctor_(&slaveMCU , SLAVE_MCU_ADDRESS, SLEEP_POWERDOWN_MODE, SLEEP_HOURS, SLEEP_MINUTES, SLEEP_SECONDS);
 	fsm_mcu_ctor_(&fsm_activeMCU, &slaveMCU);

	/* config scheduled tasks */
	config_fsm_scheduled_tasks_(&fsm_activeMCU);
 	task_config_setup_n_check_onboard_connections_();
 	config_SDCard_tasks_();
 	config_TempSensor_tasks_();
 	config_GPS_tasks_();
 	config_IMU_tasks_();
 	task_config_blinking_();
 	task_config_setup_completed_update_measuring_();

	//task_config_check_onboard_flags_();
	fsm_init_(&fsm_activeMCU, fsm_mcu_startup_);
	scheduler_init_();

	/* add scheduled tasks */
	scheduler_add_task_(&setup_n_check_onboard_connections_task);
 	scheduler_run_();
	return 0;
}



	