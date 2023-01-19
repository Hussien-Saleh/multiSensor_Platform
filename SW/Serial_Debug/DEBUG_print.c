#include "Serial_Debug/DEBUG_print.h"

struct io_descriptor *UART_EXT_io;

volatile bool debug_tx_flag = false;

/************************************************************************/
/* All Serial functions DEBUG_print...()
	Serial print functions for printing debug messages
	using io_write on UART_EXT
                                                                       */
/************************************************************************/
void tx_cb_USART_DEBUG(const struct usart_async_descriptor *const io_descr){
	/* Transfer completed */
	debug_tx_flag = true;
}

void _DEBUG_print_len(uint8_t * print_arr, uint16_t str_len){
	
	debug_tx_flag = false;
	while (io_write(UART_EXT_io, print_arr, str_len) != str_len);
	while (!debug_tx_flag);
}

void _DEBUG_print(uint8_t * print_arr){
	
	debug_tx_flag = false;
	while (io_write(UART_EXT_io, print_arr, strlen(print_arr)) != strlen(print_arr));
	while (!debug_tx_flag);
}

void _DEBUG_print_char(uint8_t * print_char){
	
	while (io_write(UART_EXT_io, print_char, 1) != 1);
}


/************************************************************************/
/* DEBUG_print_setup()
	Initialization of all DEBUG Serial Print							*/
/************************************************************************/
void _DEBUG_print_setup(){
	
	// initialize FLX7_USART
	usart_async_register_callback(&FLX7_USART, USART_ASYNC_TXC_CB, tx_cb_USART_DEBUG);
	usart_async_get_io_descriptor(&FLX7_USART, &UART_EXT_io);
	usart_async_enable(&FLX7_USART);		
}
