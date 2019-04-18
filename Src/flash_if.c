/*************************************************************  
 * NOTE : flash_if.c
 *      Flash Interface
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/

#include "main.h"
#include "flash_if.h"

uint32_t eraseFlash( uint32_t address, uint32_t nbSecores )
{
	FLASH_EraseInitTypeDef	EraseInitStruct;
	uint32_t	SectorError;
	
	jprintf( "Start %s : 0x%08x\r\n", __FUNCTION__, address );
	
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase		= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange	= FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks			= getBank( address );
	EraseInitStruct.Sector			= getSector( address );
	EraseInitStruct.NbSectors		= nbSecores;
    if( HAL_FLASHEx_Erase( &EraseInitStruct, &SectorError ) != HAL_OK )  
    {
		return FLASH_FAIL;  
    }     

	return FLASH_OK;	
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  */
uint32_t writeFlash( uint32_t flashAddress, uint32_t *data, uint32_t dataLength )
{
	uint32_t	i = 0;

	for( i = 0; ( i < dataLength ) && ( flashAddress <= ( USER_FLASH_LAST_PAGE_ADDRESS - 32 ) ); i += 8 )
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word */ 
		if( HAL_FLASH_Program( FLASH_TYPEPROGRAM_FLASHWORD, flashAddress, (uint64_t)((uint32_t)(data + i)) ) == HAL_OK )
		{   
			/* Check the written value */
			if( *(uint32_t*)flashAddress != *(uint32_t*)(data + i) )
			{   
				/* Flash content doesn't match SRAM content */
				jeprintf( "check fail!!!\r\n" );
				return FLASH_FAIL;
			}
			   
			/* Increment FLASH destination address */
			flashAddress += 32; 
		}   
		else
		{   
			jeprintf( "fail write Flash!!!\r\n" );
			/* Error occurred while writing data in Flash memory */
			return FLASH_FAIL;
		}   
	}

	return FLASH_OK;
}

uint32_t writeByteFlash( uint32_t flashAddress, uint8_t *data, uint32_t dataLength )
{
	uint32_t	i = 0;

	for( i = 0; ( i < dataLength ) && ( flashAddress <= ( USER_FLASH_LAST_PAGE_ADDRESS - 32 ) ); i += 32 )
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word */ 
		if( HAL_FLASH_Program( FLASH_TYPEPROGRAM_FLASHWORD, flashAddress, (uint64_t)((uint32_t)(data + i)) ) == HAL_OK )
		{   
			/* Check the written value */
			if( *(uint32_t*)flashAddress != *(uint32_t*)(data + i) )
			{   
				/* Flash content doesn't match SRAM content */
				jeprintf( "check fail!!!\r\n" );
				return FLASH_FAIL;
			}
			   
			/* Increment FLASH destination address */
			flashAddress += 32; 
		}   
		else
		{   
			jeprintf( "fail write Flash!!!\r\n" );
			/* Error occurred while writing data in Flash memory */
			return FLASH_FAIL;
		}   
	}

	return FLASH_OK;
}

uint32_t readByteFlash( uint32_t flashAddress, uint8_t* data, uint32_t dataLength )
{
	uint32_t i = 0;

    for( i = 0; ( i < dataLength ) && ( flashAddress <= ( USER_FLASH_LAST_PAGE_ADDRESS - 4 ) ); i++ )
    {   
        data[i] = *(uint8_t*)flashAddress;
        
        /* Increment FLASH destination address */
        flashAddress += 1;
        __DSB();
    }   

    return FLASH_OK;
}

uint32_t copyFlash( uint32_t srcAddress, uint32_t destAddress, uint32_t dataLength )
{
	uint32_t	i, j;
	uint64_t	data[4];
	
	jprintf( "Start %s, source : 0x%08x, target : 0x%08x, size : %d\r\n", __FUNCTION__, srcAddress, destAddress, dataLength );
	
	// write & verify data
	for( i = 0; ( i < dataLength ) && ( destAddress <= ( USER_FLASH_LAST_PAGE_ADDRESS - 8 ) ); i += 8 )
	{
		/* read source address data */
		for( j = 0; j < 4; j++  )
		{
			data[j] = *(uint64_t*)( srcAddress + ( j * 8 ) );
		}
		
		jprintf( "." );

		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word */ 
		if( HAL_FLASH_Program( FLASH_TYPEPROGRAM_FLASHWORD, destAddress, (uint64_t)(uint32_t)data ) == HAL_OK )
		{   
			/* Check the written value */
			if( *(uint64_t*)destAddress != *(uint64_t*)srcAddress )
			{
				jeprintf( "fail verify data\r\n" );
				return FLASH_FAIL;
			}
			
			/* Increment FLASH destination address */
			destAddress	+= 32;
			srcAddress	+= 32;
		}
		else
		{
			jeprintf( "fail write Flash!!!\r\n" );
			/* Error occurred while writing data in Flash memory */
			return FLASH_FAIL;
		}
	} 
	
	jprintf( "\r\n" );
	
	return FLASH_OK;	
}

static uint32_t getSector( uint32_t address )
{
	uint32_t addr	= address & 0x000FFFFF;

	if(addr < ADDR_FLASH_SECTOR_1)													return FLASH_SECTOR_0;  
	else if((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1))			return FLASH_SECTOR_1;  
	else if((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2))			return FLASH_SECTOR_2;  
	else if((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3))			return FLASH_SECTOR_3;  
	else if((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4))			return FLASH_SECTOR_4;  
	else if((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5))			return FLASH_SECTOR_5;  
	else if((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6))			return FLASH_SECTOR_6;  
	else /*((addr < FLASH_END_ADDR) && (addr >= ADDR_FLASH_SECTOR_7))*/				return FLASH_SECTOR_7;  
}

static uint32_t getBank( uint32_t address )
{
	if((address < FLASH_BANK2_BASE) && (address >= FLASH_BANK1_BASE))				return FLASH_BANK_1;
	else /*((Address < FLASH_BANK2_BASE) && (Address >= FLASH_BANK1_BASE))*/		return FLASH_BANK_2;
}
