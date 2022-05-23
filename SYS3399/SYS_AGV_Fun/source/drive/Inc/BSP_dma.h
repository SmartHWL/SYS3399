/**************************************************************************//**
 * @file     BSP_dma.h
 * @brief    DMA related Configuration Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/

#ifndef __BSP_DMA_H__
#define	__BSP_DMA_H__

/********************************************************************************
 * Include files
 *******************************************************************************/
#include <stdint.h>
#include "SC32F58128_dma.h"

/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/


/********************************************************************************
 * Constants and macros
 *******************************************************************************/

#define  IS_DMA0_CHNL_APINT    (DMA->DONE & (1<<0))
#define  CLR_DMA0_INT_APFLG()   DMA->DONE = (1<<0)
#define  MAX_DATA_CNT          (1024)
#define  R_POWER1              (5)      //2^ORDER 

/*******************************************************************************
 * DMA1 used for motor0 position control
 *******************************************************************************/
#define  DMA_MAX_DATA_CNT_M0             (1024)
#define  IS_DMA_CHNL_APINT_M0            (DMA->DONE & (1<<1))
#define  CLR_DMA_INT_APFLG_M0()           DMA->DONE = (1<<1)
#define  DMA_CTRL_CHNL_ENABLE_M0()        DMA->CTRL1_b.CHNL_ENABLE = 1;
/*******************************************************************************
 * DMA2 used for motor1 position control
 *******************************************************************************/
#define  DMA_MAX_DATA_CNT_M1             (1024)
#define  IS_DMA_CHNL_APINT_M1            (DMA->DONE & (1<<2))
#define  CLR_DMA_INT_APFLG_M1()           DMA->DONE = (1<<2)
#define  DMA_CTRL_CHNL_ENABLE_M1()        DMA->CTRL2_b.CHNL_ENABLE = 1;
/*******************************************************************************
 * DMA3 used for get QEP0 position for M0
 *******************************************************************************/
#define  DMA3_MAX_DATA_CNT_M0             (1024)
#define  IS_DMA3_CHNL_APINT_M0            (DMA->DONE & (1<<3))
#define  CLR_DMA3_INT_APFLG_M0()           DMA->DONE = (1<<3)
#define  DMA3_CTRL_CHNL_ENABLE_M0()        DMA->CTRL3_b.CHNL_ENABLE = 1;
/*******************************************************************************
 * DMA4 used for get QEP1 position for M1
 *******************************************************************************/
#define  DMA4_MAX_DATA_CNT_M1             (1024)
#define  IS_DMA4_CHNL_APINT_M1            (DMA->DONE & (1<<4))
#define  CLR_DMA4_INT_APFLG_M1()           DMA->DONE = (1<<4)
#define  DMA4_CTRL_CHNL_ENABLE_M1()        DMA->CTRL4_b.CHNL_ENABLE = 1;
/********************************************************************************
 * Variables
 *******************************************************************************/

/********************************************************************************
 * prototypes of the functions
 *******************************************************************************/
extern void DMA_Init(void);
extern void DMA_TranferData2UartTx(void *pu32SrcAdd, void *pu32DstAdd, uint16_t u16Cnt);
extern void SendNotSendData2UartTx(void);


#endif
