/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_tc.h>

#include <hpl_usart_base.h>

#include <hpl_usart_base.h>

/* The channel amount for ADC */
#define ADC_0_CH_AMOUNT 1

/* The buffer size for ADC */
#define ADC_0_CH0_BUF_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_0_CH_MAX 0

/*! The buffer size for USART */
#define USART_GNSS_BUFFER_SIZE 16
/*! The buffer size for USART */
#define FLX7_USART_BUFFER_SIZE 16

struct adc_async_descriptor ADC_0;
#if ADC_0_CH_AMOUNT < 1
/* Avoid compiling errors. */
struct adc_async_channel_descriptor ADC_0_ch[1];
#warning none of ADC channel is enabled, please check
#else
struct adc_async_channel_descriptor ADC_0_ch[ADC_0_CH_AMOUNT];
#endif
struct timer_descriptor       SLEEP_TIMER;
struct usart_async_descriptor USART_GNSS;
struct usart_async_descriptor FLX7_USART;

static uint8_t ADC_0_ch0_buf[ADC_0_CH0_BUF_SIZE];

#ifdef ADC_0_CH_MAX
static uint8_t ADC_0_map[ADC_0_CH_MAX + 1];
#endif

static uint8_t USART_GNSS_buffer[USART_GNSS_BUFFER_SIZE];
static uint8_t FLX7_USART_buffer[FLX7_USART_BUFFER_SIZE];

struct i2c_m_sync_desc I2C_INT;

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
static void ADC_0_init(void)
{
	_pmc_enable_periph_clock(ID_ADC);
#ifdef ADC_0_CH_MAX
	adc_async_init(&ADC_0, ADC, ADC_0_map, ADC_0_CH_MAX, ADC_0_CH_AMOUNT, &ADC_0_ch[0], (void *)NULL);
#endif
	adc_async_register_channel_buffer(&ADC_0, CONF_ADC_0_CHANNEL_0, ADC_0_ch0_buf, ADC_0_CH0_BUF_SIZE);

	gpio_set_pin_function(V_BATT_SW_0758, GPIO_PIN_FUNCTION_OFF);
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void SLEEP_TIMER_PORT_init(void)
{
}
/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void SLEEP_TIMER_init(void)
{
	_pmc_enable_periph_clock(ID_TC0_CHANNEL0);
	SLEEP_TIMER_PORT_init();
	timer_init(&SLEEP_TIMER, TC0, _tc_get_timer());
}

void SCHEDULER_PORT_init(void)
{
}

void SCHEDULER_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_TC1_CHANNEL2);
}

void I2C_INT_PORT_init(void)
{

	gpio_set_pin_function(I2C_INT_SCL, MUX_PA9A_FLEXCOM0_TWCK);

	gpio_set_pin_function(I2C_INT_SDA, MUX_PA10A_FLEXCOM0_TWD);
}

void I2C_INT_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_FLEXCOM0);
}

void I2C_INT_init(void)
{
	I2C_INT_CLOCK_init();

	i2c_m_sync_init(&I2C_INT, FLEXCOM0);

	I2C_INT_PORT_init();
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_GNSS_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_FLEXCOM4);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_GNSS_PORT_init()
{

	gpio_set_pin_function(GNSS_TX, MUX_PB9A_FLEXCOM4_RXD);

	gpio_set_pin_function(GNSS_RX, MUX_PB8A_FLEXCOM4_TXD);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void USART_GNSS_init(void)
{
	USART_GNSS_CLOCK_init();
	USART_GNSS_PORT_init();
	usart_async_init(&USART_GNSS, FLEXCOM4, USART_GNSS_buffer, USART_GNSS_BUFFER_SIZE, _usart_get_usart_async());
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void FLX7_USART_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_FLEXCOM7);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void FLX7_USART_PORT_init()
{

	gpio_set_pin_function(FLX7_UART_RX, MUX_PA27B_FLEXCOM7_RXD);

	gpio_set_pin_function(FLX7_UART_TX, MUX_PA28B_FLEXCOM7_TXD);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void FLX7_USART_init(void)
{
	FLX7_USART_CLOCK_init();
	FLX7_USART_PORT_init();
	usart_async_init(&FLX7_USART, FLEXCOM7, FLX7_USART_buffer, FLX7_USART_BUFFER_SIZE, _usart_get_usart_async());
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	_pmc_enable_periph_clock(ID_PIOB);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA0 */

	gpio_set_pin_level(GNSS_RESET,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GNSS_RESET, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GNSS_RESET, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA1 */

	gpio_set_pin_level(LEDS_ON,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(LEDS_ON, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LEDS_ON, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA3 */

	gpio_set_pin_level(GNSS_ON,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GNSS_ON, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GNSS_ON, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA4 */

	// Set pin direction to input
	gpio_set_pin_direction(GNSS_1PPS, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GNSS_1PPS,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(GNSS_1PPS, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA11 */

	gpio_set_pin_level(SD_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SD_CS, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA12 */

	// Set pin direction to input
	gpio_set_pin_direction(SD_CD, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SD_CD,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(SD_CD, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA13 */

	gpio_set_pin_level(SD_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SD_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SD_OFF, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA18 */

	gpio_set_pin_level(KIM_ON,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(KIM_ON, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(KIM_ON, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA21 */

	gpio_set_pin_level(I2C_SENSOR_3V3_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(I2C_SENSOR_3V3_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(I2C_SENSOR_3V3_OFF, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA22 */

	gpio_set_pin_level(EXT_1V8_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EXT_1V8_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EXT_1V8_OFF, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA23 */

	gpio_set_pin_level(EXT_3V3_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EXT_3V3_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EXT_3V3_OFF, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA29 */

	gpio_set_pin_level(FLX7_SCK_IO,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(FLX7_SCK_IO, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FLX7_SCK_IO, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA30 */

	gpio_set_pin_level(EXT_5V_ON,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EXT_5V_ON, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EXT_5V_ON, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA31 */

	// Set pin direction to input
	gpio_set_pin_direction(SLEEP_WARN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SLEEP_WARN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(SLEEP_WARN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB10 */

	gpio_set_pin_level(SD_MOSI_DI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SD_MOSI_DI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SD_MOSI_DI, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB11 */

	// Set pin direction to input
	gpio_set_pin_direction(SD_MISO_DO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SD_MISO_DO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(SD_MISO_DO, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB13 */

	gpio_set_pin_level(SD_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SD_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SD_SCK, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB14 */

	gpio_set_pin_level(LED_G,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(LED_G, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_G, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB15 */

	gpio_set_pin_level(LED_R,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(LED_R, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_R, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();

	delay_driver_init();
	SLEEP_TIMER_init();

	SCHEDULER_CLOCK_init();
	SCHEDULER_PORT_init();
	SCHEDULER_init();

	I2C_INT_init();
	USART_GNSS_init();
	FLX7_USART_init();
}
