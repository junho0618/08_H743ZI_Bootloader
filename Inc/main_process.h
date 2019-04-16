#ifndef __MAIN_PROCESS_H
#define __MAIN_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define	VERIFY_Pin								BTN_USER_Pin
#define	VERIFY_GPIO_Port						BTN_USER_GPIO_Port

/*---------------------------------------------------------------------------------------------
     Functions
---------------------------------------------------------------------------------------------*/
void MainProcess( void );

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_PROCESS_H */