/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>
#include <hpl_pmc.h>

#include <hal_adc_async.h>

#include <hal_delay.h>
#include <hal_timer.h>
#include <tc_lite.h>
#include <hal_i2c_m_sync.h>
#include <hal_usart_async.h>
#include <hal_usart_async.h>

extern struct adc_async_descriptor ADC_0;

/* The enabled channel for ADC */
#define CONF_ADC_0_CHANNEL_0 0

extern struct timer_descriptor SLEEP_TIMER;

extern struct i2c_m_sync_desc        I2C_INT;
extern struct usart_async_descriptor USART_GNSS;
extern struct usart_async_descriptor FLX7_USART;

void delay_driver_init(void);

void SCHEDULER_CLOCK_init(void);
void SCHEDULER_PORT_init(void);

void I2C_INT_CLOCK_init(void);
void I2C_INT_init(void);
void I2C_INT_PORT_init(void);

void USART_GNSS_PORT_init(void);
void USART_GNSS_CLOCK_init(void);
void USART_GNSS_init(void);

void FLX7_USART_PORT_init(void);
void FLX7_USART_CLOCK_init(void);
void FLX7_USART_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
