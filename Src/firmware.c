﻿ /*************************************************************  
 * NOTE : firmware.c
 *      firmware interface
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/
#include "firmware.h"
#include "flash_if.h"

SFwInfo		gstruFwInfo;
uint32_t	guFwChanged = 0;

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
	
	gstruFwInfo.mucInitialized		= FIRMWARE_INITIALIZED;
	gstruFwInfo.mucUpdated			= 0x01;
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
	gstruFwInfo.mstruBlInfo.mSize				= 128 * 1024;
	gstruFwInfo.mstruBlInfo.mCheckSum			= 0x0a0a0a0a;
	
	// Main Application Info
	gstruFwInfo.mstruMainAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruMainAppInfo.marrucVersion[3]	= 0x00;
	
	gstruFwInfo.mstruMainAppInfo.mJumpAddress		= FIRMWARE_MAINAPP_ADD;
	gstruFwInfo.mstruMainAppInfo.mSize				= 0x9557;							// 256 * 1024
	gstruFwInfo.mstruMainAppInfo.mCheckSum			= 0x003cd941;
	
	// Update Application Info
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruUpdateAppInfo.marrucVersion[3]	= 0x00;

	gstruFwInfo.mstruUpdateAppInfo.mJumpAddress		= FIRMWARE_UPDATEAPP_ADD;
	gstruFwInfo.mstruUpdateAppInfo.mSize			= 256 * 1024;
	gstruFwInfo.mstruUpdateAppInfo.mCheckSum		= 0x0a0a0a0a;
	
	// Backup Application Info
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruBackupAppInfo.marrucVersion[3]	= 0x00;

	gstruFwInfo.mstruBackupAppInfo.mJumpAddress		= FIRMWARE_BACKUPAPP_ADD;
	gstruFwInfo.mstruBackupAppInfo.mSize			= 256 * 1024;
	gstruFwInfo.mstruBackupAppInfo.mCheckSum		= 0x0a0a0a0a;
		
	// Verification Application Info
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[0]	= 0x01;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[1]	= 0x00;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[2]	= 0x00;
	gstruFwInfo.mstruVeriAppInfo.marrucVersion[3]	= 0x00;
	
	gstruFwInfo.mstruVeriAppInfo.mJumpAddress		= FIRMWARE_VERIFICATIONAPP_ADD;
	gstruFwInfo.mstruVeriAppInfo.mSize				= 256 * 1024;
	gstruFwInfo.mstruVeriAppInfo.mCheckSum			= 0x0a0a0a0a;
	
	ret = writeFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Write Firmware Infomation!!!\r\n" );
		return 1;
	}
	
	return 0;
}

uint32_t readFirmwareInfo( void )
{
	uint32_t flashAddress;
	
	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	flashAddress = FIRMWARE_INFO_ADD;
	
	if( readByteFlash( flashAddress, (uint8_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) ) )
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
	uint32_t flashAddress;
	uint32_t ret = 0;
	
	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	flashAddress = FIRMWARE_INFO_ADD;
	
	initFlash();
	
	eraseFlash( flashAddress );
	
	ret = writeFlash( flashAddress, (uint32_t*)&gstruFwInfo, (uint16_t)sizeof( SFwInfo ) / 4 );
	if( ret )		
	{		
		jeprintf( "Fail write firmware information!!!\r\n" );
	}
	
	deinitFlash();
	
	return ret;
}

/*---------------------------------------------------------------------------------------------
     Firmware Application
---------------------------------------------------------------------------------------------*/
uint32_t checkCS( SAppInfo *appInfo )
{
	uint32_t	flashAddress	= appInfo->mJumpAddress;
	uint32_t	size			= appInfo->mSize;
	
	uint32_t	savedCS			= appInfo->mCheckSum;
	uint32_t	calcCS			= 0;

	uint8_t		readData = 0;
	
	jprintf( "Start %s\r\n", __FUNCTION__ );
	
	while( size )
	{
		readByteFlash( flashAddress, &readData, 1 );		// read
		calcCS += (uint32_t)readData;									// calculate
		
		flashAddress += 1;
		size--;		
	}	
	
	jiprintf( "calcCS : 0x%08x, savedCS : 0x%08x \r\n", calcCS, savedCS );
	
	return ( savedCS == calcCS ? 0 : 1 );
}

uint32_t updateMainApp( void )
{
	uint32_t sourceFlashAddress;
	uint32_t destinationFlashAddress;
	uint32_t size;
	uint32_t ret = 0;
	
	jprintf( "Start %s\r\n", __FUNCTION__ );

	// set source, destination, size
//	sourceFlashAddress		= gstruFwInfo.mstruUpdateAppInfo.mJumpAddress;
//	destinationFlashAddress	= gstruFwInfo.mstruMainAppInfo.mJumpAddress;
//	size					= gstruFwInfo.mstruUpdateAppInfo.mSize;
	
	sourceFlashAddress		= 0x080E0000;
	destinationFlashAddress	= 0x08140000;
	size					= 128*1024;

	initFlash();
	
	ret = copyFlash( sourceFlashAddress, destinationFlashAddress, size );
	if( ret )
	{
		jeprintf( "Fail update main application!!!\r\n" );		
	}
	
	deinitFlash();
	
	return ret;
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
		jeprintf( "Fail Read Firmware Infomation!!!\r\n" );
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
		jeprintf( "Fail Write Firmware Infomation!!!\r\n" );
		// 후처리
	}
		
	// 4. FirmwareInfo Variable set 0
	memset( &gstruFwInfo, 0x00, sizeof( SFwInfo ) );

	// 5. Read FirewareInfo & Print
	ret = readFirmwareInfo();
	if( ret )
	{
		jeprintf( "Fail Read Firmware Infomation!!!\r\n" );
		// 후처리
	}
	
	printFirmwareInfo();	
}
#endif // TEST_FIRMWARE_INFO