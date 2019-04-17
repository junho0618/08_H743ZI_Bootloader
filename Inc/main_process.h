#ifndef __MAIN_PROCESS_H
#define __MAIN_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define	ENABLE_VERIFICATION_APP_JUMP			1
#define	ENABLE_IAP								1

#define	VERIFY_CHECK_Pin						BTN_USER_Pin
#define	VERIFY_CHECK_GPIO_Port					BTN_USER_GPIO_Port

#define	IAP_CHECK_Pin							BTN_USER_Pin
#define	IAP_CHECK_GPIO_Port						BTN_USER_GPIO_Port

/*---------------------------------------------------------------------------------------------
     Functions
---------------------------------------------------------------------------------------------*/
void MainProcess( void );

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_PROCESS_H */