/*************************************************************  
 * NOTE : debug.c
 *      uart-printf
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/

#include "debug.h"

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit( &PRINTF_UART_PORT, (uint8_t *)&ch, 1, 0xFFFF );
    return ch;
}