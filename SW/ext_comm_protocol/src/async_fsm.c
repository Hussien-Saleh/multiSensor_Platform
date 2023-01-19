/*
 * @brief: Asynchronous Event-driven state machines for Active Objects
 * @file: async_fsm.c
 * @details: Framework that describes the states of the connected external slave unit using event-driven active objects paradigm approach 
			 with a scheduler that acts as an event dispatcher and a ring buffer queue management to process the data of sensor channels 
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/
#include "ext_comm_protocol/include/async_fsm.h"

uint8_t checkStateCount = 0;
//static volatile bool slaveMCU_measuringDone = false;

/* sensors measuring counters before sleep mode */
static uint16_t sensor_0_measuringCounter = 0; 
static uint16_t sensor_1_measuringCounter = 0;
static uint16_t sensor_2_measuringCounter = 0;
static uint16_t sensor_3_measuringCounter = 0;
/* scheduled tasks */
static scheduled_task_description_ error_state_timeout_task;
static scheduled_task_description_ mcu_startup_task;
static scheduled_task_description_ mcu_reset_task;
static scheduled_task_description_ sensor_0_measuring_task;
static scheduled_task_description_ sensor_1_measuring_task;
static scheduled_task_description_ sensor_2_measuring_task;
static scheduled_task_description_ sensor_3_measuring_task;
static scheduled_task_description_ check_sensors_state_task;



/*******************************************************************************************************/
/*								FSM Initialization and Update			         					   */
/*******************************************************************************************************/
void fsm_init_(activeObjectFsm_* me, state_ initalState){
	
	//DEBUG_print("\n\nFSM is initialized\n");
	eventDispatcher_ entryEvent = {.signal = ENTRY};
	me->currentState = initalState;
	me->currentState(me, &entryEvent);
}
/*******************************************************************************************************/
void fsm_dispatch_(activeObjectFsm_* me, eventDispatcher_ *e){
	
	static eventDispatcher_ entryEvent = {.signal = ENTRY};
	static eventDispatcher_ exitEvent = {.signal = EXIT};
	state_ s = me->currentState;
	fsmReturnStatus r = me->currentState(me, e);
	
	if(r == RET_EVT_TRANSITION){
		s(me , &exitEvent); //call exit action of last state
		me->currentState(me, &entryEvent); //call entry action of new state
	}
}
/*******************************************************************************************************/
void fsm_mcu_ctor_(activeObjectFsm_* me, mcu_ *mcu_obj){	
	me->mcu = mcu_obj;
}
/*******************************************************************************************************/
void config_fsm_scheduled_tasks_(activeObjectFsm_* me){
	
	task_config_mcu_startup_(me);
	task_config_error_state_timeout_(me);
	task_config_sensor_0_measuring_(me);
	task_config_sensor_1_measuring_(me);
	task_config_sensor_2_measuring_(me);
	task_config_sensor_3_measuring_(me);
	task_config_check_sensors_state_(me);
}
/*******************************************************************************************************/
/*										 Events Dispatchers											   */
/*******************************************************************************************************/
void event_dispatch_error_state_timeout_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = ERROR_STATE_TIMEOUT};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_mcu_startup_completed_ (void* param, void* param_2){
	eventDispatcher_ e = {.signal = MCU_STARTUP_COMPLETED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_mcu_reset_completed_ (void* param, void* param_2){
	eventDispatcher_ e = {.signal = MCU_RESET_COMPLETED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_error_sensors_unchecked_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = ERROR_SENSORS_UNCHECKED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_sensors_checked_mcu_be_ready_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = SENSORS_CHECKED_MCU_READY_MODE_DISPATCHED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_mcu_start_measuring_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = MCU_START_MEASURING_DISPATCHED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_mcu_measuring_done_goto_sleep_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = MCU_MEASURING_DONE_SLEEP_MODE_DISPATCHED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
void event_dispatch_mcu_sleeping_done_be_ready_(void* param, void* param_2){
	eventDispatcher_ e = {.signal = MCU_SLEEPING_DONE_READY_MODE_DISPATCHED};
	fsm_dispatch_((activeObjectFsm_*)param, &e);
}
/*******************************************************************************************************/
/*										 Scheduled Tasks Configuration											   */
/*******************************************************************************************************/
void task_config_mcu_startup_(activeObjectFsm_* me){
	mcu_startup_task.task = (task_) &event_dispatch_mcu_startup_completed_;
	mcu_startup_task.param = (void*) me;
	mcu_startup_task.param_2 = NULL;
	mcu_startup_task.time_expire = 40*SECOND_TO_MILLI; //40s
	mcu_startup_task.time_after_fire = 0;
}
/*******************************************************************************************************/
void task_config_error_state_timeout_(activeObjectFsm_* me){
	error_state_timeout_task.task = (task_) &event_dispatch_error_state_timeout_;
	error_state_timeout_task.param = (void*) me;
	error_state_timeout_task.param_2 = NULL;
	error_state_timeout_task.time_expire = 2*HOUR_TO_MILLI; //ms
	error_state_timeout_task.time_after_fire = 2*HOUR_TO_MILLI;
}
/*******************************************************************************************************/
void task_config_sensor_0_measuring_(activeObjectFsm_* me){
	sensor_0_measuring_task.task = (task_) &start_sensor_processing_;
	sensor_0_measuring_task.param = (void*) me;
	sensor_0_measuring_task.param_2 = (void*) (&me->mcu->sensors[SENSOR_0].sensorIDNumber);
	sensor_0_measuring_task.time_expire = SENSOR_0_TRIGGER_TIME*SECOND_TO_MILLI; 
	sensor_0_measuring_task.time_after_fire = SENSOR_0_TRIGGER_TIME*SECOND_TO_MILLI;
}
/*******************************************************************************************************/
void task_config_sensor_1_measuring_(activeObjectFsm_* me){
	sensor_1_measuring_task.task = (task_) &start_sensor_processing_;
	sensor_1_measuring_task.param = (void*) me;
	sensor_1_measuring_task.param_2 = (void*) (&me->mcu->sensors[SENSOR_1].sensorIDNumber);
	sensor_1_measuring_task.time_expire = SENSOR_1_TRIGGER_TIME*SECOND_TO_MILLI;
	sensor_1_measuring_task.time_after_fire = SENSOR_1_TRIGGER_TIME*SECOND_TO_MILLI;
}
/*******************************************************************************************************/
void task_config_sensor_2_measuring_(activeObjectFsm_* me){
	sensor_2_measuring_task.task = (task_) &start_sensor_processing_;
	sensor_2_measuring_task.param = (void*) me;
	sensor_2_measuring_task.param_2 = (void*) (&me->mcu->sensors[SENSOR_2].sensorIDNumber);
	sensor_2_measuring_task.time_expire = SENSOR_2_TRIGGER_TIME*SECOND_TO_MILLI;
	sensor_2_measuring_task.time_after_fire = SENSOR_2_TRIGGER_TIME*SECOND_TO_MILLI;
}
/*******************************************************************************************************/
void task_config_sensor_3_measuring_(activeObjectFsm_* me){
	sensor_3_measuring_task.task = (task_) &start_sensor_processing_;
	sensor_3_measuring_task.param = (void*) me;
	sensor_3_measuring_task.param_2 = (void*) (&me->mcu->sensors[SENSOR_3].sensorIDNumber);
	sensor_3_measuring_task.time_expire = SENSOR_3_TRIGGER_TIME*SECOND_TO_MILLI;
	sensor_3_measuring_task.time_after_fire = SENSOR_3_TRIGGER_TIME*SECOND_TO_MILLI;
}
/*******************************************************************************************************/
void task_config_check_sensors_state_(activeObjectFsm_* me){
	check_sensors_state_task.task = (task_) &check_sensors_state_cb_;
	check_sensors_state_task.param = (void*) me;
	check_sensors_state_task.param_2 = NULL;
	check_sensors_state_task.time_expire = 1*SECOND_TO_MILLI;
	check_sensors_state_task.time_after_fire = 1*SECOND_TO_MILLI;
}


/*******************************************************************************************************/
/*										 Helper Functions								         	   */
/*******************************************************************************************************/
uint8_t check_num_available_sensors_(activeObjectFsm_* me){
	
	//if number of sensors is checked, return success
	if(comm_handler_get_num_sensors_on_mcu_(me->mcu) == RET_SUCCESS){
		me->mcu->numberSensorsChecked = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t create_sensors_(activeObjectFsm_* me){
	
	if(check_num_available_sensors_(me) == RET_SUCCESS){
		//create sensor with initial parameters based on the number of sensor
		for(uint8_t count = 0; count < me->mcu->totalSensorsAvailable; count++){
			comm_handler_sensor_ctor_(me->mcu, count);
		}
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t reset_slave_mcu_(activeObjectFsm_* me){
	
	if(comm_handler_mcu_reset_(me->mcu) == RET_SUCCESS){
		me->mcu->isMcuReset = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t put_slave_mcu_into_ready_mode_(activeObjectFsm_* me){
	
	if(comm_handler_mcu_be_ready_(me->mcu) == RET_SUCCESS){
		me->mcu->isMcuReady = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t activate_sensor_(activeObjectFsm_* me, uint8_t sensorIDx){
	
	if(comm_handler_activate_sensor_(me->mcu, sensorIDx) == RET_SUCCESS){
		me->mcu->sensors[sensorIDx].isSensorActivated = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
uint8_t get_sensor_max_measure_time_(activeObjectFsm_* me, uint8_t sensorIDx){
	
	if(comm_handler_sensor_get_max_measuring_time_(me->mcu, sensorIDx) == RET_SUCCESS){
		me->mcu->sensors[sensorIDx].isMaxMeasureTimeChecked = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
void start_sensor_processing_(void* me, void* sensorNum){
		
	activeObjectFsm_ *tempObj = (activeObjectFsm_ *)me;
	uint8_t *idx = (uint8_t *)sensorNum;
	
	if(*idx < tempObj->mcu->totalSensorsAvailable){
		if(comm_handler_sensor_trigger_measurement_(tempObj->mcu, *idx) == RET_SUCCESS){
			tempObj->mcu->sensors[*idx].isMeasurementTriggered = false;
			switch(*idx){
				
				case SENSOR_0:				
					delay_ms(tempObj->mcu->sensors[SENSOR_0].maxMeasurementTime);
					check_measurement_status_and_retrieve_(tempObj, idx);
					break;
					
				case SENSOR_1:
					delay_ms(tempObj->mcu->sensors[SENSOR_1].maxMeasurementTime);
					check_measurement_status_and_retrieve_(tempObj, idx);
					break;
					
				case SENSOR_2:
					delay_ms(tempObj->mcu->sensors[SENSOR_2].maxMeasurementTime);
					check_measurement_status_and_retrieve_(tempObj, idx);
					break;
					
				case SENSOR_3:
					delay_ms(tempObj->mcu->sensors[SENSOR_3].maxMeasurementTime);
					check_measurement_status_and_retrieve_(tempObj, idx);
					break;
			}
		}
	}
}
/*******************************************************************************************************/
void check_measurement_status_and_retrieve_(void* me, void *sensorNum){
	
	activeObjectFsm_ *tempObj = (activeObjectFsm_ *)me;
	uint8_t *idx = (uint8_t *)sensorNum;
	sensor_dataset_ data_set; //temporary dataset to store the data

	//after callback it will call the next process
	if(comm_handler_is_sensor_measurement_ready_(tempObj->mcu, *idx) == RET_SUCCESS){
		tempObj->mcu->sensors[*idx].isMeasurementReady = false;
		if(comm_handler_sensor_retrieve_measurement_(tempObj->mcu, *idx) == RET_SUCCESS){
			tempObj->mcu->sensors[*idx].isMeasurementRetrieved = false;
			data_set.active_sensor_idx = tempObj->mcu->sensors[*idx].sensorIDNumber;
			memcpy(data_set.timestamp ,tempObj->mcu->sensors[*idx].measurementTimestamp, EIGHT_BYTES);
			data_set.data_size = tempObj->mcu->sensors[*idx].measurementSize;
			data_set.data_type = tempObj->mcu->sensors[*idx].measurementType;
			memcpy(data_set.data, tempObj->mcu->sensors[*idx].measurementData, tempObj->mcu->sensors[*idx].measurementSize);
		
			//enqueue retrieved data
			if((enqueue_data_((void*) &data_set)) == RET_SUCCESS){
				
				DEBUG_print("\n\nExternal Sensor data enqueued\n");

				if(*idx == SENSOR_0){
					sensor_0_measuringCounter++;
					if(sensor_0_measuringCounter == SENSOR_0_MAX_MEASURE_COUNT){
						scheduler_remove_task_(&sensor_0_measuring_task);
						sensor_0_measuringCounter = 0;
						checkStateCount++;
						DEBUG_print("\n\nSensor 0 measuring task removed\n");
					}			
				}
				if(*idx == SENSOR_1){
					sensor_1_measuringCounter++;
					if(sensor_1_measuringCounter == SENSOR_1_MAX_MEASURE_COUNT){
						scheduler_remove_task_(&sensor_1_measuring_task);
						sensor_1_measuringCounter = 0;
						checkStateCount++;
						DEBUG_print("\n\nSensor 1 measuring task removed\n");
					}
				}
				if(*idx == SENSOR_2){
					sensor_2_measuringCounter++;
					if(sensor_2_measuringCounter == SENSOR_2_MAX_MEASURE_COUNT){
						scheduler_remove_task_(&sensor_2_measuring_task);
						sensor_2_measuringCounter = 0;
						checkStateCount++;
						DEBUG_print("\n\nSensor 2 measuring task removed\n");
					}
				}
				if(*idx == SENSOR_3){
					sensor_3_measuringCounter++;
					if(sensor_3_measuringCounter == SENSOR_3_MAX_MEASURE_COUNT){
						scheduler_remove_task_(&sensor_3_measuring_task);
						sensor_3_measuringCounter = 0;
						checkStateCount++;
						DEBUG_print("\n\nSensor 3 measuring task removed\n");
					}
				}
			}
		}
	}
}
/*******************************************************************************************************/
uint8_t put_slave_mcu_into_sleep_mode_(activeObjectFsm_* me){
	
	if(comm_handler_mcu_sleep_(me->mcu) == RET_SUCCESS){
		me->mcu->isMcuAsleep = false;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
/*******************************************************************************************************/
void check_sensors_state_cb_(void* param, void* param_2){
	
	activeObjectFsm_ *tempObj = (activeObjectFsm_ *)param;
	
	if(NUM_SELECTED_SENSORS <= tempObj->mcu->totalSensorsAvailable){
		
		if(checkStateCount == NUM_SELECTED_SENSORS){
			checkStateCount = 0;
			event_dispatch_mcu_measuring_done_goto_sleep_(param, param_2);
		}
	}
	else{
		
		if(checkStateCount == tempObj->mcu->totalSensorsAvailable){
			checkStateCount = 0;
			event_dispatch_mcu_measuring_done_goto_sleep_(param, param_2);
		}
		
	}
	
}
/*******************************************************************************************************/
/*										 State Functions											   */
/*******************************************************************************************************/
fsmReturnStatus fsm_mcu_startup_ (activeObjectFsm_* me, eventDispatcher_ *e){
	
	switch(e->signal){
		case ENTRY:
		
			scheduler_add_task_(&mcu_startup_task);
			return RET_EVT_HANDLED;
		
// 		case EXIT:	
// 			DEBUG_print("\n\nExit: Startup state\n");
// 			return RET_EVT_HANDLED;
// 			
		case MCU_STARTUP_COMPLETED:
					
			//scheduler_remove_task_(&mcu_startup_task);
			return TRANSITION(fsm_sensors_check_n_create_);
			
		default:
			return RET_EVT_IGNORED;
	}
}
/*******************************************************************************************************/
fsmReturnStatus fsm_sensors_check_n_create_ (activeObjectFsm_* me, eventDispatcher_ *e){
	
	switch(e->signal){
		case ENTRY:
			scheduler_add_task_(&error_state_timeout_task);
			if(create_sensors_(me) == RET_SUCCESS){
				uint8_t count_check = 0;
				for(uint8_t k = 0; k < me->mcu->totalSensorsAvailable; k++){
					if(activate_sensor_(me, k) == RET_SUCCESS){
						if(get_sensor_max_measure_time_(me, k) == RET_SUCCESS){
							count_check++;
						}
					}
				}
				if(count_check == me->mcu->totalSensorsAvailable){
					event_dispatch_sensors_checked_mcu_be_ready_(me, NULL);
					//scheduler_add_task_(&sensors_checked_mcu_be_ready_to_measure_task);
				}	
			}else{
				event_dispatch_error_sensors_unchecked_(me, NULL);
				//scheduler_add_task_(&error_sensors_unchecked_task);
			}
			return RET_EVT_HANDLED;
		
		case EXIT:
			//DEBUG_print("\n\nExit: Check Sensors state\n");
			scheduler_remove_task_(&error_state_timeout_task);		
			return RET_EVT_HANDLED;
		
		case ERROR_STATE_TIMEOUT:
			scheduler_remove_task_(&error_state_timeout_task);
			return TRANSITION(fsm_error_);

		case SENSORS_CHECKED_MCU_READY_MODE_DISPATCHED:
			scheduler_remove_task_(&error_state_timeout_task);
			return TRANSITION(fsm_mcu_ready_);
			
		case ERROR_SENSORS_UNCHECKED:
			scheduler_remove_task_(&error_state_timeout_task);
			return TRANSITION(fsm_error_);

		default:
			return RET_EVT_IGNORED;
	}
}
/*******************************************************************************************************/
fsmReturnStatus fsm_error_ (activeObjectFsm_* me, eventDispatcher_ *e){
	
	switch(e->signal){	
		case ENTRY:
			if(reset_slave_mcu_(me) == RET_SUCCESS){
				event_dispatch_mcu_reset_completed_(me, NULL);
			}
			return RET_EVT_HANDLED;
		
		case EXIT:
			return RET_EVT_HANDLED;
		
		case MCU_RESET_COMPLETED:
			return TRANSITION(fsm_mcu_startup_);
		
		default:
			return RET_EVT_IGNORED;
	}
}
/*******************************************************************************************************/
fsmReturnStatus fsm_mcu_ready_(activeObjectFsm_* me, eventDispatcher_ *e){
	
	switch(e->signal){
		case ENTRY:	
			scheduler_add_task_(&error_state_timeout_task);
			scheduler_add_task_(&check_sensors_state_task);
			//put the slave microcontroller into ready mode to measure
			if(put_slave_mcu_into_ready_mode_(me) == RET_SUCCESS){				
				//DEBUG_print("MCU is ready now");
				event_dispatch_mcu_start_measuring_(me, NULL);
			}
			return RET_EVT_HANDLED;
		  
		case EXIT:	  
		  	//DEBUG_print("\n\nExit: MCU ready state\n");
		  	scheduler_remove_task_(&error_state_timeout_task);
		  	return RET_EVT_HANDLED;
		  		  
		case ERROR_STATE_TIMEOUT:
		    scheduler_remove_task_(&error_state_timeout_task);
			return TRANSITION(fsm_error_);
			
		case MCU_START_MEASURING_DISPATCHED:
			//DEBUG_print("MCU start measuring");
			scheduler_remove_task_(&error_state_timeout_task);
			return TRANSITION(fsm_mcu_measuring_);
			 
		default:
			return RET_EVT_IGNORED;
	}
}
/*******************************************************************************************************/
fsmReturnStatus fsm_mcu_measuring_(activeObjectFsm_* me, eventDispatcher_ *e){
	
	switch(e->signal){
		case ENTRY:
			if((SENSOR_0_SELECTION_STATUS == SELECTED) && (SENSOR_0 < me->mcu->totalSensorsAvailable)){
				scheduler_add_task_(&sensor_0_measuring_task);
				DEBUG_print("\n\nSensor 0 scheduled task added\n");
			}
			if((SENSOR_1_SELECTION_STATUS == SELECTED) && (SENSOR_1 < me->mcu->totalSensorsAvailable)){
				scheduler_add_task_(&sensor_1_measuring_task);
				DEBUG_print("\n\nSensor 1 scheduled task added\n");
			}	
 			if((SENSOR_2_SELECTION_STATUS == SELECTED) && (SENSOR_2 < me->mcu->totalSensorsAvailable)){
				scheduler_add_task_(&sensor_2_measuring_task);
				DEBUG_print("\n\nSensor 2 scheduled task added\n");
			}		
			if((SENSOR_3_SELECTION_STATUS == SELECTED) && (SENSOR_3 < me->mcu->totalSensorsAvailable)){
				scheduler_add_task_(&sensor_3_measuring_task);
				DEBUG_print("\n\nSensor 3 scheduled task added\n");
				
			}
			return RET_EVT_HANDLED;
		
// 		case EXIT:
// 			//scheduler_remove_task_(&error_state_timeout_task);
// 			return RET_EVT_HANDLED;
// 				
		case MCU_MEASURING_DONE_SLEEP_MODE_DISPATCHED:
			//scheduler_remove_task_(&error_state_timeout_task);
			//DEBUG_print("measuring of slave MCU is done, goto sleep ");
			return TRANSITION(fsm_mcu_sleep_);
		
		default:
			return RET_EVT_IGNORED;
	}
}
/*******************************************************************************************************/
fsmReturnStatus fsm_mcu_sleep_(activeObjectFsm_*me, eventDispatcher_ *e){
	
	switch(e->signal){
		case ENTRY:
			scheduler_add_task_(&error_state_timeout_task);
			if(put_slave_mcu_into_sleep_mode_(me) == RET_SUCCESS){
				
				DEBUG_print("\nsleep mode...\n");
				delay_ms((SLEEP_HOURS*HOUR_TO_MILLI) + (SLEEP_MINUTES*MINUTE_TO_MILLI) + (SLEEP_SECONDS*SECOND_TO_MILLI));
				event_dispatch_mcu_sleeping_done_be_ready_(me, NULL);
				//slaveMCU_measuringDone = true;
			}
// 		
			return RET_EVT_HANDLED;
		
		case EXIT:
 			scheduler_remove_task_(&error_state_timeout_task);
			return RET_EVT_HANDLED;
		
 		case MCU_SLEEPING_DONE_READY_MODE_DISPATCHED:
// 			
			scheduler_remove_task_(&error_state_timeout_task);
			DEBUG_print("\n\MCU is now awake and ready to measure again.....\n");
			return TRANSITION(fsm_mcu_measuring_);

		default:
			return RET_EVT_IGNORED;
	}
}

// bool ret_slaveMCU_status(){
// 	
// 	return slaveMCU_measuringDone;
// }



/*******************************************************************************************************/