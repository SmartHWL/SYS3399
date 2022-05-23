/**************************************************************************//**
 * @file     BSP_pwm.h
 * @brief    PWM related Configuration Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include "SC32F58128_pwm.h"
//#include "SystemPU.h"


#define PWM_PERIOD (80000000/2/20000)    //20kHZ
#define DEAD_TIME_CLK  (1 * 80000 / 1000)


extern void BSP_PwmInit(void);
extern void motor_stop(void);
extern void motor_start(void);
extern void TZ_clear(void);
extern void PWM_PROTECT_CFG(void);
extern void PWM_Duty_CFG(uint16_t a,uint16_t b,uint16_t c);
extern void PWM_ResolveIncentiveInit(uint32_t u32Period);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_PWM_H__ */


