/**************************************************************************//**
 * @file     BSP_uart.c
 * @brief    UART related Configuration Source File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/

/******************************************************************************
* include file
*******************************************************************************/
#include "BSP_uart.h"
#include "SC32F58128_Gpio.h"

/******************************************************************************
* global data for the project
*******************************************************************************/


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
  * @brief     UART BSP Initialization.
  * @param[in] void : void
*/
void BSP_UartInit(void)
{
	/*UART0*/
  GPIO_MuxSel(GPIO_29, UART0_HS_TXD_29);
  GPIO_MuxSel(GPIO_28, UART0_HS_RXD_28);

  GPIO_PullUpDownSel(GPIO_28, PULL_UP);
    
	UartInit(0, 115200, UART_BIT8, PARITY_NONE, STOPBITS_ONE, UART_FIFO_DISABLE, UART_FIFO_DISABLE);
  UART_INT_EN_RX(0);
 #if UART_DEBUG_EN
  UART_INT_EN_TX(0);
 #endif
  UART_INT_EN_OE(0);
  CLR_INT_ALL(0);
  UART_TX_EN(0);
  UART_RX_EN(0);
  UART_EN(0);	
	
}
