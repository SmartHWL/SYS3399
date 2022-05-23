/**
* @file          : devInit.h
* @author        : mcu Lab 
* @date          : mm/dd/yyy
* @brief         : This file is for init function define
* @version       : Ver. 1.00

* H/W Platform   : SL_FOC FOR MOTOR CONCTROL

*------------------------------------------------------------------------------

* Compiler info : keil v5

*------------------------------------------------------------------------------
 
Note: In this software, the function is used in motor control.

*-------------------------------------------------------------------------------

*  History:  

*              mm/dd/yyyy ver. x.y.z author

*              mm1/dd1/yyyy1 ver. x.y.z author1

*------------------------------------------------------------------------------
* @attention
 * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, SLMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.

* <h2><center>&copy; COPYRIGHT 2015 SLMicroelectronics</center></h2>
******************************************************************************
*/

/******************************************************************************/
#ifndef __DEVINIT_H__
#define __DEVINIT_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include "SC32F58128_Gpio.h"

/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/
 
/********************************************************************************
 * Constants and macros
 *******************************************************************************/
#define  DEF_OHE_SHUNT_SMP  1


/********************************************************************************
 * Variables
 *******************************************************************************/

 /********************************************************************************
 * prototypes of the functions
 *******************************************************************************/
extern void INIT_DeviceInit(void);
extern void INIT_SetIRQ_c1(void);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __DEVINIT_H__ */

