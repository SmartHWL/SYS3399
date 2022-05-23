/**************************************************************************//**
 * @file     BSP_uart.h
 * @brief    UART Related Config Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include <stdint.h>
#include <SC32F58128.h>
#include "SC32F58128_Uart.h"

/********************************************************************************
 * Constants and macros
 *******************************************************************************/
#define DEBUG_UART   0

#if (DEBUG_UART==0)
    #define IS_INT_RX()          IS_INT_MASK_RX(0)
    #define CLR_INT_FLAG_RX()    CLR_INT_RX(0)
    #define IS_INT_TX()          IS_INT_MASK_TX(0)
    #define CLR_INT_FLAG_TX()    CLR_INT_TX(0)
    #define IS_INT_OE()          IS_INT_MASK_OE(0)
    #define UART_REST_OE()       UART_REST_STATUS(0)
    #define UART_BUFF         UART0->UARTDR_b.DATA
    #define UART_BUFF_ADD   &(UART0->UARTDR)
    #define TX_INT_EN         UART0->UARTIMSC_b.Transmit_IM = 1
    #define TX_INT_DIS        UART0->UARTIMSC_b.Transmit_IM = 0
    #define EN_UART_TXDMA()   UART_TXDMA_EN(0)
    #define DIS_UART_TXDMA()  UART_TXDMA_DIS(0)
    #define DBG_DMA_TRIG_SRC  0uL  //trig source uart0_Hs_tx;
#elif (DEBUG_UART==1)
    #define IS_INT_RX()          IS_INT_MASK_RX(1)
    #define CLR_INT_FLAG_RX()    CLR_INT_RX(1)
    #define IS_INT_TX()          IS_INT_MASK_TX(1)
    #define CLR_INT_FLAG_TX()    CLR_INT_TX(1)
    #define IS_INT_OE()          IS_INT_MASK_OE(1)
    #define UART_REST_OE()       UART_REST_STATUS(1)
    #define UART_BUFF         UART1->UARTDR_b.DATA
    #define UART_BUFF_ADD   &(UART1->UARTDR)
    #define TX_INT_EN         UART1->UARTIMSC_b.Transmit_IM = 1
    #define TX_INT_DIS        UART1->UARTIMSC_b.Transmit_IM = 0
    #define EN_UART_TXDMA()   UART_TXDMA_EN(1)
    #define DIS_UART_TXDMA()  UART_TXDMA_DIS(1)
    #define DBG_DMA_TRIG_SRC  2uL  //trig source uart1_ls_tx;
#elif (DEBUG_UART==2)
    #define IS_INT_RX()          IS_INT_MASK_RX(2)
    #define CLR_INT_FLAG_RX()    CLR_INT_RX(2)
    #define IS_INT_TX()          IS_INT_MASK_TX(2)
    #define CLR_INT_FLAG_TX()    CLR_INT_TX(2)
    #define IS_INT_OE()          IS_INT_MASK_OE(2)
    #define UART_REST_OE()       UART_REST_STATUS(2)
    #define UART_BUFF         UART2->UARTDR_b.DATA
    #define UART_BUFF_ADD   &(UART2->UARTDR)
    #define TX_INT_EN         UART2->UARTIMSC_b.Transmit_IM = 1
    #define TX_INT_DIS        UART2->UARTIMSC_b.Transmit_IM = 0
    #define EN_UART_TXDMA()   UART_TXDMA_EN(2)
    #define DIS_UART_TXDMA()  UART_TXDMA_DIS(2)
    #define DBG_DMA_TRIG_SRC  4uL  //trig source uart2_ls_tx;
#endif


/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/


/********************************************************************************
* Variables
*******************************************************************************/


/********************************************************************************
* prototypes of the functions
*******************************************************************************/
extern void Uart0Init(uint32_t brdRate);
extern void Uart1Init(uint32_t brdRate);
extern void Uart2Init(uint32_t brdRate);
extern void BSP_UartInit(void);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_UART_H__ */

