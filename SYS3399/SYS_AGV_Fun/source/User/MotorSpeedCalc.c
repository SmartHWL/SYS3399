/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName:   Boundary.c
 Author:     Version :   Date: 
 xyb         1.0         2022年1月11日
 Description: 采样信息获取
 Version: 版本信息
 History: 
 <author>    <time>          <version >   <desc> 
 xyb        2022年1月11日       1.0        初版
***********************************************************/
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "BSP_gpio.h"
#include "BSP_uart.h"
#include "IQmathLib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "MotorSpeedCalc.h"
#include "UartToMotor.h"
#include "Control&Filter.h"
#include "MowerFSM.h"
#include "UserConfig.h"


MOTORSPEEDCALC_STR MotorSpeedCalc;

void MotorSpeedCalcInit(MOTORSPEEDCALC_STR *pstr);
void MotorSpeedCalcCtrl(MOTORSPEEDCALC_STR *pstr);
void vWalkMotorLoad(uint8_t u8LEnable,uint8_t u8REnable,uint8_t u8LDir,uint8_t u8RDir,uint16_t u16LSpeed,uint16_t u16RSpeed);
void vCutMotorLoad(bool bEnable,uint16_t u16Speed);
void vStallCheck(WALK_MOTOR_STR  *pstrWalk);



	/*        
	
		              RPM                Rwheel                
       Ωpu = ------------ * ----------------------*t
                  60          RMower*Reduction ratio  
                 
			                        Rwheel                
                 (f) * ----------------------*t															 
                       RMower*Reduction ratio 
 RADIUS_WHEEL    (0.105)  
 RADIUS_MOWER    (0.14)
 RATIO_OF_REDUCTION_  100

*/

void vMotorSpeedCalcInit(MOTORSPEEDCALC_STR *pstr)
{
	pstr->s32Coef=(int32_t)(60*1000*RATIO_OF_REDUCTION*RADIUS_MOWER/RADIUS_WHEEL)>>12; /*s->ms*/
//	pstr->s32AnglePu=_IQ(0.25);
//	pstr->s32SpeedRPM=1500;
	
}


void vMotorSpeedCalcCtrl(MOTORSPEEDCALC_STR *pstr)
{

	pstr->s32TurnMs=((int64_t)pstr->s32Coef*pstr->s32AnglePu/pstr->s32SpeedRPM)>>12;
		
}

/************************************************* 
 Function:    vWalkMotorLoad
 Description: 行走电机控制
 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/
void vWalkMotorLoad(uint8_t u8LEnable,uint8_t u8REnable,uint8_t u8LDir,uint8_t u8RDir,uint16_t u16LSpeed,uint16_t u16RSpeed)/*行走电机*/
{
	
	  static uint16_t lu16WalkTimeCount,lu16SpeedToDriver;	    

	  if((u8LEnable==FALSE)||(u8REnable==FALSE))/**电机不行走**/
		{
			MowerFSM.WalkMotor.Out.u16LSpeed=0;
			MowerFSM.WalkMotor.Out.u16RSpeed=0;
			lu16WalkTimeCount=0;		
		}
		else	if(u8LDir==FORWARD&&u8RDir==FORWARD)/**电机直走**/
    {

			if(lu16WalkTimeCount<TIMES(1.0))
			{			
				lu16WalkTimeCount++;		
				lu16SpeedToDriver=TURNSPEED;
			}
			else
			{
				if(lu16SpeedToDriver<u16LSpeed)
				{						
					lu16SpeedToDriver+=1;
				}
								
				if(lu16SpeedToDriver>WALKSPEED)   lu16SpeedToDriver=WALKSPEED;
				if(lu16SpeedToDriver<TURNSPEED)   lu16SpeedToDriver=TURNSPEED;			
			}
			
			MowerFSM.WalkMotor.Out.u8LDir=u8LDir;
			MowerFSM.WalkMotor.Out.u8RDir=u8RDir;
			MowerFSM.WalkMotor.Out.u16LSpeed=lu16SpeedToDriver;
			MowerFSM.WalkMotor.Out.u16RSpeed=lu16SpeedToDriver;
			
		}
		else/**割草机再转向**/
		{
		
			MowerFSM.WalkMotor.Out.u8LDir=u8LDir;
			MowerFSM.WalkMotor.Out.u8RDir=u8RDir;	
			
			MowerFSM.WalkMotor.Out.u16LSpeed=u16LSpeed;		
			MowerFSM.WalkMotor.Out.u16RSpeed=u16RSpeed;			
			
			lu16WalkTimeCount=0;
		}	
}

/************************************************* 
 Function:    vCutMotorLoad
 Description: 割草电机控制
 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/
void vCutMotorLoad(bool bEnable,uint16_t u16Speed)/*割草电机*/
{

	if(bEnable==FALSE)
	{	
		MowerFSM.CutMotor.Out.u16Speed=0;
		
	}
	else 
	{
				
		MowerFSM.CutMotor.Out.u16Speed=u16Speed;		
		
		MowerFSM.CutMotor.Para.u32WorkTimeCnt++;
		if(MowerFSM.CutMotor.Para.u32WorkTimeCnt>TIMEHOUR(2))
		{
			MowerFSM.CutMotor.Para.u32WorkTimeCnt=0;
			
			if(MowerFSM.CutMotor.Out.bDir==TRUE)
			{
				MowerFSM.CutMotor.Out.bDir=FALSE;			
			}
			else
			{
				MowerFSM.CutMotor.Out.bDir=TRUE;
			}
			
		}	
 }	
	
}

/************************************************* 
 Function:    vWalkMotorLoad
 Description: 堵转判断
 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/
void vStallCheck(WALK_MOTOR_STR  *pstrWalk)
{
	if((*pstrWalk->In.pu16Q14LeftCurrent>_IQ14(0.11))||(*pstrWalk->In.pu16Q14RightCurrent>_IQ14(0.11)))
	{
	   pstrWalk->Para.u16StallCnt++;
		 
		if(pstrWalk->Para.u16StallCnt>TIMEMS(300))
		{
			pstrWalk->Para.u16StallCnt=0;	
			pstrWalk->Para.bTurnFlag=TRUE;			
			pstrWalk->Status.bStallFlag=TRUE;
		}		
	}
	else
	{
		if(pstrWalk->Para.u16StallCnt>10)
			pstrWalk->Para.u16StallCnt-=10;
		else
			pstrWalk->Para.u16StallCnt=0;
		
	}	
	
}

