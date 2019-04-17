/*************************************************************  
 * NOTE : main_process.c
 *      Application Main Process
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/

#include "main_process.h"
#include "firmware.h"
#include "usbd_dfu.h"
#include "usb_device.h"

void IAP_Process( void );
void jumpToApplication( uint32_t address );

pFunction	Jump_To_Application;
uint32_t	JumpAddress;

void MainProcess( void )
{	
	uint32_t	ret = 0;
	
	jprintf( "Start Main Process!!! \r\n" );

	// Check Verification
#if ENABLE_VERIFICATION_APP_JUMP	// H/W 구성 후 재코딩...
	if( HAL_GPIO_ReadPin( VERIFY_CHECK_GPIO_Port, VERIFY_CHECK_Pin ) )
	{
		jiprintf( "Jump to Verification Application!!!\r\n" );
		jumpToApplication( FIRMWARE_VERIFICATIONAPP_ADD );
	}
#endif
	
	// Check IAP
#if ENABLE_IAP		// H/W 구성 후 재코딩...
	if( HAL_GPIO_ReadPin( IAP_CHECK_GPIO_Port, IAP_CHECK_Pin ) )
	{
		jiprintf( "Go to IAP Process!!!\r\n" );
		IAP_Process();
	}
#endif

	// Read Firmware Information
	ret = readFirmwareInfo();
	if( ret )
	{
		// occur error
		jeprintf( "Fail read Firmware Information!!!\r\n" );
	}
	
	// Check Firmware Information Initialized
	if( gstruFwInfo.mucInitialized != FIRMWARE_INITIALIZED )
	{		
		initFirmwareInfo();
	}	
		
	// Check firmware updated
	if( gstruFwInfo.mucUpdated )
	{		
		ret = updateMainApp();
		if( ret == 0 )							// update OK
		{	
			gstruFwInfo.mucUpdated	= 0;
			guFwChanged	= 1;
		}
	}

	// Check Main Application CheckSum
	do
	{
		ret = checkCS( &gstruFwInfo.mstruMainAppInfo );
		if( ret )
		{
			// Main Application CheckSum Fail
			if( checkCS( &gstruFwInfo.mstruBackupAppInfo ) )
			{
				// Backup Application CheckSum Fail
				// jump IAP Process
				IAP_Process();
			}
			else
			{
				// backupMainApp();
			}
		}
	} while( ret );

	if( guFwChanged )
	{
		writeFirmwareInfo();
	}	
	
	jumpToApplication( gstruFwInfo.mstruMainAppInfo.mJumpAddress );
	
//	testFirmwareInfo();
}

void IAP_Process( void )
{
	jprintf( "DFU Upgrade Mode Start..\r\n" );

	MX_USB_DEVICE_Init();
	
	while( 1 )
	{
		asm( "NOP" );
	}	
}

void jumpToApplication( uint32_t address )
{
	jprintf( "Jump Application Address..[0x%08x]\r\n", (*(__IO uint32_t*)address));
	if( ((*(__IO uint32_t*)address) & 0x2FFE0000 ) == 0x20000000 )
	{
		// 1. Disable Interrupt
		__disable_irq();

		// 2. Initialize user application's Stack Pointer
		__set_MSP(*(__IO uint32_t*)address);

		// 3. Initialize user application's VTOR Register
		SCB->VTOR=address;

		// 4. Jump to user application
		JumpAddress = *(__IO uint32_t*)(address + 4);
		Jump_To_Application = (pFunction)JumpAddress;
		Jump_To_Application();
	}
	
	IAP_Process();
}
