/**************************************************************************//**
 * @file     BSP_spi.h
 * @brief    SPI Related Config Header File
 * @version  V1.00
 * @date     18. September 2018
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include <stdint.h>
#include <SC32F58128.h>
#include "SC32F58128_spi.h"
#include "SC32F58128_gpio.h"
/********************************************************************************
 * Constants and macros
 *******************************************************************************/
#define GPIO_SPI0_CLK        GPIO_18
#define GPIO_SPI0_TXD        GPIO_16
#define GPIO_SPI0_RXD        GPIO_17
#define GPIO_SPI0_FFS        GPIO_19

#define SPI0_FFS_NULL        0
#define HARD                 1
#define SOFT                 2
#define GPIO_SPI0_FFS_CTRL   SOFT

#define Dummy_Byte 0xFFFF
#define SPIT_FLAG_TIMEOUT 0xFFFFFFFF

#define SPI_CS_EN            GPIO_SetLow(GPIO_SPI0_FFS)
#define SPI_CS_DIS           GPIO_SetHigh(GPIO_SPI0_FFS)
/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/
/** @defgroup SPI_Comm_status
  * @{
  */
typedef enum eSpiCommState{
  SPI_COMM_TX_TIMEOUT             = 0x10000000,
  SPI_COMM_RX_TIMEOUT             = 0x20000000,
}e_SpiCommState;
/**
  * @}
  */
/********************************************************************************
* Variables
*******************************************************************************/


/********************************************************************************
* prototypes of the functions
*******************************************************************************/
extern void InitSPI0Gpio(void);
extern void BSP_SpiInit(void);
extern SPI_InitTypeDef            SPI_Cfg_Init;         /*!< SPI communication parameters */
extern uint32_t SPI_DEV_SendByte(uint16_t byte);
/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_SPI_H__ */

