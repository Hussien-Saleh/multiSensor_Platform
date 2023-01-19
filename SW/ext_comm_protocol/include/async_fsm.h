/*
 * @brief: Asynchronous Event-driven state machines for Active Objects
 * @file: async_fsm.h
 * @details: Framework that describes the states of the connected external slave unit using event-driven active objects paradigm approach 
			 with a scheduler that acts as an event dispatcher and a ring buffer queue management to process the data of sensor channels 
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/
#ifndef ASYNC_FSM_H_
#define ASYNC_FSM_H_

#include <atmel_start.h>
#include "config.h"
#include "ext_comm_protocol/include/comm_handler.h"
#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"


#define TRANSITION(newState) (me->currentState = newState, RET_EVT_TRANSITION)

typedef struct activeObjectFsmClass_  activeObjectFsm_;
typedef struct eventDispatcherClass_ eventDispatcher_;

enum {
	
	RET_EVT_HANDLED, //< event was handled
	RET_EVT_IGNORED, //< event was ignored; not used in this implementation
	RET_EVT_TRANSITION //< event was handled and a state transition occurred
};

/* possible event signals for the external mcu */
enum {
	ENTRY,
	EXIT,
	ERROR_STATE_TIMEOUT,
	MCU_STARTUP_COMPLETED,
	MCU_RESET_COMPLETED,
	ERROR_SENSORS_UNCHECKED,
	SENSORS_CHECKED_MCU_READY_MODE_DISPATCHED,
	MCU_START_MEASURING_DISPATCHED,
	MCU_MEASURING_DONE_SLEEP_MODE_DISPATCHED,
	MCU_SLEEPING_DONE_READY_MODE_DISPATCHED,
	
}eventSignals_;

//< typedef to be used with above enum
typedef uint8_t fsmReturnStatus; 

/** typedef for state event handler functions */
typedef fsmReturnStatus (*state_)(activeObjectFsm_*, eventDispatcher_*);

struct activeObjectFsmClass_ {
	state_ currentState; //< current state, pointer to event handler
	mcu_ *mcu; //active object related with the state machine
};

struct eventDispatcherClass_ {
	uint8_t signal; //< identifies the type of event
};
/*******************************************************************************************************/
/*									 FUNCTION PROTOTYPES											   */
/*******************************************************************************************************/
/*								FSM Initialization and Update			         					   */
/*******************************************************************************************************/
/* Function to initialize the state machine and set the first initial state for it
 *  @param me, pointer to the state machine
 *  @param initialState, initial state for the state machine
*/
void fsm_init_(activeObjectFsm_* me, state_ initalState);

/* Function to dispatch an event to the state machine
*  @param me, pointer to the state machine
*  @param e, pointer to the event signal
*/
void fsm_dispatch_(activeObjectFsm_* me, eventDispatcher_ *e);

/*  Function to create state machine for slave microcontroller unit 
 *  @param me, pointer to the state machine of the unit
 *  @param u, pointer to external sensor micro controller unit
*/
void fsm_mcu_ctor_(activeObjectFsm_* me, mcu_ *mcu_obj);

/* Function to configure scheduled tasks for state machines
 *  @param me, pointer to the state machine
*/
void config_fsm_scheduled_tasks_(activeObjectFsm_* me);

/*******************************************************************************************************/
/*										 Events Dispatchers											   */
/*******************************************************************************************************/

void event_dispatch_error_state_timeout_(void* param, void* param_2);
void event_dispatch_mcu_startup_completed_ (void* param, void* param_2);
void event_dispatch_mcu_reset_completed_ (void* param, void* param_2);
void event_dispatch_error_sensors_unchecked_(void* param, void* param_2);
void event_dispatch_sensors_checked_mcu_be_ready_(void* param, void* param_2);
void event_dispatch_mcu_start_measuring_(void* param, void* param_2);
void event_dispatch_mcu_measuring_done_goto_sleep_(void* param, void* param_2);
void event_dispatch_mcu_sleeping_done_be_ready_(void* param, void* param_2);

/*******************************************************************************************************/
/*										 Scheduled Tasks											   */
/*******************************************************************************************************/

void task_config_mcu_startup_(activeObjectFsm_* me);
void task_config_error_state_timeout_(activeObjectFsm_* me);
void task_config_mcu_reset_(activeObjectFsm_* me);
void task_config_sensor_0_measuring_(activeObjectFsm_* me);
void task_config_sensor_1_measuring_(activeObjectFsm_* me);
void task_config_sensor_2_measuring_(activeObjectFsm_* me);
void task_config_sensor_3_measuring_(activeObjectFsm_* me);
void task_config_check_sensors_state_(activeObjectFsm_* me);


/*******************************************************************************************************/
/*										 Helper Functions								         	   */
/*******************************************************************************************************/

uint8_t check_num_available_sensors_(activeObjectFsm_* me);
uint8_t create_sensors_(activeObjectFsm_* me);
uint8_t reset_slave_mcu_(activeObjectFsm_* me);
uint8_t put_slave_mcu_into_ready_mode_(activeObjectFsm_* me);
uint8_t activate_sensor_(activeObjectFsm_* me, uint8_t sensorIDx);
uint8_t get_sensor_max_measure_time_(activeObjectFsm_* me, uint8_t sensorIDx);
void start_sensor_processing_(void* me, void* sensorNum);
void check_measurement_status_and_retrieve_(void* me, void * sensorNum);
uint8_t put_slave_mcu_into_sleep_mode_(activeObjectFsm_* me);
void check_sensors_state_cb_(void* param, void* param_2);

// bool ret_slaveMCU_status();

/*******************************************************************************************************/
/*										 State Functions											   */
/*******************************************************************************************************/

fsmReturnStatus fsm_mcu_startup_ (activeObjectFsm_* me, eventDispatcher_ *e);
fsmReturnStatus fsm_sensors_check_n_create_ (activeObjectFsm_* me, eventDispatcher_ *e);
fsmReturnStatus fsm_error_ (activeObjectFsm_* me, eventDispatcher_ *e);
fsmReturnStatus fsm_mcu_ready_(activeObjectFsm_* me, eventDispatcher_ *e);
fsmReturnStatus fsm_mcu_measuring_(activeObjectFsm_* me, eventDispatcher_ *e);
fsmReturnStatus fsm_mcu_sleep_(activeObjectFsm_*me, eventDispatcher_ *e);


#endif 