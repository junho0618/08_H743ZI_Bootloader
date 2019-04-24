#ifndef __FIRMWARE_H
#define __FIRMWARE_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/*---------------------------------------------------------------------------------------------
     For debuging
---------------------------------------------------------------------------------------------*/
#define	TEST_FIRMWARE_INFO							0

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define FIRMWARE_INITIALIZED						0xAA
#define APPLICATION_SECTOR_COUNT					3								// Application sector count : 1 sector = 128K
#define	FIRMWARE_RETRY_COUNT						3								// Firmware function retry count
#define	FIRMWARE_RETRY_DELAY						10								// Firmware function retry delay(ms)

#define FIRMWARE_BOOTLOADER_ADD						((uint32_t)0x08000000)
#define FIRMWARE_MAINAPP_ADD						((uint32_t)0x08020000)
#define FIRMWARE_UPDATEAPP_ADD						((uint32_t)0x08100000)
#define FIRMWARE_BACKUPAPP_ADD						((uint32_t)0x08080000)
#define FIRMWARE_VERIFICATIONAPP_ADD				((uint32_t)0x08160000)
#define FIRMWARE_INFO_ADD							((uint32_t)0x080E0000)

#define	VERSION_SIZE								4
#define MODEL_NAME									"New D-Loger"
#define	MODEL_NAME_SIZE								sizeof( MODEL_NAME )
#define	SERIAL_NUMBER								"GIT123"
#define	SERIAL_NUMBER_SIZE							sizeof( SERIAL_NUMBER )

/*---------------------------------------------------------------------------------------------
     Typedefs
---------------------------------------------------------------------------------------------*/
enum BOOTMODE
{
	BOOT_NORMAL	= 0,
	BOOT_VERIFY = 1,
	BOOT_UPDATE	= 2,
	BOOT_IAP	= 3
};

enum FIRMWARE_FLAG
{
	FLAG_NG	= 0,
	FLAG_OK	= 1
};

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
	// flag
	uint8_t		mucInitialized;								// Firmware Info initialied( aa:initialized )
	uint8_t		mucBootMode;								// Booting mode 0: Normal 1: Verify 2: Update 3: IAP
	uint8_t		mucUpdated;									// update 성공 유무
	uint8_t		mucBackuped;								// backup 성공 유무

	// Model Info
	uint8_t		marrucModelName[MODEL_NAME_SIZE];			// Model Name
	uint8_t		marrucSerialNo[SERIAL_NUMBER_SIZE];			// Serial Number
	
	// Application Info
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
void		printSerialNumber( void );
void		printFirmwareInfo( void );
uint32_t	initFirmwareInfo( void );
uint32_t	loadFirmwareInfo( void );
uint32_t	saveFirmwareInfo( void );

uint32_t	checkCS( SAppInfo *appInfo );
uint32_t	copyApplication( SAppInfo *source, SAppInfo *target );

#if ( TEST_FIRMWARE_INFO )
void		testFirmwareInfo( void );
#endif	// TEST_FIRMWARE_INFO

/*---------------------------------------------------------------------------------------------
     Variables
---------------------------------------------------------------------------------------------*/
extern SFwInfo	gstruFwInfo;
extern uint32_t	guFwInfoChanged;

#ifdef __cplusplus
}
#endif

#endif /* __FIRMWARE_H */
