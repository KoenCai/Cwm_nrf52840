
#ifndef __W25Q128_H
#define __W25Q128_H
#include <stdint.h>


/*****************************************************************************
**
*****************************************************************************/
//SPI���Ŷ���
#define  SPI_SS_PIN     NRF_GPIO_PIN_MAP(0,23)
#define  SPI_SCK_PIN    NRF_GPIO_PIN_MAP(0,22)
#define  SPI_MISO_PIN   NRF_GPIO_PIN_MAP(0,21)
#define  SPI_MOSI_PIN   NRF_GPIO_PIN_MAP(0,20)


#define    SPIFlash_CS_LOW    nrf_gpio_pin_clear(SPI_SS_PIN)   //Ƭѡ����͵�ƽ��ʹ��оƬ
#define    SPIFlash_CS_HIGH   nrf_gpio_pin_set(SPI_SS_PIN)     //Ƭѡ����ߵ�ƽ��ȡ��Ƭѡ


//W25Q128 ID
#define  W25Q128_ID     0XEF17




//SPI Flash�����
#define    SPIFlash_WriteEnable       0x06  //дʹ������
#define    SPIFlash_WriteDisable      0x04  //д��ֹ����
#define    SPIFlash_PageProgram       0x02  //ҳд������
#define    SPIFlash_ReadStatusReg		  0x05  //��״̬�Ĵ���1
#define    SPIFlash_WriteStatusReg		0x01  //д״̬�Ĵ���1
#define    SPIFlash_ReadData          0x03  //����������
#define    SPIFlash_SecErase          0x20  //��������
#define    SPIFlash_BlockErase        0xD8  //�����
#define    SPIFlash_ChipErase         0xC7  //ȫƬ����
#define    SPIFlash_ReadID            0x90  //��ȡID


#define    SPIFLASH_CMD_LENGTH        0x04
#define    SPIFLASH_WRITE_BUSYBIT     0x01



#define    SPIFlash_PAGE_SIZE        256
#define    SPIFlash_SECTOR_SIZE      (1024*4)
#define    SPI_TXRX_MAX_LEN          255

#define    FLASH_BLOCK_NUMBLE         7
#define    FLASH_PAGE_NUMBLE          8


void SPI_Flash_Init(void);
uint16_t SpiFlash_ReadID(void);


uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t size);
uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t size);
void SPIFlash_Erase_Sector(uint32_t SecAddr);
void SPIFlash_Erase_Chip(void);
uint8_t SpiFlash_Write_Buf(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum);


#endif
