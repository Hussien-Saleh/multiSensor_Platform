/*
 * @brief: SD card Driver
 * @file: SD.h
 * @details: Driver for SD card
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#ifndef SD_H_
#define SD_H_

#include <atmel_start.h>
#include "config.h"
#include "FatFs/ff.h"
#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"

scheduled_task_description_ SD_insertion_timeout_task;
/************************************************************************************************************/
/*											Function Prototypes												*/
/************************************************************************************************************/

void config_SDCard_tasks_();
void SD_insertion_timeout_task_config_();
void SD_insertion_timeout_(void* param, void* param_2);

uint8_t init_SD_();
void save_sensor_data_to_SD_(uint16_t* count);

void save_IMU_data_to_SD_(IMU_DataSet *d);

#endif 