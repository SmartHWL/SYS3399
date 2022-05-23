#ifndef __MOTORSPEEDCALC_H_
#define __MOTORSPEEDCALC_H_
#include "stdint.h"
#include "stdbool.h"
#include "MowerFSM.h"

typedef struct
{
	int32_t   s32SpeedRPM;/*电机的机械角度*/    	
	int32_t   s32Coef;                            
	int32_t	  s32TurnMs;
	int32_t   s32AnglePu;
}MOTORSPEEDCALC_STR;

extern MOTORSPEEDCALC_STR MotorSpeedCalc;
extern void vMotorSpeedCalcInit(MOTORSPEEDCALC_STR *pstr);
extern void vMotorSpeedCalcCtrl(MOTORSPEEDCALC_STR *pstr);
extern void vWalkMotorLoad(uint8_t u8LEnable,uint8_t u8REnable,uint8_t u8LDir,uint8_t u8RDir,uint16_t u16LSpeed,uint16_t u16RSpeed);
extern void vCutMotorLoad(bool bEnable,uint16_t u16Speed);
extern void vStallCheck(WALK_MOTOR_STR  *pstrWalk);
#endif


