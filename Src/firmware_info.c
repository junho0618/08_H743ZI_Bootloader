 /*************************************************************  
 * NOTE : firmware_info.c
 *      firmware infomation control
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/
#include "firmware_info.h"
#include "flash_if.h"

SFwInfo	gstruFwInfo;
uint32_t gFlashAddress;

void printFirmwareInfo( void )
{
	uint32_t	i;
	
	printf( "\n\n\r==================================================\r\n" );
	printf( "===   Print Firmware Info   ======================\r\n" );
	printf( "==================================================\r\n" );
	printf( "mucUpdated : 0x%02x\r\n", gstruFwInfo.mucUpdated );
		
	// Model Name
	printf( "Model Name : %s\r\n", gstruFwInfo.marrucModelName );
	
	// Serial Number
	printf( "Serial Number : 0x" );
	for( i = 0; i < SERIAL_NUMBER_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.marrucSerialNo[i] );
	}
	printf( "\r\n" );
	
	// Bootloader Info
	printf( "==================================================\r\n" );
	printf( "Bootloader Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.mstruBlInfo.marrucVersion[i] );
	}
	printf( "\r\n" );
	
	printf( "Bootloader Jump Address : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mJumpAddress );
	printf( "Bootloader Size : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mSize );
	printf( "Bootloader CheckSum : 0x%08x\r\n", gstruFwInfo.mstruBlInfo.mCheckSum );
	
	// Main Application Info
	printf( "==================================================\r\n" );
	printf( "Main Application \r\n" );
	printf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.mstruMainAppInfo.marrucVersion[i] );
	}
	printf( "\r\n" );
	
	printf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mJumpAddress );
	printf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mSize );
	printf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruMainAppInfo.mCheckSum );
	
	// Update Application Info
	printf( "==================================================\r\n" );
	printf( "Update Application \r\n" );
	printf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.mstruUpdateAppInfo.marrucVersion[i] );
	}
	printf( "\r\n" );
	
	printf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mJumpAddress );
	printf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mSize );
	printf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruUpdateAppInfo.mCheckSum );
	
	// Backup Application Info
	printf( "==================================================\r\n" );
	printf( "Backup Application \r\n" );
	printf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.mstruBackupAppInfo.marrucVersion[i] );
	}
	printf( "\r\n" );
	
	printf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mJumpAddress );
	printf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mSize );
	printf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruBackupAppInfo.mCheckSum );
		
	// Verification Application Info
	printf( "==================================================\r\n" );
	printf( "Verification Application \r\n" );
	printf( "Application Version : 0x" );
	for( i = 0; i < VERSION_SIZE; i++ )
	{
		printf( "%02x", gstruFwInfo.mstruVeriAppInfo.marrucVersion[i] );
	}
	printf( "\r\n" );
	
	printf( "Application Jump Address : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mJumpAddress );
	printf( "Application Size : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mSize );
	printf( "Application CheckSum : 0x%08x\r\n", gstruFwInfo.mstruVeriAppInfo.mCheckSum );
	printf( "==================================================\r\n" );
}

uint32_t readFirmwareInfo( void )
{
	gFlashAddress = FIRMWARE_INFO_ADD;
	
	if( readByteFlash( &gFlashAddress, (uint8_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) ) )
	{
		// occer error
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t writeFirmwareInfo( void )
{	
	gFlashAddress = FIRMWARE_INFO_ADD;
	
	initFlash();
	
	eraseFlash( gFlashAddress );
	
	if( writeFlash( gFlashAddress, (uint32_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) / 4 ) )
	{		
		// occer error
		deinitFlash();
		return 1;
	}
	else
	{
		deinitFlash();
		return 0;
	}
}


#if ( TEST_FIRMWARE_INFO )
void testFirmwareInfo( void )
{
	uint32_t	ret = 0;
	
	// 1. Print Variable
	printFirmwareInfo();
	
	// 2. Print Read FirmwareInfo
	ret = readFirmwareInfo();
	if( ret )
	{
		printf( "Fail Read Firmware Infomation!!!\r\n" );
		// 후처리
	}
	
	printFirmwareInfo();
	
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
	
	ret = writeFirmwareInfo();
	if( ret )
	{
		printf( "Fail Write Firmware Infomation!!!\r\n" );
		// 후처리
	}
		
	// 4. FirmwareInfo Variable set 0
	memset( &gstruFwInfo, 0x00, sizeof( SFwInfo ) );

	// 5. Read FirewareInfo & Print
	ret = readFirmwareInfo();
	if( ret )
	{
		printf( "Fail Read Firmware Infomation!!!\r\n" );
		// 후처리
	}
	
	printFirmwareInfo();	
}
#endif // TEST_FIRMWARE_INFO