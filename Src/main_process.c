﻿ /*************************************************************  
 * NOTE : main_process.c
 *      Application Main Process
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/

#include "main_process.h"
#include "firmware_info.h"

void MainProcess( void )
{	
	printf( "Start Main Process!!! \r\n" );
	
	testFirmwareInfo();
	
#if 0	
	if( HAL_GPIO_ReadPin( BTN_USER_GPIO_Port, BTN_USER_Pin ) != GPIO_PIN_SET )
	{
		printf( "USBD_DFU_APP_DEFAULT_ADD..[0x%08x]\r\n", (*(__IO uint32_t*)USBD_DFU_APP_DEFAULT_ADD));
		if( ((*(__IO uint32_t*)USBD_DFU_APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000 )
		{
			printf( "APP Start..\r\n" );

			// 1. Disable Interrupt
			__disable_irq();

			// 2. Initialize user application's Stack Pointer
			__set_MSP(*(__IO uint32_t*)USBD_DFU_APP_DEFAULT_ADD);

			// 3. Initialize user application's VTOR Register
			SCB->VTOR=USBD_DFU_APP_DEFAULT_ADD;

			// 4. Jump to user application
			JumpAddress = *(__IO uint32_t*)(USBD_DFU_APP_DEFAULT_ADD + 4);
			Jump_To_Application = (pFunction)JumpAddress;
			Jump_To_Application();
		}
	}
  
	printf( "DFU Upgrade Mode Start..\r\n" );

	MX_USB_DEVICE_Init();
#endif	
}