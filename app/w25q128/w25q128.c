
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"

#include "nrf_drv_spi.h"
#include "w25q128.h"

//SPI��������ʵ��ID,ID�������Ŷ�Ӧ��0:SPI0  1:SPI1 2:SPI2
#define SPI_INSTANCE  0 
//��������Ϊspi��SPI��������ʵ��
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  
//SPI������ɱ�־
static volatile bool spi_xfer_done;  
//SPI���ͻ������飬ʹ��EasyDMAʱһ��Ҫ����Ϊstatic����
static uint8_t    spi_tx_buf[6];  
//SPI���ջ������飬ʹ��EasyDMAʱһ��Ҫ����Ϊstatic����
static uint8_t    spi_rx_buf[6];  


/*****************************************************************************
** ��  ����д��һ���ֽ�
** ��  ����Dat����д�������
** ����ֵ����
******************************************************************************/
void Spi_WriteOneByte(uint8_t Dat)
{   
	  spi_tx_buf[0] = Dat;
	  spi_xfer_done = false;
	  SPIFlash_CS_LOW;
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 1, spi_rx_buf, 0));
    while(!spi_xfer_done);
	  SPIFlash_CS_HIGH;
}
/*****************************************************************************
** ��  ����дʹ��
** ��  ������
** ����ֵ����
******************************************************************************/
static void SpiFlash_Write_Enable(void)
{
  Spi_WriteOneByte(SPIFlash_WriteEnable);
}
/*****************************************************************************
** ��  ������ȡW25Q128оƬID
** ��  ������
** ����ֵ��16λID��W25Q128оƬIDΪ��0xEF17
******************************************************************************/
uint16_t SpiFlash_ReadID(void)
{
  uint16_t dat = 0;
	//׼������
	spi_tx_buf[0] = SPIFlash_ReadID;
	spi_tx_buf[1] = 0x00;
	spi_tx_buf[2] = 0x00;
	spi_tx_buf[3] = 0x00;
	spi_tx_buf[4] = 0xFF;
	spi_tx_buf[5] = 0xFF;
	//������ɱ�־����Ϊfalse
	spi_xfer_done = false;
	//����CS��ʹ��W25Q128FV
	SPIFlash_CS_LOW;
	//�������ݴ���
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 6, spi_rx_buf, 6));
	//�ȴ�SPI�������
  while(!spi_xfer_done);
  //����CS���ͷ�W25Q128FV
	SPIFlash_CS_HIGH;
	//����������������ֽڲ��Ƕ�ȡ��ID
	dat|=spi_rx_buf[4]<<8;  
	dat|=spi_rx_buf[5];	

  return dat;		
}
/*****************************************************************************
** ��  ������ȡW25Q128״̬�Ĵ���
** ��  ������
** ����ֵ��
******************************************************************************/
static uint8_t SpiFlash_ReadSR(void)
{
	  spi_tx_buf[0] = SPIFlash_ReadStatusReg;
	  spi_tx_buf[1] = 0x00;
    //������ɱ�־����Ϊfalse
	  spi_xfer_done = false;
	  SPIFlash_CS_LOW;
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 2, spi_rx_buf, 2));
    while(!spi_xfer_done);
    SPIFlash_CS_HIGH;
    return spi_rx_buf[1];
}
//�ȴ�W25Q128����
void SpiFlash_Wait_Busy(void)   
{   
	while((SpiFlash_ReadSR()&0x01)==0x01);  		// �ȴ�BUSYλ���
} 
/*****************************************************************************
** ��  ��������������W25Q128FVSIG��С�Ĳ�����λ������
** ��  ����[in]SecAddr��������ַ
** ����ֵ����
******************************************************************************/
void SPIFlash_Erase_Sector(uint32_t SecAddr)
{
		//����дʹ������
    SpiFlash_Write_Enable();
		
		//������������
		spi_tx_buf[0] = SPIFlash_SecErase;		
		//24λ��ַ
    spi_tx_buf[1] = (uint8_t)((SecAddr&0x00ff0000)>>16);
    spi_tx_buf[2] = (uint8_t)((SecAddr&0x0000ff00)>>8);
    spi_tx_buf[3] = (uint8_t)SecAddr;
    //������ɱ�־����Ϊfalse
    spi_xfer_done = false;
	  //����CS��ʹ��W25Q128FV
	  SPIFlash_CS_LOW;
	  //�������ݴ��䣺���ͳ���4���ֽڣ���ȡ����0�ֽ�
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, SPIFLASH_CMD_LENGTH, spi_rx_buf, 0));
	  //�ȴ�SPI�������
    while(!spi_xfer_done);
	  //����CS���ͷ�W25Q128FV
    SPIFlash_CS_HIGH;
	  //�ȴ�W25Q128FV��ɲ���
    SpiFlash_Wait_Busy();
}
/*****************************************************************************
** ��  ����ȫƬ����W25Q128FV��ȫƬ���������ʱ�����ֵΪ��40��
** ��  ������
** ����ֵ����
******************************************************************************/
void SPIFlash_Erase_Chip(void)
{
		//����дʹ������
    SpiFlash_Write_Enable();
		//ȫƬ��������
		spi_tx_buf[0] = SPIFlash_ChipErase;
	  //������ɱ�־����Ϊfalse
    spi_xfer_done = false;
	  //����CS��ʹ��W25Q128FV
	  SPIFlash_CS_LOW;
	  //�������ݴ��䣺���ͳ���1���ֽڣ���ȡ����0�ֽ�
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 1, spi_rx_buf, 0));
    while(!spi_xfer_done);
	  //����CS���ͷ�W25Q128FV
	  SPIFlash_CS_HIGH;
    //�ȴ�W25Q128FV��ɲ���
    SpiFlash_Wait_Busy();
}
/*****************************************************************************
** ��  ������ָ���ĵ�ַд������,���д��ĳ��Ȳ��ܳ����õ�ַ����ҳ���ʣ��ռ�
**         *pBuffer:ָ���д������ݻ���
**         WriteAddr:д�����ʼ��ַ
**         WriteBytesNum:д����ֽ�����һ�����256���ֽ�
** ����ֵ��RET_SUCCESS
******************************************************************************/
uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t size)
{
	  //���д������ݳ����Ƿ�Ϸ���д�볤�Ȳ��ܳ���ҳ��Ĵ�С
	  if (size > (SPIFlash_PAGE_SIZE - (WriteAddr%SPIFlash_PAGE_SIZE)))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }
	  if (size == 0) return NRF_ERROR_INVALID_LENGTH;
	
    //����дʹ������
    SpiFlash_Write_Enable();
    //ҳ�������
		spi_tx_buf[0] = SPIFlash_PageProgram;
		//24λ��ַ���ߵ�ַ��ǰ
    spi_tx_buf[1] = (uint8_t)((WriteAddr&0x00ff0000)>>16);
    spi_tx_buf[2] = (uint8_t)((WriteAddr&0x0000ff00)>>8);
    spi_tx_buf[3] = (uint8_t)WriteAddr;
	  spi_tx_buf[4] = *pBuffer;
	  
	
	  //����CS��ʹ��W25Q128FV
	  SPIFlash_CS_LOW;
	  //������ɱ�־����Ϊfalse
		spi_xfer_done = false;
	  //�������ݴ���
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, SPIFLASH_CMD_LENGTH+1, spi_rx_buf, 0));
	  //�ȴ�SPI�������
    while(!spi_xfer_done);

	
	  //������ɱ�־����Ϊfalse
		spi_xfer_done = false;
	  //�������ݴ���
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, pBuffer+1, size-1, spi_rx_buf, 0));
	  //�ȴ�SPI�������
    while(!spi_xfer_done);
	  //����CS���ͷ�W25Q128FV
		SPIFlash_CS_HIGH;
	  //�ȴ�W25Q128FV��ɲ���
		SpiFlash_Wait_Busy();

    return NRF_SUCCESS;
}
/*****************************************************************************
** ��  ������ָ���ĵ�ַд�����ݣ���д����ҳ
**         *pBuffer:ָ���д�������
**         WriteAddr:д�����ʼ��ַ
**         size:д����ֽ���
** ����ֵ��RET_SUCCESS
******************************************************************************/
uint8_t SpiFlash_Write_Buf(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t size)
{
    uint32_t PageByteRemain = 0;
	//������ʼ��ַ����ҳ���ʣ��ռ�
    PageByteRemain = SPIFlash_PAGE_SIZE - WriteAddr%SPIFlash_PAGE_SIZE;
	//�����̵����ݳ��Ȳ�����ҳ���ʣ��ռ䣬������ݳ��ȵ���size
    if(size <= PageByteRemain)
    {
        PageByteRemain = size;
    }
	//�ִα�̣�ֱ�����е����ݱ�����
    while(true)
    {
        //���PageByteRemain���ֽ�
		SpiFlash_Write_Page(pBuffer,WriteAddr,PageByteRemain);
		//��������ɣ��˳�ѭ��
        if(size == PageByteRemain)
        {
            break;
        }
        else
        {
            //������ȡ���ݵĻ����ַ
			pBuffer += PageByteRemain;
			//�����̵�ַ
            WriteAddr += PageByteRemain;
			//���ݳ��ȼ�ȥPageByteRemain
            size -= PageByteRemain;
			//�����´α�̵����ݳ���
            if(size > SPIFlash_PAGE_SIZE)
            {
                PageByteRemain = SPIFlash_PAGE_SIZE;
            }
            else
            {
                PageByteRemain = size;
            }
        }
    }
    return NRF_SUCCESS;
}
/*****************************************************************************
** ��  ������ָ���ĵ�ַ����ָ�����ȵ�����
** ��  ����pBuffer��ָ���Ŷ������ݵ��׵�ַ       
**         ReadAddr�����������ݵ���ʼ��ַ
**         size���������ֽ�����ע��size���ܳ���pBuffer�Ĵ�С��������������
** ����ֵ��
******************************************************************************/
uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t size)
{
		uint8_t read_size;
	  spi_tx_buf[0] = SPIFlash_ReadData;
		
		//24λ��ַ���ߵ�ַ��ǰ
    spi_tx_buf[1] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
    spi_tx_buf[2] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
    spi_tx_buf[3] = (uint8_t)ReadAddr;
	
	  //����CS��ʹ��W25Q128FV
	  SPIFlash_CS_LOW;
	  //������ɱ�־����Ϊfalse
		spi_xfer_done = false;
	  //�������ݴ���
	  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, SPIFLASH_CMD_LENGTH, spi_rx_buf, 0));
	  //�ȴ�SPI�������
    while(!spi_xfer_done);
	  //��ʼ��ȡ����
	  while(size!=0)
		{
			if(size<=SPI_TXRX_MAX_LEN)
			{
				read_size = size;
				size = 0;
			}
			else
			{
				read_size = SPI_TXRX_MAX_LEN;
				size -= SPI_TXRX_MAX_LEN;
			}
			//������ɱ�־����Ϊfalse
		  spi_xfer_done = false;
	    //�������ݴ���
	    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 0, pBuffer, read_size));
	    //�ȴ�SPI�������
      while(!spi_xfer_done);
			pBuffer += read_size;
		}
	  //����CS���ͷ�W25Q128FV
    SPIFlash_CS_HIGH;

    return NRF_SUCCESS;
}
//SPI�¼�������
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
  //����SPI�������  
	spi_xfer_done = true;
}

/*****************************************************************************
** ��  ����������������W25Q128�Ĺܽ�
** ��  �Σ���
** ����ֵ����
******************************************************************************/
void SPI_Flash_Init(void)
{
    //��������SPIƬѡ������Ϊ���
	  nrf_gpio_cfg_output(SPI_SS_PIN);
	  //����CS
	  SPIFlash_CS_HIGH;
	  //ʹ��Ĭ�����ò�����ʼ��SPI���ýṹ��
	  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	  //��дSPI�ź����ӵ���������
    spi_config.ss_pin   = NRF_DRV_SPI_PIN_NOT_USED;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
	  //��ʼ��SPI
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
}
/********************************************END FILE*******************************************/



                                                                                                
