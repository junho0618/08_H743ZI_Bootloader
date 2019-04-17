/*************************************************************  
 * NOTE : flash_if.c
 *      Flash Interface
 * Author : Lee junho
 * Since : 2019.04.12
**************************************************************/

#include "main.h"
#include "flash_if.h"

void initFlash( void )
{
	HAL_FLASH_Unlock();
	
	__HAL_FLASH_CLEAR_FLAG( FLASH_IT_ALL_BANK1 );
	__HAL_FLASH_CLEAR_FLAG( FLASH_IT_ALL_BANK2 );
}

void deinitFlash( void )
{
	HAL_FLASH_Lock();
}

uint32_t eraseFlash( uint32_t address )
{
	FLASH_EraseInitTypeDef	EraseInitStruct;
	uint32_t	SectorError;
	
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase		= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange	= FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks			= getBank( address );
	EraseInitStruct.Sector			= getSector( address );
	EraseInitStruct.NbSectors		= 1;
    if( HAL_FLASHEx_Erase( &EraseInitStruct, &SectorError ) != HAL_OK )  
    {
    	HAL_FLASH_Lock();	  
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
	uint32_t	flashWord;
	
	// erase destination sector
	eraseFlash( destAddress );
	
	// read data from source & write data to destination 
	while( dataLength > 0 )
	{
		jprintf( "." );
		
		if( readByteFlash( srcAddress, (uint8_t*)&flashWord, 4 ) )
		{
			return FLASH_FAIL;
		}

		if( writeFlash( destAddress, &flashWord, 1 ) )
		{
			return FLASH_FAIL;
		}		
		
		srcAddress	+= 32;
		destAddress	+= 32;

		if( dataLength >= 32 )
		{
			dataLength -= 32;
		}
		else
		{
			if( readByteFlash( srcAddress, (uint8_t*)&flashWord, dataLength ) )
			{
				return FLASH_FAIL;
			}

			if( writeFlash( destAddress, &flashWord, 1 ) )
			{
				return FLASH_FAIL;
			}	
			
			break;
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
