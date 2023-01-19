/*
 * @brief: Scheduler using hardware timer
 * @file: scheduler.c
 * @details: Scheduler used to manage the timing of tasks and events
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut für Mechatronik (IMEK) - Technische Universität Hamburg Harburg (TUHH)
 *
*/

#include "scheduler/scheduler.h"

static scheduled_task_description_* task_list = NULL; //list of scheduled tasks
static volatile system_time_ current_time = 0;

/*******************************************************************************************************/
static void scheduler_update_(void){
	
	scheduled_task_description_* current_task = task_list; //pointer to the head of linked task list
	
	//traverse through the task list and update expire time
	while (current_task != NULL){
		
		if(current_task->time_expire == 0){
			
			current_task->task_execute = 1; //mark expired tasks for execution
				
			//reset expiration time of periodic tasks
			if(current_task->time_after_fire != 0){
				
				current_task->time_expire = current_task->time_after_fire;
			}
		}
		
		//if(current_task->time_expire > 0)
		(current_task->time_expire)--; // decrease expire time by 1ms
			
		current_task = current_task->next_task;
	}
	
	//set pointer to the head of the linked task list (NULL)
	current_task = task_list;
	
	CRITICAL_SECTION_ENTER()   
	current_time++;
	CRITICAL_SECTION_LEAVE() 
}
/*******************************************************************************************************/
void scheduler_init_(){
	
	//set timer callback
	tc_register_callback(TC1, 2, scheduler_update_);	
	//start timer
	start_timer(TC1, 2);

}
/*******************************************************************************************************/
void scheduler_run_(){
	
	task_ temp_current_task = NULL; //task to execute
	void* temp_param = NULL;//parameter to pass
	void* temp_param_2 = NULL;
	
	bool temp_task_execute = false; //execute flag
	
	while(1){
		
		scheduled_task_description_* current_task = task_list; 
		
		while(current_task != NULL){
			
			CRITICAL_SECTION_ENTER()
			if(current_task->task_execute){
				
				temp_current_task = current_task->task;//store the task
				temp_param = current_task->param;//store the parameter
				temp_param_2 = current_task->param_2;	
				temp_task_execute = true;//set the execute flag
				current_task->task_execute = 0; //reset the flag for the task
				
				//if the task is not periodic, remove it
				if(current_task->time_after_fire == 0){	
					
					scheduler_remove_task_(current_task);
				}
			}
						
			current_task = current_task->next_task;
			CRITICAL_SECTION_LEAVE()

			//execute the task
			if(temp_task_execute){
				
				temp_current_task(temp_param, temp_param_2);
				temp_task_execute = false;
			}			
		}
	}
}
/*******************************************************************************************************/
bool scheduler_add_task_(scheduled_task_description_* task_to_add){
	
	//check if invalid task is added to the list (NULL)
	if(task_to_add == NULL){
		return false;
	}
	
	CRITICAL_SECTION_ENTER()
	//check if the task list is empty
	if(task_list == NULL){
		//add the task
		task_list = task_to_add;
		task_to_add->next_task = NULL;
	}
	else{
		
		scheduled_task_description_* current_task = task_list; //point to the beginning of linked list
		
		//check if already scheduled in the first node
		if(current_task == task_to_add){
			return false;
		}
		//traverse through the linked list
		while(current_task->next_task != NULL){
			
			//check if the task is already scheduled
			if(current_task->next_task == task_to_add){
				return false;
			}
			current_task = current_task->next_task;		
		}
		//add the task
		current_task->next_task = task_to_add;
		task_to_add->next_task = NULL;			
	}
	CRITICAL_SECTION_LEAVE()
	return true;
}
/*******************************************************************************************************/
void scheduler_remove_task_(scheduled_task_description_* task_to_remove){
	
	//point to the beginning of the task list
	scheduled_task_description_* current_task = task_list;
	CRITICAL_SECTION_ENTER()
	//check if to be removed is the first node of the task list
	if(task_list == task_to_remove){
		
		task_list = task_to_remove->next_task;
	}
	else{
		//traverse through the task list
		while(current_task->next_task != NULL){
			
			//check if the next task is to be removed
			if(current_task->next_task == task_to_remove){
				
				current_task->next_task = task_to_remove->next_task; //remove task
				break;
			}
			current_task = current_task->next_task;
		}
	}
	CRITICAL_SECTION_LEAVE()	
}
/*******************************************************************************************************/
system_time_ scheduler_get_time_(){
	return current_time;
}
/*******************************************************************************************************/
void scheduler_set_time_(system_time_ time){
	
	CRITICAL_SECTION_ENTER()
	current_time = time;
	CRITICAL_SECTION_LEAVE()

}
/*******************************************************************************************************/
system_time_ to_system_time_(human_readable_time_ readable_time){
	
	system_time_ sys_time = (((readable_time.hour)*HOUR_TO_MILLI) + ((readable_time.minute)*MINUTE_TO_MILLI) + ((readable_time.second)*SECOND_TO_MILLI) + (readable_time.millisecond));
	return sys_time;
}
/*******************************************************************************************************/
human_readable_time_ from_system_time_(system_time_ sys_time){
	
	human_readable_time_ h_time;
	h_time.hour = (sys_time/(HOUR_TO_MILLI));
	sys_time = (sys_time - ((HOUR_TO_MILLI)*h_time.hour));
	h_time.minute = ((sys_time/MINUTE_TO_MILLI));
	sys_time = (sys_time - ((MINUTE_TO_MILLI)*h_time.minute));
	h_time.second = ((sys_time/SECOND_TO_MILLI));
	sys_time = (sys_time - ((SECOND_TO_MILLI)*h_time.second));
	h_time.millisecond = sys_time;
	return h_time;
}