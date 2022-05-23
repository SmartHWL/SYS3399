/**************************************************************************//**
 * @file     BSP_dma.c
 * @brief    DMA related Configuration Source File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/

/******************************************************************************
* include file
*******************************************************************************/
#include "SC32F58128.h"
#include "BSP_dma.h"
#include "osc.h"
#include "BSP_uart.h"

/******************************************************************************
* global data for the project
*******************************************************************************/

/******************************************************************************
* loacal data for the current file
*******************************************************************************/
uint16_t mg_u16SendingDatCnt;
uint16_t mg_u16SentDtatCnt;

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
  * @brief      DMA data transfer function.
  * @param[in]  pu32SrcAdd : souce address.
  * @param[in]  pu32SrcAdd : dest address.
  * @param[in]  u16Cnt     : transfer data counter.
  * @Note       u16Cnt should be less then 16bit 
*/ 
void DMA_TranferData2UartTx(void *pu32SrcAdd, void *pu32DstAdd, uint16_t u16Cnt)
{
  if(u16Cnt > MAX_DATA_CNT)
  {
    mg_u16SendingDatCnt = u16Cnt - MAX_DATA_CNT;
    mg_u16SentDtatCnt = MAX_DATA_CNT;
    DmaDataStr[0].pSrcEndPtr = pu32SrcAdd;
    DmaDataStr[0].pDstEndPtr = pu32DstAdd;
    DmaDataStr[0].Ctrl.bit.cycle = 1;           // basic function
    DmaDataStr[0].Ctrl.bit.n_minus1 = MAX_DATA_CNT - 1;
    DmaDataStr[0].Ctrl.bit.R_power = R_POWER1;
    DmaDataStr[0].Ctrl.bit.size = 0;            // BYTE MODE
    DmaDataStr[0].Ctrl.bit.src_inc = 0;         // BYTE INC
    DmaDataStr[0].Ctrl.bit.dst_inc = 3;         // address not changed
  }
  else
  {
    mg_u16SendingDatCnt = 0;
    mg_u16SentDtatCnt = 0;
    DmaDataStr[0].pSrcEndPtr = pu32SrcAdd;
    DmaDataStr[0].pDstEndPtr = pu32DstAdd;
    DmaDataStr[0].Ctrl.bit.cycle = 1;           // basic function
    DmaDataStr[0].Ctrl.bit.n_minus1 = u16Cnt - 1;
    DmaDataStr[0].Ctrl.bit.R_power = R_POWER1;
    DmaDataStr[0].Ctrl.bit.size = 0;            // BYTE MODE
    DmaDataStr[0].Ctrl.bit.src_inc = 0;         // BYTE INC
    DmaDataStr[0].Ctrl.bit.dst_inc = 3;         // address not changed
  }
	DMA->CTRL0_b.CHNL_ENABLE = 1;
	EN_UART_TXDMA();
}


/**
  * @brief  DMA data transfer function.
  * @Note:  This function was called by DMA ISR, when mg_u16SendingDatCnt == 0, disable DMA interrupt.
*/ 
void SendNotSendData2UartTx(void)
{
  void * pSrcAdd;
  void * pDstAdd;

  if(mg_u16SendingDatCnt == 0)
  {
    DIS_UART_TXDMA();
		g_u8TxBuffState = 0;
     return;
  } 

  pSrcAdd = DmaDataStr[0].pSrcEndPtr;
  pDstAdd = DmaDataStr[0].pDstEndPtr;

  if(mg_u16SendingDatCnt > MAX_DATA_CNT)
  {
    DmaDataStr[0].pSrcEndPtr = (int8_t *)pSrcAdd + mg_u16SentDtatCnt;
    DmaDataStr[0].pDstEndPtr = (int8_t *)pDstAdd;
    DmaDataStr[0].Ctrl.bit.cycle = 1;           //basic function
    DmaDataStr[0].Ctrl.bit.n_minus1 = MAX_DATA_CNT-1;
    DmaDataStr[0].Ctrl.bit.R_power = R_POWER1;
    DmaDataStr[0].Ctrl.bit.size = 0;            //BYTE MODE
    DmaDataStr[0].Ctrl.bit.src_inc = 0;         //BYTE INC
    DmaDataStr[0].Ctrl.bit.dst_inc = 3;         //address not changed
    mg_u16SendingDatCnt = mg_u16SendingDatCnt - MAX_DATA_CNT;
    mg_u16SentDtatCnt += MAX_DATA_CNT; 
  }
  else
  {
    DmaDataStr[0].pSrcEndPtr = (int8_t *)pSrcAdd + mg_u16SentDtatCnt;
    DmaDataStr[0].pDstEndPtr = (int8_t *)pDstAdd;
    DmaDataStr[0].Ctrl.bit.cycle = 1;           //basic function
    DmaDataStr[0].Ctrl.bit.n_minus1 = mg_u16SendingDatCnt - 1;
    DmaDataStr[0].Ctrl.bit.R_power = R_POWER1;
    DmaDataStr[0].Ctrl.bit.size = 0;            //BYTE MODE
    DmaDataStr[0].Ctrl.bit.src_inc = 0;         //BYTE INC
    DmaDataStr[0].Ctrl.bit.dst_inc = 3;         //address not changed
    mg_u16SendingDatCnt =  0;
    mg_u16SentDtatCnt = 0; 
  }

	DMA->CTRL0_b.CHNL_ENABLE = 1;
	EN_UART_TXDMA();
}

/**
  * @brief     init all data for the function.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * DMA_Init();
  * @endcode
  * @Note:
*/ 
void DMA_Init(void)
{
	uint8_t  i;

	/**< Dma Data Structure Init */
	for(i = 0; i < MAX_DMA_CHANNEL; i ++)
	{
		DmaDataStr[i].pSrcEndPtr = 0;
		DmaDataStr[i].pDstEndPtr = 0;
		DmaDataStr[i].pAltSrcEndPtr = 0;
		DmaDataStr[i].pAltDstEndPtr = 0;
		DmaDataStr[i].Ctrl.all = 0;
		DmaDataStr[i].AltCtrl.all = 0;
	}

 	/**< DMA Base Pointer Init */
  DMA->BASE_PTR = (uint32_t)DmaDataStr;

	/**< Clear Error Status */
  DMA->BUS_ERR = 0x0;
  DMA->CFG_ERR = 0x0;

	/**< DMA Control Register Init */
	DMA->CTRL0_b.CHNL_REQ1_MASK = 1;
	DMA->CTRL0_b.CHNL_REQ0_SEL = DBG_DMA_TRIG_SRC;
  DMA->DONE = 0x0;
	DMA->CTRL0_b.CHNL_DMADONE_INTREN = 1;

	/**< DMA Enable */
	DMA->CTRL0_b.CHNL_ENABLE = 1;
  DMA->CFG_b.ENABLE = 1;
}



