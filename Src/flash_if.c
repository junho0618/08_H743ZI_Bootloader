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

uint32_t eraseFlash( uint32_t Addr )
{
	FLASH_EraseInitTypeDef	EraseInitStruct;
	uint32_t	SectorError;
	
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase		= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange	= FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks			= getBank( Addr );
	EraseInitStruct.Sector			= getSector( Addr );
	EraseInitStruct.NbSectors		= 1;
    if( HAL_FLASHEx_Erase( &EraseInitStruct, &SectorError ) != HAL_OK )  
    {
    	HAL_FLASH_Lock();	  
		return FLASH_FAIL;  
    }     

	return FLASH_OK;	
}

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
				return FLASH_FAIL;
			}
			   
			/* Increment FLASH destination address */
			flashAddress += 32; 
		}   
		else
		{   
			/* Error occurred while writing data in Flash memory */
			return FLASH_FAIL;
		}   
	}

	return FLASH_OK;
}

uint32_t readByteFlash( uint32_t* flashAddress, uint8_t* data, uint16_t dataLength )
{
	uint32_t i = 0;
    
    for( i = 0; ( i < dataLength ) && ( *flashAddress <= ( USER_FLASH_LAST_PAGE_ADDRESS - 4 ) ); i++ )
    {   
        data[i] = *(uint8_t*)*(flashAddress);
        
        /* Increment FLASH destination address */
        *flashAddress += 1;
        __DSB();
    }   

    return FLASH_OK;
}

uint32_t readWordFlash( uint32_t* flashAddress, uint32_t* data, uint16_t dataLength )
{
	return 0;
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