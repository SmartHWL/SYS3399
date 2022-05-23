/**************************************************************************//**
 * @file     BSP_gpio.h
 * @brief    GPIO Related Config Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include "SC32F58128_can.h"

/********************************************************************************
 * Constants and macros
 *******************************************************************************/
/* 80,000,000 / 8 / Baudrate */
#define CAN_Baudrate_1M			10	
#define CAN_Baudrate_500k		20
#define CAN_Baudrate_333k		30
#define CAN_Baudrate_250k		40
#define CAN_Baudrate_200k		50
#define CAN_Baudrate_125k		80
#define CAN_Baudrate_100k		100
#define CAN_Baudrate_50k		200
#define CAN_Baudrate_40k		250
#define CAN_Baudrate_20k		500
#define CAN_Baudrate_10k		1000
/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/


/********************************************************************************
* Variables
*******************************************************************************/
extern CanTxMsg CAN_TxMessage;
extern CanRxMsg CAN_RxMessage[8];

/********************************************************************************
* prototypes of the functions
*******************************************************************************/
extern void BSP_CanInit(uint16_t	Baudrate);
extern void Can_Test(void);
extern uint8_t BSP_CanReceive(void);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_GPIO_H__ */

