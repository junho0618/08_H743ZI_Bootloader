#ifndef __FIRMWARE_INFO_H
#define __FIRMWARE_INFO_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define FIRMWARE_BOOTLOADER_ADD						((uint32_t)0x08000000)
#define FIRMWARE_MAINAPP_ADD						((uint32_t)0x08020000)
#define FIRMWARE_UPDATEAPP_ADD						((uint32_t)0x08060000)
#define FIRMWARE_BACKUPAPP_ADD						((uint32_t)0x080A0000)
#define FIRMWARE_VERIFICATIONAPP_ADD				((uint32_t)0x08100000)
#define FIRMWARE_INFO_ADD							((uint32_t)0x080E0000)

#define	VERSION_SIZE								4
#define MODEL_NAME									"New D-Loger"
#define	MODEL_NAME_SIZE								sizeof( MODEL_NAME )
#define	SERIAL_NUMBER_SIZE							12

#define	TEST_FIRMWARE_INFO							1

/*---------------------------------------------------------------------------------------------
     Typedefs
---------------------------------------------------------------------------------------------*/
struct _SAppInfo
{
	uint8_t		marrucVersion[VERSION_SIZE];				// Application Version
	uint32_t	mJumpAddress;								// Application Address
	uint32_t	mSize;										// Application Size
	uint32_t	mCheckSum;									// Application CheckSum
};
typedef struct _SAppInfo SAppInfo;

struct _SFwInfo
{
	uint8_t		mucUpdated;									// update 진행 유무
	uint8_t		marrucModelName[MODEL_NAME_SIZE];			// Model Name
	uint8_t		marrucSerialNo[SERIAL_NUMBER_SIZE];			// Serial Number
	SAppInfo	mstruBlInfo;								// Bootloader Info
	SAppInfo	mstruMainAppInfo;							// Main Application Info
	SAppInfo	mstruUpdateAppInfo;							// Update Application Info
	SAppInfo	mstruBackupAppInfo;							// Backup Application Info
	SAppInfo	mstruVeriAppInfo;							// Verification Application Info
};
typedef struct _SFwInfo SFwInfo;

/*---------------------------------------------------------------------------------------------
     Functions
---------------------------------------------------------------------------------------------*/
void		printFirmwareInfo( void );
uint32_t	readFirmwareInfo( void );
uint32_t	writeFirmwareInfo( void );

#if ( TEST_FIRMWARE_INFO )
void testFirmwareInfo( void );
#endif	// TEST_FIRMWARE_INFO

/*---------------------------------------------------------------------------------------------
     Variables
---------------------------------------------------------------------------------------------*/
extern SFwInfo	gstruFwInfo;

#ifdef __cplusplus
}
#endif

#endif /* __FIRMWARE_INFO_H */