/*
 * @brief: Scheduler using hardware timer
 * @file: scheduler.h
 * @details: Scheduler used to manage the timing of tasks and events
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <atmel_start.h>
#include "config.h"

typedef struct scheduled_task_description_class_ scheduled_task_description_;
typedef struct human_readable_time_class_ human_readable_time_;
typedef uint64_t system_time_; //system time

typedef void (*task_) (void*, void*); // type of function pointer for tasks

struct scheduled_task_description_class_ {
	
	task_ task; //function pointer to call
	void* param; //pointer which is passed to task when it is executed
	void* param_2;
	uint32_t time_expire; //time offset in ms after which the task is called
	uint32_t time_after_fire; //period of timer after firing, 0 means execute once
	uint8_t task_execute:1; //for internal use
	uint8_t reserved:7;// reserved
	struct scheduled_task_description_class_* next_task; //pointer to next task, internal use
	
};
struct human_readable_time_class_{
	
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t millisecond;
};
/* Function to initialize the task scheduler, uses hardware timer */
void scheduler_init_();

/* Function to run scheduler in infinite loop */
void scheduler_run_();

/**
 * Function to add a new task to the scheduler, may be called from any context
 *
 * @param ptr_task_desc  Pointer to taskDescriptor structure. The scheduler takes
 *					possession of the memory pointed at by ptr_task_desc until the task
 *					is removed by scheduler_remove or -- in case of a 
 *					non-periodic task -- the task is executed. ptr_task_desc->expire 
 *					and ptr_task_desc->execute are changed to by the task scheduler.
 *
 * @return     false, if task is already scheduled or invalid (NULL)
 *             true, if task was successfully added to scheduler and 
 *             will be executed after ptr_task_desc->expire ms.
 */

bool scheduler_add_task_(scheduled_task_description_* ptr_task_desc);


/**
 * Function to remove a task from the scheduler.
 *
 * @param ptr_task_desc	pointer to task descriptor to remove
 */

void scheduler_remove_task_(scheduled_task_description_* ptr_task_desc);

/* Function to get the current system time in milliseconds */
system_time_ scheduler_get_time_();

/* Function to set the current system time in milliseconds */
void scheduler_set_time_(system_time_ sys_time);

/* Function to convert from human readable time to system machine time in milliseconds
 *
 *@param readable_time, human readable time
*/
system_time_ to_system_time_(human_readable_time_ readable_time);

/* Function to convert from system machine time in milliseconds to human readable time
 *
 *@param sys_time, system time in milliseconds
*/
human_readable_time_ from_system_time_(system_time_ sys_time);

#endif /* SCHEDULER_H_ */