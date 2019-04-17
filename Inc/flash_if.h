#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

/*---------------------------------------------------------------------------------------------
     Defines
---------------------------------------------------------------------------------------------*/
#define	FLASH_FAIL							1
#define FLASH_OK							0
	
#define	USER_FLASH_LAST_PAGE_ADDRESS		(uint32_t)0x081FFFFF	

#define ADDR_FLASH_SECTOR_0					((uint32_t)0x00000000) /* Base @ of Sector 0, 128 Kbytes */
#define ADDR_FLASH_SECTOR_1					((uint32_t)0x00020000) /* Base @ of Sector 1, 128 Kbytes */
#define ADDR_FLASH_SECTOR_2					((uint32_t)0x00040000) /* Base @ of Sector 2, 128 Kbytes */
#define ADDR_FLASH_SECTOR_3					((uint32_t)0x00060000) /* Base @ of Sector 3, 128 Kbytes */
#define ADDR_FLASH_SECTOR_4					((uint32_t)0x00080000) /* Base @ of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5					((uint32_t)0x000A0000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6					((uint32_t)0x000C0000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7					((uint32_t)0x000E0000) /* Base @ of Sector 7, 128 Kbytes */

/*---------------------------------------------------------------------------------------------
     Functions
---------------------------------------------------------------------------------------------*/
void		initFlash( void );
void		deinitFlash( void );
uint32_t	eraseFlash( uint32_t Addr );
uint32_t	writeFlash( uint32_t flashAddress, uint32_t *data, uint32_t dataLength );
uint32_t	writeByteFlash( uint32_t flashAddress, uint8_t *data, uint32_t dataLength );
uint32_t	readByteFlash( uint32_t flashAddress, uint8_t* data, uint32_t dataLength );
uint32_t	copyFlash( uint32_t srcAddress, uint32_t destAddress, uint32_t dataLength );

static uint32_t	getSector( uint32_t Address );
static uint32_t	getBank( uint32_t Address );

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_IF_H */