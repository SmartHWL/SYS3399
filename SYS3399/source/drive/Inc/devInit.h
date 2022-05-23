/**
  ******************************************************************************
  * @file    devInit.h
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/07/19
  * @brief   Device Init Header File
  * @details None
	* @note    None
  ******************************************************************************
  * @attention
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SILAN MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
  ******************************************************************************
	* Change History
	* -# 2017/07/19 | v1.0.0 | Wangjianlong | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Define to prevent recursive inclusion
 ******************************************************************************/
#ifndef __DEVINIT_H__
#define __DEVINIT_H__

/*******************************************************************************
 * Include section
 ******************************************************************************/
#include "BSP_uart.h"
#include "BSP_adc.h"
#include "BSP_pwm.h"
#include "BSP_gpio.h"
#include "BSP_dma.h"

/*******************************************************************************
 * Typedefs, structs and enums
 ******************************************************************************/


/*******************************************************************************
 * Constants and macros
 ******************************************************************************/
//#define AD_PRD 2
//#define DUAL_CORE
//#define M0 0
//#define M1 1
//#define M0M1 2
//#define MOTOR_EN M0M1
//#define WHICH_MOTOR_IS_EN MOTOR_EN




/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes of the functions
 ******************************************************************************/
extern void BSP_DeviceInit(void);
extern void BSP_DeviceInit_c1(void);
/*---------------------------------- End of the file ---------------------------------*/
#endif /* __DEVINIT_H__ */

