#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#include <atmel_start.h>
#include "config.h"
//#include "../config.h"

void _DEBUG_print_setup(void);
void _DEBUG_print_len(uint8_t * print_arr, uint16_t str_len);
void _DEBUG_print(uint8_t * print_arr);
void _DEBUG_print_char(uint8_t * print_char);

#endif /* DEBUG_PRINT_H_ */