/*************************************************************  
 * NOTE : firmware.c
 *      firmware interface
 *		firmware function retry count : FIRMWARE_RETRY_COUNT
 *		firmware function retry delay : FIRMWARE_RETRY_DELAY
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/
#include "firmware.h"
#include "flash_if.h"

SFwInfo		gstruFwInfo;
uint32_t	guFwInfoChanged = 0;

/*---------------------------------------------------------------------------------------------
     Firmware Information
---------------------------------------------------------------------------------------------*/
void printSerialNumber( void )
{
	uint32_t	i;
	
	jiprintf( "Serial Number : 0x" );
	for( i = 0; i < SERIAL_NUMBER_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.marrucSerialNo[i] );
	}
	jiprintf( "\r\n" );
}

void printFirmwareInfo( void )
{
	uint32_t	i;
	
	jiprintf( "==================================================\r\n" );
	jiprintf( "===   Print Firmware Info   ======================\r\n" );
	jiprintf( "==================================================\r\n" );
	jprintf( "mucUpdated : 0x%02x\r\n", gstruFwInfo.mucUpdated );
	jprintf( "mucBootMode : 0x%02x\r\n", gstruFwInfo.mucBootMode );
		
	// Model Name
	jprintf( "Model Name : %s\r\n", gstruFwInfo.marrucModelName );
	
	// Serial Number
	printSerialNumber();
	
	// Bootloader Info
	jprintf( "==================================================\r\n" );
	jiprintf( "Bootloader Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.mstruBlInfo.marrucVersion[i] );
	}
	jprintf( "\r\n" );
	
	jprintf( "Bootloader Jump Address : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mJumpAddress );
	jprintf( "Bootloader Size : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mSize );
	jprintf( "Bootloader CheckSum : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mCheckSum );
	
	// Main Application Info
	jprintf( "==================================================\r\n" );
	jiprintf( "Main Application \r\n" );
	jiprintf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.mstruMainAppInfo.marrucVersion[i] );
	}
	jprintf( "\r\n" );
	
	jprintf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mJumpAddress );
	jprintf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mSize );
	jprintf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mCheckSum );
	
	// Update Application Info
	jprintf( "==================================================\r\n" );
	jiprintf( "Update Application \r\n" );
	jiprintf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.mstruUpdateAppInfo.marrucVersion[i] );
	}
	jprintf( "\r\n" );
	
	jprintf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mJumpAddress );
	jprintf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mSize );
	jprintf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mCheckSum );
	
	// Backup Application Info
	jprintf( "==================================================\r\n" );
	jiprintf( "Backup Application \r\n" );
	jiprintf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.mstruBackupAppInfo.marrucVersion[i] );
	}
	jprintf( "\r\n" );
	
	jprintf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mJumpAddress );
	jprintf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mSize );
	jprintf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mCheckSum );
		
	// Verification Application Info
	jprintf( "==================================================\r\n" );
	jiprintf( "Verification Application \r\n" );
	jiprintf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		jiprintf( "%02x", gstruFwInfo.mstruVeriAppInfo.marrucVersion[i] );
	}
	jprintf( "\r\n" );
	
	jprintf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mJumpAddress );
	jprintf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mSize );
	jprintf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mCheckSum );
	jprintf( "==================================================\r\n" );
}

uint32_t initFirmwareInfo( void )
{
	uint32_t	flashAddress;
	uint32_t	ret = 0;
	
	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	gstruFwInfo.mucBootMode			= BOOT_NORMAL;
	gstruFwInfo.mucInitialized		= FIRMWARE_INITIALIZED;
	gstruFwInfo.mucUpdated			= 0x00;
	memcpy( gstruFwInfo.marrucModelName, MODEL_NAME, MODEL_NAME_SIZE );

	// read serial number	
	flashAddress = SERIAL_NUMBER_ADD;
	if( readByteFlash( flashAddress, (uint8_t*)gstruFwInfo.marrucSerialNo, (uint16_t)SERIAL_NUMBER_SIZE ) )
	{
		jeprintf( "Fail read Serial Number!!!\r\n" );
		return 1;
	}
	else
	{
		printSerialNumber();
	}
	
	// Bootloader Info
	gstruFwInfo.mstruBlInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruBlInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruBlInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruBlInfo.marrucVersion[3]	= 0x00;
	
	gstruFwInfo.mstruBlInfo.mJumpAddress		= FIRMWARE_BOOTLOADER_ADD;
	gstruFwInfo.mstruBlInfo.mSize				= 0x20000;								// Max 0x20000( 128K )
	gstruFwInfo.mstruBlInfo.mCheckSum			= 0x0a0a0a0a;
	
	// Main Application Info
	gstruFwInfo.mstruMainAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[3]	= 0x00;
	
	gstruFwInfo.mstruMainAppInfo.mJumpAddress		= FIRMWARE_MAINAPP_ADD;
	gstruFwInfo.mstruMainAppInfo.mSize				= 0x956c;							// Max 0x40000( 256K )
	gstruFwInfo.mstruMainAppInfo.mCheckSum			= 0x003ce4f2;
	
	// Update Application Info
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[3]	= 0x00;

	gstruFwInfo.mstruUpdateAppInfo.mJumpAddress		= FIRMWARE_UPDATEAPP_ADD;
	gstruFwInfo.mstruUpdateAppInfo.mSize			= 0x40000;							// Max 0x40000( 256K )
	gstruFwInfo.mstruUpdateAppInfo.mCheckSum		= 0x0a0a0a0a;
	
	// Backup Application Info
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[3]	= 0x00;

	gstruFwInfo.mstruBackupAppInfo.mJumpAddress		= FIRMWARE_BACKUPAPP_ADD;
	gstruFwInfo.mstruBackupAppInfo.mSize			= 0x40000;							// Max 0x40000( 256K )
	gstruFwInfo.mstruBackupAppInfo.mCheckSum		= 0x0a0a0a0a;
		
	// Verification Application Info
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[3]	= 0x00;
	
	gstruFwInfo.mstruVeriAppInfo.mJumpAddress		= FIRMWARE_VERIFICATIONAPP_ADD;
	gstruFwInfo.mstruVeriAppInfo.mSize				= 0x40000;							// Max 0x40000( 256K )
	gstruFwInfo.mstruVeriAppInfo.mCheckSum			= 0x0a0a0a0a;
	
	ret = saveFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Save Firmware Infomation!!!\r\n" );
		return 1;
	}
	
	return 0;
}

uint32_t loadFirmwareInfo( void )
{
	uint32_t flashAdd	= FIRMWARE_INFO_ADD;	
	uint32_t count		= FIRMWARE_RETRY_COUNT;
	uint32_t ret		= 0;
	
//	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	while( count-- )
	{
		ret = readByteFlash( flashAdd, (uint8_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) );
		if( ret )
		{
			jeprintf( "fail read firmware info(%d)!!!\r\n", count );
			HAL_Delay( FIRMWARE_RETRY_DELAY );				// 10ms delay
		}
		else
		{
//			printFirmwareInfo();
			return 0;
		}
	}
	
	return 1;
}

uint32_t saveFirmwareInfo( void )
{	
	uint32_t flashAdd	= FIRMWARE_INFO_ADD;
	uint32_t count		= FIRMWARE_RETRY_COUNT;
	uint32_t ret		= 0;
	
//	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	HAL_FLASH_Unlock();

	while( count-- )
	{	
		eraseFlash( flashAdd, 1 );
		
		ret = writeFlash( flashAdd, (uint32_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) / 4 );
		if( ret )		
		{		
			jeprintf( "Fail save firmware information(%d)!!!\r\n", count );
			HAL_Delay( FIRMWARE_RETRY_DELAY );				// 10ms delay
		}
		else
		{
			break;
		}
	}
	
	HAL_FLASH_Lock();
	
	return ret;
}

/*---------------------------------------------------------------------------------------------
     Firmware Application
---------------------------------------------------------------------------------------------*/
uint32_t checkCS( SAppInfo *appInfo )
{
	uint32_t flashAdd	= appInfo->mJumpAddress;
	uint32_t calcCS		= 0;

	uint32_t count		= FIRMWARE_RETRY_COUNT;
	uint32_t i;

//	jprintf( "Start %s, target : 0x%08x\r\n", __FUNCTION__, flashAdd );
	
	while( count-- )
	{
		for( i = 0; ( i < appInfo->mSize ) && ( flashAdd <= ( USER_FLASH_LAST_PAGE_ADDRESS - 1 ) ); i++ )
		{
			calcCS += (uint64_t)( *(uint8_t*)(flashAdd + i) );						// read & calculate
//			flashAdd += 1;
		}
		
		if( appInfo->mCheckSum == calcCS )
		{
			return 0;
		}
		else
		{
			jeprintf( "calcCS : 0x%08x, savedCS : 0x%08x(%d) \r\n", calcCS, appInfo->mCheckSum, count );
			HAL_Delay( FIRMWARE_RETRY_DELAY );
		}					
	}

	return 1;
}

uint32_t copyApplication( SAppInfo *source, SAppInfo *target )
{
	uint32_t count		= FIRMWARE_RETRY_COUNT;
	uint32_t ret		= 0;
	
//	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	HAL_FLASH_Unlock();
	
	while( count-- )
	{
		// erase destination sector
		ret = eraseFlash( target->mJumpAddress, APPLICATION_SECTOR_COUNT );	
		if( ret )
		{
			jeprintf( "fail eraseFlash(%d)!!!\r\n", count );
			HAL_Delay( FIRMWARE_RETRY_DELAY );
			continue;
		}
		
		ret = copyFlash( source->mJumpAddress, target->mJumpAddress, source->mSize );
		if( ret )
		{
			jeprintf( "Fail copy flash(%d)!!!\r\n", count );
			HAL_Delay( FIRMWARE_RETRY_DELAY );
		}
		else
		{	
			memcpy( target->marrucVersion, source->marrucVersion, VERSION_SIZE );		// Application Version
			target->mSize		= source->mSize;
			target->mCheckSum	= source->mCheckSum;
			
			ret = checkCS( target );
			if( ret )
			{
				jeprintf( "Fail CheckSum(%d)!!!\r\n", count );
				HAL_Delay( FIRMWARE_RETRY_DELAY );
			}
			else
			{
				jiprintf( "Success copy application!!!\r\n" );
				HAL_FLASH_Lock();
				
				return 0;
			}
		}
	}
		
	return 1;
}

#if ( TEST_FIRMWARE_INFO )
void testFirmwareInfo( void )
{
	uint32_t	ret = 0;
	
	// 1. Print Variable
	printFirmwareInfo();
	
	// 2. Print Load FirmwareInfo
	ret = loadFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Load Firmware Infomation!!!\r\n" );
		goto test_fail;
	}
	
	printFirmwareInfo();														// Flash에 저장된 Firmware Information
	
	// 3. Write FirmwareInfo	
	gstruFwInfo.mucUpdated			= 0x01;
	memcpy( gstruFwInfo.marrucModelName, MODEL_NAME, MODEL_NAME_SIZE );
	
	gstruFwInfo.marrucSerialNo[0]	= 0x01;
	gstruFwInfo.marrucSerialNo[1]	= 0x02;
	gstruFwInfo.marrucSerialNo[2]	= 0x03;
	gstruFwInfo.marrucSerialNo[3]	= 0x04;
	gstruFwInfo.marrucSerialNo[4]	= 0x05;
	gstruFwInfo.marrucSerialNo[5]	= 0x06;
	gstruFwInfo.marrucSerialNo[6]	= 0x07;
	gstruFwInfo.marrucSerialNo[7]	= 0x08;
	gstruFwInfo.marrucSerialNo[8]	= 0x09;
	gstruFwInfo.marrucSerialNo[9]	= 0x0a;
	gstruFwInfo.marrucSerialNo[10]	= 0x0b;
	gstruFwInfo.marrucSerialNo[11]	= 0x0c;
	
	// Bootloader Info
	gstruFwInfo.mstruBlInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruBlInfo.marrucVersion[1]	= 0x02;
	gstruFwInfo.mstruBlInfo.marrucVersion[2]	= 0x03;
	gstruFwInfo.mstruBlInfo.marrucVersion[3]	= 0x04;
	
	gstruFwInfo.mstruBlInfo.mJumpAddress		= FIRMWARE_BOOTLOADER_ADD;
	gstruFwInfo.mstruBlInfo.mSize				= 0x12345678;
	gstruFwInfo.mstruBlInfo.mCheckSum			= 0x0a0a0a0a;
	
	// Main Application Info
	gstruFwInfo.mstruMainAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[1]	= 0x02;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[2]	= 0x03;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[3]	= 0x04;
	
	gstruFwInfo.mstruMainAppInfo.mJumpAddress		= FIRMWARE_MAINAPP_ADD;
	gstruFwInfo.mstruMainAppInfo.mSize				= 0x12345678;
	gstruFwInfo.mstruMainAppInfo.mCheckSum			= 0x0a0a0a0a;
	
	// Update Application Info
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[1]	= 0x02;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[2]	= 0x03;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[3]	= 0x04;

	gstruFwInfo.mstruUpdateAppInfo.mJumpAddress		= FIRMWARE_UPDATEAPP_ADD;
	gstruFwInfo.mstruUpdateAppInfo.mSize			= 0x12345678;
	gstruFwInfo.mstruUpdateAppInfo.mCheckSum		= 0x0a0a0a0a;
	
	// Backup Application Info
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[1]	= 0x02;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[2]	= 0x03;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[3]	= 0x04;

	gstruFwInfo.mstruBackupAppInfo.mJumpAddress		= FIRMWARE_BACKUPAPP_ADD;
	gstruFwInfo.mstruBackupAppInfo.mSize			= 0x12345678;
	gstruFwInfo.mstruBackupAppInfo.mCheckSum		= 0x0a0a0a0a;
		
	// Verification Application Info
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[1]	= 0x02;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[2]	= 0x03;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[3]	= 0x04;
	
	gstruFwInfo.mstruVeriAppInfo.mJumpAddress		= FIRMWARE_VERIFICATIONAPP_ADD;
	gstruFwInfo.mstruVeriAppInfo.mSize				= 0x12345678;
	gstruFwInfo.mstruVeriAppInfo.mCheckSum			= 0x0a0a0a0a;
	
	ret = saveFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Save Firmware Infomation!!!\r\n" );
		goto test_fail;
	}
		
	// 4. FirmwareInfo Variable set 0
	memset( &gstruFwInfo, 0x00, sizeof( SFwInfo ) );

	// 5. Load FirewareInfo & Print
	ret = loadFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Load Firmware Infomation!!!\r\n" );
		goto test_fail;
	}
	
	printFirmwareInfo();	
	
test_fail:
	while( 1 )
	{
		asm( "NOP" );
	}
}
#endif // TEST_FIRMWARE_INFO
