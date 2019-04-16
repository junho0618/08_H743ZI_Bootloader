#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"	

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define	DEBUG								1

#define PRINTF_UART_PORT					huart2
#define PUTCHAR_PROTOTYPE					inline int fputc(int ch, FILE *f) 

#if ( DEBUG )
	#define jprintf( fmt, args... )			printf( fmt, ##args );																		// Normal Print
	#define	jiprintf( fmt, args... )		printf( "\033[33m" ); printf( fmt, ##args ); printf( "\033[0m" );							// Info Print
	#define	jeprintf( fmt, args... )		printf( "\033[31m" ); printf( "%s : " fmt, __FUNCTION__ , ##args ); printf( "\033[0m" );	// Error Print
#else
	#define jprintf(x...) do { } while (0)
	#define jiprintf(x...) do { } while (0)
	#define jeprintf(x...) do { } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */