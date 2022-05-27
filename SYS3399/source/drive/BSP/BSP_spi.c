/**************************************************************************//**
 * @file     BSP_spi.c
 * @brief    UART related Configuration Source File
 * @version  V1.00
 * @date     18. September 2018
 *
 * @note
 *
 ******************************************************************************/

/******************************************************************************
* include file
*******************************************************************************/
#include <SC32F58128.h>
#include <SC32F58128_gpio.h>
#include "BSP_spi.h"
#include <SC32F58128_spi.h>
#include <string.h>

/******************************************************************************
* global data for the project
*******************************************************************************/
SPI_InitTypeDef            SPI_Cfg_Init;         /*!< SPI communication parameters */
uint32_t                   FlashID;
/******************************************************************************
* loacal data for the current file
*******************************************************************************/

/******************************************************************************/
/* local prototypes function **************************************************/
/******************************************************************************/

/******************************************************************************
* loacal function for the current file
*******************************************************************************/

/******************************************************************************
* global function for the current project
*******************************************************************************/
/**
  * @brief      Gpio Initialization.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * InitSPI0Gpio();
  * @endcode
  * @Note:
*/
void BSP_SpiInit(void){
  /* SPI0 parameter configuration*/
  SPI_Cfg_Init.SPI_BaudRate = 4000000;
  SPI_Cfg_Init.SPI_CPHA     = SPI_CPHA_2EDGE; // SPI_CPHA_1EDGE 第一个边沿采样；SPI_CPHA_2EDGE 第二个边沿采样
  SPI_Cfg_Init.SPI_CPOL     = SPI_CPOL_HIGH; // 初始状态为高
  SPI_Cfg_Init.SPI_DataSize = SPI_DATASIZE_8b;//SPI_DATASIZE_16b;
  SPI_Cfg_Init.SPI_FirstBit = SPI_FIRST_BIT_MSB; // 高位有效
  SPI_Cfg_Init.SPI_Mode     = SPI_MODE_MASTER;
  
  SPI_Init(SPI0, &SPI_Cfg_Init);
  
  SPI_LoopBackCmd(SPI0, ENABLE);

  InitSPI0Gpio();

}


/**
  * @brief      Gpio Initialization.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * InitSPI0Gpio();
  * @endcode
  * @Note:
*/
void InitSPI0Gpio(void){
	
  /* SPI0 Gpio Config */
  GPIO_OutputEnable(GPIO_SPI0_FFS);
	
  GPIO_MuxSel(GPIO_SPI0_TXD, SSP0_TXD_16);
  GPIO_MuxSel(GPIO_SPI0_RXD, SSP0_RXD_17);
  GPIO_MuxSel(GPIO_SPI0_CLK, SSP0_CLK_18);
	
  GPIO_OpenDrainEnable(GPIO_SPI0_RXD);
  GPIO_PullUpDownSel(GPIO_SPI0_RXD, PULL_UP);
  GPIO_OutputHighCapabilityEnable(GPIO_SPI0_TXD);
	
  SPI_Cmd(SPI0, ENABLE);
}

/**
  * @brief      SPI transmits one byte.
  * @param[in]  byte : data to be transmitted
  * @retval     uint32_t : The value of the received data of low 16 bits, hight 16 bits is status
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * SPI_DEV_SendByte();
  * @endcode
  * @Note:
*/
uint32_t SPI_DEV_SendByte(uint16_t byte)
{
   
  uint32_t SPITimeout = 0;
  SPITimeout = SPIT_FLAG_TIMEOUT;
 
  /* Wait for the module to be ready to transmit or FIFO is empty*/
  while (SPI_GetFlagStatus(SPI0, SPI_FLAG_BUSY) != RESET)
  {
    if ((SPITimeout--) == 0) return SPI_COMM_TX_TIMEOUT;
  }
 
  /* write data to the buffer register */
  SPI_SendData(SPI0, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* wait for received data ,RNE */
  while (SPI_GetFlagStatus(SPI0, SPI_FLAG_RNE) == RESET)
  {
    if ((SPITimeout--) == 0) return SPI_COMM_RX_TIMEOUT;
  }
 
  /* read the data from the buffer register */
  return SPI_ReceiveData(SPI0);

}

