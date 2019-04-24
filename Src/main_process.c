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
void goNormalBooting( void );
void goUpdateBooting( void );
void jumpToApplication( uint32_t address );

pFunction	Jump_To_Application;
uint32_t	JumpAddress;

void MainProcess( void )
{	
	uint32_t	ret = 0;
	
	jprintf( "Start Main Process!!! \r\n" );
	
	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET );
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET );
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET );
	

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

	// Firmware Information set 0
	memset( &gstruFwInfo, 0x00, sizeof( SFwInfo ) );

#if( TEST_FIRMWARE_INFO )	
	testFirmwareInfo();	
#endif // TEST_FIRMWARE_INFO	

	// Load Firmware Information
	ret = loadFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail load firmware information!!!\r\n" );
	}
	
	// Check Firmware Information Initialized
	if( gstruFwInfo.mucInitialized != FIRMWARE_INITIALIZED )
	{		
		initFirmwareInfo();
		
		// main => backup
		ret = copyApplication( &gstruFwInfo.mstruMainAppInfo, &gstruFwInfo.mstruBackupAppInfo );
		if( ret )	// Fail
		{
			jeprintf( "Fail Backup Application!!!\r\n" );
			gstruFwInfo.mucBackuped = 0;
		}
		
		gstruFwInfo.mucBackuped = 1;
	}
	
	// print Serial Number
	printSerialNumber();	
	
	// Check Booting Mode
	switch( gstruFwInfo.mucBootMode )
	{
		case BOOT_NORMAL	:
			goNormalBooting();
			break;

		case BOOT_VERIFY	:
			jiprintf( "Jump to Verification Application!!!\r\n" );
			jumpToApplication( FIRMWARE_VERIFICATIONAPP_ADD );
			break;

		case BOOT_UPDATE	:
			goUpdateBooting();
			break;

		case BOOT_IAP		:
		default				:
			IAP_Process();
			break;
	}	
}

void IAP_Process( void )
{
	jprintf( "DFU Upgrade Mode Start..\r\n" );

	MX_USB_DEVICE_Init();
	
	while( 1 )
	{
		HAL_GPIO_TogglePin( LED3_GPIO_Port, LED3_Pin );
		
		HAL_Delay( 100 );
	}
}

void goNormalBooting( void )
{
	uint32_t ret = 0;

	jiprintf( "Start Normal Booting..\r\n" );
	
	ret = checkCS( &gstruFwInfo.mstruMainAppInfo );					// main app check CS
	if( ret ) // Fail
	{
		if( gstruFwInfo.mucBackuped )								// backup app 유무 체크
		{
			ret = checkCS( &gstruFwInfo.mstruBackupAppInfo );			// backup app check CS
			if( ret )  // Fail
			{
				IAP_Process();
			}
			else
			{
				// backup => main
				ret = copyApplication( &gstruFwInfo.mstruBackupAppInfo, &gstruFwInfo.mstruMainAppInfo );
				if( ret )	// Fail
				{
					jeprintf( "Fail Backup Application!!!\r\n" );
					// jump IAP Process
					IAP_Process();
				}

				saveFirmwareInfo();
			}
		}
		else
		{
			IAP_Process();
		}
	}
	
	// Jump Main Application
	jumpToApplication( gstruFwInfo.mstruMainAppInfo.mJumpAddress );
}

void goUpdateBooting( void )
{
	uint32_t ret = 0;
	
	jiprintf( "Start Update Booting..\r\n" );
	
	gstruFwInfo.mucUpdated = 0x00;
	
	// update => main	
	jprintf( "Start Update Application!!!\r\n" );
	ret = copyApplication( &gstruFwInfo.mstruUpdateAppInfo, &gstruFwInfo.mstruMainAppInfo );
	if( ret )
	{	
		jeprintf( "Fail update!!!\r\n" );
		goto errorUpdate;
	}
	
	// main => backup
	jprintf( "Start Backup Application!!!\r\n" );
	ret = copyApplication( &gstruFwInfo.mstruMainAppInfo, &gstruFwInfo.mstruBackupAppInfo );
	if( ret )
	{	
		jeprintf( "Fail backup!!!\r\n" );
		gstruFwInfo.mucBackuped = 0;
		goto errorUpdate;
	}
	
	gstruFwInfo.mucBackuped	= 0x01;			
	gstruFwInfo.mucUpdated	= 0x01;

	saveFirmwareInfo();
	
errorUpdate :		
	// Jump Main Application
	jumpToApplication( gstruFwInfo.mstruMainAppInfo.mJumpAddress );
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
