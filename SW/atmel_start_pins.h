/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMG has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define GNSS_RESET GPIO(GPIO_PORTA, 0)
#define LEDS_ON GPIO(GPIO_PORTA, 1)
#define GNSS_ON GPIO(GPIO_PORTA, 3)
#define GNSS_1PPS GPIO(GPIO_PORTA, 4)
#define I2C_INT_SCL GPIO(GPIO_PORTA, 9)
#define I2C_INT_SDA GPIO(GPIO_PORTA, 10)
#define SD_CS GPIO(GPIO_PORTA, 11)
#define SD_CD GPIO(GPIO_PORTA, 12)
#define SD_OFF GPIO(GPIO_PORTA, 13)
#define V_BATT_SW_0758 GPIO(GPIO_PORTA, 17)
#define KIM_ON GPIO(GPIO_PORTA, 18)
#define I2C_SENSOR_3V3_OFF GPIO(GPIO_PORTA, 21)
#define EXT_1V8_OFF GPIO(GPIO_PORTA, 22)
#define EXT_3V3_OFF GPIO(GPIO_PORTA, 23)
#define FLX7_UART_RX GPIO(GPIO_PORTA, 27)
#define FLX7_UART_TX GPIO(GPIO_PORTA, 28)
#define FLX7_SCK_IO GPIO(GPIO_PORTA, 29)
#define EXT_5V_ON GPIO(GPIO_PORTA, 30)
#define SLEEP_WARN GPIO(GPIO_PORTA, 31)
#define GNSS_RX GPIO(GPIO_PORTB, 8)
#define GNSS_TX GPIO(GPIO_PORTB, 9)
#define SD_MOSI_DI GPIO(GPIO_PORTB, 10)
#define SD_MISO_DO GPIO(GPIO_PORTB, 11)
#define SD_SCK GPIO(GPIO_PORTB, 13)
#define LED_G GPIO(GPIO_PORTB, 14)
#define LED_R GPIO(GPIO_PORTB, 15)

#endif // ATMEL_START_PINS_H_INCLUDED
