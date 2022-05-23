/**
  ******************************************************************************
  * @file    BSP_gpio.h
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/04/26
  * @brief   GPIO Related Config Header File
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
	* -# 2017/04/26 | v1.0.0 | Wangjianlong | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Define to prevent recursive inclusion
 ******************************************************************************/
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "SC32F58128_Gpio.h"
//#include "UserConfig.h"
//#include "HardwareInclude.h"
/*******************************************************************************
 * Typedefs, structs and enums
 ******************************************************************************/

/********************************************************************************
 * Constants and macros
 *******************************************************************************/

#define DIGITAL		(0<<4)
#define ANALOGY		(1<<4)


enum ePort      //端口号枚举变量
{
	PortA = 1 ,
	PortB = 2 ,
	PortC = 3 ,
};

/**< GPIO80/ADCINA0       */
/**< GPIO81/ADCINA1       */
/**< GPIO82/ADCINA2/AIO2  */
/**< GPIO83/ADCINA3       */
/**< GPIO84/ADCINA4/AIO4  */
/**< GPIO85/ADCINA5       */
/**< GPIO86/ADCINA6/AIO6  */
/**< GPIO87/ADCINA7       */
/**< GPIO88ADCINB0        */
/**< GPIO89ADCINB1        */
/**< GPIO90/ADCINB2/AIO10 */
/**< GPIO91/ADCINB3       */
/**< GPIO92/ADCINB4/AIO12 */
/**< GPIO93/ADCINB5       */
/**< GPIO94/ADCINB6/AIO14 */
/**< GPIO95/ADCINB7       */




#ifdef GPIO_REVERSE_PIN
#define GET_REVERSE_STATUS()                       GPIO_GetGpioStatus(GPIO_REVERSE_PIN)
#endif

#ifdef GPIO_FORWARD_PIN
#define GET_FORWARD_STATUS()                       GPIO_GetGpioStatus(GPIO_FORWARD_PIN)
#endif

//#define SET_CTRL_CHARGE_RLY_HIGH()              GPIO_SetHigh(GPIO_CTRL_CHARGE_RLY_PIN)
//#define SET_CTRL_CHARGE_RLY_LOW()               GPIO_SetLow(GPIO_CTRL_CHARGE_RLY_PIN)
//#define SET_CTRL_CHARGE_RLY_TOGGLE()            GPIO_Toggle(GPIO_CTRL_CHARGE_RLY_PIN)



#ifdef GPIO_RED_LED_PIN
#define SET_RED_LED_HIGH()              GPIO_SetHigh(GPIO_RED_LED_PIN)
#define SET_RED_LED_LOW()               GPIO_SetLow(GPIO_RED_LED_PIN)
#endif

#if DEBUG_MOTOR_TEST && defined (GPIO_BRK_RLY_PIN)
#define SET_BRK_RLY_HIGH()                       GPIO_SetLow(GPIO_BRK_RLY_PIN)
#elif defined GPIO_BRK_RLY_PIN
#define SET_BRK_RLY_HIGH()                      GPIO_SetHigh(GPIO_BRK_RLY_PIN)
#else
#define SET_BRK_RLY_HIGH()
#endif


#ifdef GPIO_BRK_RLY_PIN
#define SET_BRK_RLY_LOW()                       GPIO_SetLow(GPIO_BRK_RLY_PIN)
#else
#define SET_BRK_RLY_LOW()
#endif

#ifdef GPIO_INTER_LOCK_PIN
#define GET_INTER_LOCK_STATUS()                 GPIO_GetGpioStatus(GPIO_INTER_LOCK_PIN)
#endif

#ifdef GPIO_HALL_U_PIN
#define GET_HALL_U_STATUS()                       GPIO_GetGpioStatus(GPIO_HALL_U_PIN)
#endif

#ifdef GPIO_HALL_V_PIN
#define GET_HALL_V_STATUS()                       GPIO_GetGpioStatus(GPIO_HALL_V_PIN)
#endif

#ifdef GPIO_HALL_W_PIN
#define GET_HALL_W_STATUS()                       GPIO_GetGpioStatus(GPIO_HALL_W_PIN)
#endif
//#define SET_TESTPIN_HIGH              GPIO_SetHigh(GPIO_TESTPIN)
//#define SET_TESTPIN_LOW               GPIO_SetLow(GPIO_TESTPIN)
//#define SET_TESTPIN_TOGGLE            GPIO_Toggle(GPIO_TESTPIN)
/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/

/* gpio int type enum */
typedef enum
{
	GPIO_INT_TYPE_LEVEL = 0,
	GPIO_INT_TYPE_EDGE,
}eGpioIntTYPE;

/* gpio int bv enum */
typedef enum
{
	GPIO_INT_BV_SINGLE = 0,
	GPIO_INT_BV_DOUBLE,
}eGpioIntBV;

/* gpio int pol enum */
typedef enum
{
	GPIO_INT_POL_FALL = 0,
	GPIO_INT_POL_RISE,
}eGpioIntPOL;
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes of the functions
 ******************************************************************************/
extern void BSP_GpioInit(void);
/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_GPIO_H__ */
