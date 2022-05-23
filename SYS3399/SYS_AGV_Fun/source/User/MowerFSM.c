/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName:   MowerFSM.c
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
#include "UartToMotor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IQmathLib.h"

#include "ErrCode.h"
#include "MotorSpeedCalc.h"
#include "MowerFSM.h"
#include "UserConfig.h"
#include "Fault.h"
#include "UartToPanel.h"
#include "SampleInfo.h"
#include "qmi8658c_iic.h"

/* 结构体指针声明 */
MOWER_FSM_STR  MowerFSM;  

/* 函数声明 */
void vMowerFSMInit(MOWER_FSM_STR *pstrFSM,BOUNDARY_STR *pstrBoundary);
void vMowerFSMCtrl(MOWER_FSM_STR *pstrMowerFSM);
void vStatusLowPower(MOWER_FSM_STR *pstr);
void vStatusCheckCurOffset(MOWER_FSM_STR *pstr);
void vStatusCheckBoundary(MOWER_FSM_STR *pstr);
void vServoCtrlSeq(MOWER_FSM_STR *pstrFSM);
void vWalkMotorCtrl(WALK_MOTOR_STR *pstrWalkMotor);
void vServoFaultClr(MOWER_FSM_STR *pstrFSM, uint8_t u8AxisNum);
void	vServoPowenOnOff(MOWER_FSM_STR *pstrFSM);
/* 函数指针声明 */
MOWERFSM_FUN_STR strMowerFSMFun = 
{
	&vMowerFSMInit,
	&vMowerFSMCtrl,	
	&vWalkMotorCtrl,
};


void vMowerFSMInit(MOWER_FSM_STR *pstrFSM, BOUNDARY_STR *pstrBoundary)
{
	
	memset(&MowerFSM,0, sizeof(MOWER_FSM_STR));	
	vMotorSpeedCalcInit(&MotorSpeedCalc);	
	
	pstrFSM->In.pbServoEnable = (bool *)&pstrFSM->CtrlLogic.Out.bMowerOn;
	pstrFSM->In.pbFaultRst = (bool *)&UartPanel.Out.bFaultReset;	
	pstrFSM->In.pbFaultOccur = &strFault[0].Out.Flg.bFaultOccur;

	pstrFSM->WalkMotor.In.peMowerPosition=&pstrBoundary->Out.eMowerPosition;
	pstrFSM->WalkMotor.In.pu16Q14LeftCurrent=&UartMotor.Out.u16Q14LeftCurrent;
	pstrFSM->WalkMotor.In.pu16Q14RightCurrent=&UartMotor.Out.u16Q14RightCurrent;		
	
	pstrFSM->CutMotor.In.pu16Q14CutCurrent=&UartMotor.Out.u16Q14CutCurrent;		

	
	
//  MowerFSM.WalkMotor.eMotorFSM=WALK;
//  MowerFSM.eFSM=STATUS_MOWING;
  pstrFSM->u8onoff=LCD_WELCOME;
		
}
/**
* @brief  None
* @param  None
* @retval None

*/
void vMowerFSMCtrl(MOWER_FSM_STR *pstr)/*1MS执行一次*/			
{
	static uint16_t u16OffTimeCount=0;
	static uint32_t u32WorkTimeCount=0;
	
	
	CTRL_LOGIC_STR *pstrCtrlLogic;
	pstrCtrlLogic = &pstr->CtrlLogic;
	
	
	
/************************************************* 
 Function:	  
 Description: 开机关机的按键处理
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/	
	if(strSampleInfo.Switch.onoff_handler_status_out) 
	{			 
			pstr->eFSM=STATUS_STOP;	  		
			pstr->u8OffFlag=TRUE;	  	
		  pstr->u8onoff=LCD_BYEBYE;/*改变串口onff的值*/	
	}
	else
	{
			pstr->u8OffFlag=0;	
  }
	
	if(pstr->u8OffFlag){ 
		 u16OffTimeCount++;
	  if(u16OffTimeCount>OFFTIMEDELAY) /*0.5S后关机*/	
		 SET_CTRL_POWER_RLY_LOW();		
	}

	
// 	SET_CTRL_POWER_RLY_HIGH();/*测试用*/
//	SET_CTRL_BUZZER_ENABLE();		

	if((UartPanel.Out.u8Command==CMD_HOME)||(UartPanel.Out.u8Command==CMD_START))
	{				
		
		pstr->CtrlLogic.Out.bMowerOn=TRUE;			
		
		if(UartPanel.Out.u8Command==CMD_START)/*先割草后判断是否返回充电基站*/	
		{
			u32WorkTimeCount++;		
			if(u32WorkTimeCount>UartPanel.Out.u32WorkTimeOfOneShotTime)
			{
				u32WorkTimeCount=UartPanel.Out.u32WorkTimeOfOneShotTime;
				if(UartPanel.Out.bReturnofOneShotTime==FALSE)/*不返回基站充电*/
				{			
						pstr->CtrlLogic.Out.bMowerOn=FALSE;	;/*停机*/			
					  UartPanel.Out.u8Command=CMD_STOP;
				}
				else/*返回基站充电*/
				{
					UartPanel.Out.u8Command=CMD_HOME;  				
				}				
			}		
		}	
		else/*直接返回基站*/	
		{
			
		}
	
	}
	else if(UartPanel.Out.u8Command==CMD_STOP)
	{
		UartPanel.Out.u8CommandBAK=CMD_STOP;
		pstr->CtrlLogic.Out.bMowerOn=FALSE;
		
	}
	else
	{
		
		pstr->CtrlLogic.Out.bMowerOn=FALSE;
	}
   
	
	
	if(pstrCtrlLogic->State.bFaultOccur == TRUE)
	{
		UartPanel.Out.u8Command=0;
		UartPanel.Out.u8CommandBAK=0;		
		UartPanel.Out.u32WorkTimeOfOneShotTime=0;
		UartPanel.Out.bReturnofOneShotTime=0;
		u32WorkTimeCount=0;
	}
	else
	{
		
	}
	
	
/************************************************* 
 Function:	  
 Description: 充电处理
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/	
	
	 if(strSampleInfo.ChargeVol.Out.u16UChargeX10>220)
	 {		 		 
	   UartPanel.Out.u8Command=STOP;	 
		 if(strFault[0].Out.Flg.bFaultOccur==FALSE)
		 {
		  /*充电使能 */					 
			SET_CTRL_CHARGE_RLY_ENABLE(); 	
		 }
		 else
		 {
			/*关闭充电 */	
		  SET_CTRL_CHARGE_RLY_DISABLE(); 	
		 }
	 
//		 SET_CTRL_CHARGE_RLY_ENABLE(); 
	 }

	 
/************************************************* 
 Function:	  
 Description: 割草时间计时
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/		 
	 if(pstr->TimeStatistics.Para.u32CutTimeCnt>60000)
	 {
		 pstr->TimeStatistics.Para.u32CutTimeCnt=0;
		 pstr->TimeStatistics.Out.u8TimeMin++;	 
		 if(pstr->TimeStatistics.Out.u8TimeMin>60)
		 {
			 pstr->TimeStatistics.Out.u8TimeMin=0;
			 pstr->TimeStatistics.Out.u16TimeHour++;
		 }
	 }
	 
/************************************************* 
 Function:故障状态和启动状态会蜂鸣
 Description: 蜂鸣器闪烁
 Input: 	 
 Output:	  
 Return: 
 Others:  
*************************************************/		
	 
	 #define BUZZERONTIME  500//500ms
	 #define BUZZERALLTIME  (6*BUZZERONTIME) //三次蜂鸣

	 if(pstrCtrlLogic->State.bBuzzerOn==TRUE)
	 {
			if(pstr->TimeStatistics.Para.u16BuzzerCnt<BUZZERALLTIME)
			{
				 pstr->TimeStatistics.Para.u16BuzzerCnt++;
				 if(pstr->TimeStatistics.Para.u16BuzzerCnt%BUZZERONTIME==0)
				 {
					 SET_CTRL_BUZZER_TOGGLE(); 			 
				 }						
			}				 
	 }
	 else
	 {
		 SET_CTRL_BUZZER_DISABLE();
		 pstr->TimeStatistics.Para.u16BuzzerCnt=0;	 
	 }
	 
/************************************************* 
 Function:	  
 Description: 故障清除
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/		

 if(pstrCtrlLogic->State.bFaultOccur == TRUE)
 {	 
	 
	 if((strFault[0].Out.CurrentFault.u16ErrCode==ERR_LIFT)||(strFault[0].Out.CurrentFault.u16ErrCode==ERR_ROLL))
	 {
		 pstrCtrlLogic->State.bBuzzerOn=TRUE;
	 }
	 	 
	 if((strSampleInfo.Switch.LeftLiftStatus==TRUE)&&(strSampleInfo.Switch.LeftLiftStatus==TRUE))
	 {
		 strFaultFun.pvFaultClearByNO(ERR_LIFT);
	 }
	    
	 if((strSampleInfo.Stop1.In.s16StopAd>PRO_STOP_THRESHOLD)&&(strSampleInfo.Stop2.In.s16StopAd>PRO_STOP_THRESHOLD))
	 {	 
		 strFaultFun.pvFaultClearByNO(ERR_STOP);	 
	 }
	 
	 if(qmi_get_date.accelz>PRO_ROLL_ANGLE)
	 {	 
		 strFaultFun.pvFaultClearByNO(ERR_ROLL);	 
	 }	 
 } 
 else
 {
	 
 }
 

	 
/************************************************* 
 Function:	  
 Description: 面板显示的状态处理
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/	 
	if(pstr->eFSM==STATUS_MOWING)
	{
	  	if(pstr->WalkMotor.Status.bStallFlag==TRUE)
			{
				/*面板显示堵转 */
				pstrCtrlLogic->State.eMowerDisplayStatus=TRAPRECOVER;
			}
			else if(pstr->WalkMotor.eMotorFSM==HOME)
			{
				/*面板显示正在割草 */
				pstrCtrlLogic->State.eMowerDisplayStatus=BACKTOHOME;		
			}	
			else if( pstr->TimeStatistics.Para.u32StartTimeCnt<TIMES(3)) 
			{
				/*刚启动前3S内面板显示准备割草 */
				pstrCtrlLogic->State.eMowerDisplayStatus=READYCUTTING;
			}
			else
			{
				/*面板显示正在割草 */
				pstrCtrlLogic->State.eMowerDisplayStatus=CUTTING;				
			}
		
	}
	else
	{
		pstrCtrlLogic->State.eMowerDisplayStatus=DISPLAYNONE;
	}
 
	
	
	
		
	vServoPowenOnOff(pstr);
	vServoCtrlSeq(pstr);
	
	switch(pstr->eFSM)
	{
			
		case STATUS_LOW_POWER:
			vStatusLowPower(pstr);
		break;		
		
		case STATUS_CHECK_CHARGECURR_OFFSET:
			vStatusCheckCurOffset(pstr);
			break;		
		
		case STATUS_CHECK_BOUNDARY:
			vStatusCheckBoundary(pstr);
			break;			
		
		case STATUS_STOP:  

			vServoFaultClr(pstr, 0);
			
			if(!pstr->SelfCheck.Flg.Bit.bVolOk)
			{
				pstr->eFSM = STATUS_LOW_POWER;
				break;
			}

			if(!pstr->SelfCheck.Flg.Bit.bCurOffsetOk)
			{
				pstr->eFSM = STATUS_CHECK_CHARGECURR_OFFSET;
				break;
			}		
			
			if(!pstr->SelfCheck.Flg.Bit.bBoundaryOk)
			{
				pstr->eFSM = STATUS_CHECK_BOUNDARY;
				break;
			}			

			/* 自检 OK */
      pstr->Flg.Bit.bSelfCheckOk = TRUE;
			/* 清断使能请求 */
			pstrCtrlLogic->State.bServoOffReq = FALSE;
			/* 伺服断开使能 */
			pstr->Flg.Bit.bServOn = FALSE;
						
			if((pstrCtrlLogic->State.bServoOnReq == TRUE
			&& pstrCtrlLogic->State.bFaultOccur == FALSE))			               
			{

				pstrCtrlLogic->State.bServoOnReq = FALSE;
         pstr->eFSM = STATUS_STOPTOMOWING;
			}
			
		
			vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);	
			vCutMotorLoad(DISABLE,CUTSPEED);
			
		 break;
			
		case STATUS_STOPTOMOWING:		
		   

		   
		
		
			memset(&pstr->WalkMotor,0,sizeof(WALK_MOTOR_STR));   /*初始化*/
			memset(&pstr->CutMotor,0,sizeof(CUT_MOTOR_STR));     /*初始化*/		
		
			pstr->WalkMotor.In.peMowerPosition=&Boundary.Out.eMowerPosition;				
			pstr->WalkMotor.In.pu16Q14LeftCurrent=&UartMotor.Out.u16Q14LeftCurrent;
			pstr->WalkMotor.In.pu16Q14RightCurrent=&UartMotor.Out.u16Q14RightCurrent;		
			pstr->CutMotor.In.pu16Q14CutCurrent=&UartMotor.Out.u16Q14CutCurrent;		
				
		  pstr->TimeStatistics.Para.u32StartTimeCnt=0;
		
			vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);				
			vCutMotorLoad(DISABLE,CUTSPEED);
		
			pstrCtrlLogic->State.bBuzzerOn= FALSE;		/*蜂鸣器*/
		  
		
			if((pstrCtrlLogic->State.bServoOffReq == FALSE
			&& pstrCtrlLogic->State.bFaultOccur == FALSE))	
			{
			  pstr->eFSM = STATUS_MOWING;				
			}		
			else
			{
			  pstr->eFSM = STATUS_STOP;			
			}				 
				 	
			break;
		case STATUS_MOWING:		
           		
			pstrCtrlLogic->State.bServoOnReq = FALSE;	/* 上使能指令清零 */

			if((pstrCtrlLogic->State.bServoOffReq == FALSE) 
			&& (pstrCtrlLogic->State.bFaultOccur == FALSE))
			{
			  /* 置标志位 */
				pstr->Flg.Bit.bServOn = TRUE;
				pstrCtrlLogic->State.bServoOn = TRUE;
				/*本次开机割草的总计时 */
				pstr->TimeStatistics.Para.u32CutTimeCnt++; 				
				/*本次割草计时 */
				pstr->TimeStatistics.Para.u32StartTimeCnt++;	
				
				/*行走电机控制 */
        if(pstr->TimeStatistics.Para.u32StartTimeCnt>TIMES(5))
				{
					vWalkMotorCtrl(&pstr->WalkMotor);				
				}
				else
				{
					pstrCtrlLogic->State.bBuzzerOn= TRUE;		/*蜂鸣器*/
				}
						
	      /*割草电机控制 */
        if(pstr->TimeStatistics.Para.u32StartTimeCnt<TIMES(10))
				{					
					vCutMotorLoad(DISABLE,CUTSPEED);	
				}
				else
				{
					vCutMotorLoad(ENABLE,CUTSPEED);					
				}
							
			}
			else
			{
				/* 置标志位 */
				pstr->Flg.Bit.bServOn = FALSE;
				pstrCtrlLogic->State.bServoOn = FALSE;
				pstrCtrlLogic->State.bServoOffReq = FALSE;
				pstr->eFSM = STATUS_STOP;						
			}
										
				break;	 
				
		case STATUS_CHARGE:	
   	
				/*割草电机行走电机控制 */		
        vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,2500,2500);		
				vCutMotorLoad(DISABLE,CUTSPEED);
				break;	 		
		
			
		default:
			   vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,2500,2500);				
			  break;
	}
	
}	
	
/************************************************* 
 Function:	  vStatusLowPower
 Description: 低压状态控制
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vStatusLowPower(MOWER_FSM_STR *pstr)
{
	static uint16_t u16CountPowerOn=0; 
	
	uint16_t u16UdcLowLimt;
	uint16_t u16UdcBigFlt;
	uint32_t u32AftConRelayDelay=0;
	
	SELF_CHECK_STR *pstrSelfCheck;
	pstrSelfCheck = &pstr->SelfCheck;
		
	u16UdcLowLimt = PRO_VOL_UD ;
	u16UdcBigFlt = *pstrSelfCheck->Vol.pu16UdcBigFlt;
	

	switch(pstrSelfCheck->u8Step)
	{
		
		case 1:/* 上电 */
				
			if(u16CountPowerOn<200)
			{
				u16CountPowerOn++;				
			}
			else
			{
				SET_CTRL_POWER_RLY_HIGH(); 
				pstrSelfCheck->u8Step++;				
			}			
			break;
			
		case 2:
			/* 等待电源稳定 */
			if(u16UdcBigFlt > pstrSelfCheck->Vol.u16UdcOld)
			{
				pstrSelfCheck->Vol.u16WaiteTime = 0;
        pstrSelfCheck->Vol.u16ErrTime = 0;
				pstrSelfCheck->Vol.u16UdcOld = u16UdcBigFlt;		
				return;
			}
	
			if(u16UdcBigFlt >= u16UdcLowLimt)
			{
         pstrSelfCheck->Vol.u16ErrTime = 0;
				if((pstrSelfCheck->Vol.u16WaiteTime++) > 200)
				{
					pstrSelfCheck->u8Step++;
					pstrSelfCheck->Vol.u16WaiteTime = 0;
          strFaultFun.pvFaultClearByNO(ERR_VOL_UD);   /* 防止下电快速上电 */
				}
			}
			else
			{
				if((pstrSelfCheck->Vol.u16ErrTime++) > 200)
				{
					pstrSelfCheck->Vol.u16ErrTime = 0;
					strFaultFun.pvFaultHandle(ERR_VOL_UD, 0);	
				}
			}		
			break;

		case 3: //20ms延时
		   
			if((pstrSelfCheck->Vol.u16WaiteTime++) >= 100) 
			{
				if(pstrSelfCheck->Vol.u16WaiteTime >= u32AftConRelayDelay) 
				{							
					pstr->eFSM = STATUS_STOP;
					pstrSelfCheck->u8Step = 1;
					pstrSelfCheck->Flg.Bit.bVolOk= 1;					
				}				
			}
			break;

		default:

			pstrSelfCheck->Vol.u16WaiteTime = 0;
			pstrSelfCheck->Vol.u16UdcOld = u16UdcBigFlt;
			pstrSelfCheck->u8Step = 1;
			break;
	}
	
}

/************************************************* 
 Function:	  vStatusCheckCurOffset
 Description: 电流检测偏置检测状态
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/

void vStatusCheckCurOffset(MOWER_FSM_STR *pstr)
{
	uint8_t AxisNum=0;
	SELF_CHECK_STR *pstrSelfCheck;
	pstrSelfCheck = &pstr->SelfCheck;
	
	if(pstrSelfCheck->CurOffset.u16CurOffSetCnt < 256)
	{
		SET_CTRL_CHARGE_RLY_DISABLE();
		pstrSelfCheck->CurOffset.u16CurOffSetCnt++;
		pstrSelfCheck->CurOffset.s32IOffSetAdSum = 0;				
	}
	else if(pstrSelfCheck->CurOffset.u16CurOffSetCnt < 512)
	{
		pstrSelfCheck->CurOffset.u16CurOffSetCnt++;
		pstrSelfCheck->CurOffset.s32IOffSetAdSum += ADC_CHARGECURR;
	}
	else
	{      
		pstrSelfCheck->CurOffset.s32IOffsetAd = pstrSelfCheck->CurOffset.s32IOffSetAdSum >> 8;				
		pstrSelfCheck->CurOffset.u16CurOffSetCnt = 0;
		pstrSelfCheck->CurOffset.s32IOffSetAdSum = 0;

		
		/*偏置电压1.65V  放大2.5    0.4 - 0.6 偏置阈值判断 */
		if(pstrSelfCheck->CurOffset.s32IOffsetAd > (6* ADC_MAX / 10) || pstrSelfCheck->CurOffset.s32IOffsetAd < (4 * ADC_MAX / 10))
		{		
			strFaultFun.pvFaultHandle(ERR_OPA_OFFSET_A, AxisNum);
		}
		else
		{
			pstrSelfCheck->Flg.Bit.bCurOffsetOk = 1;
			pstr->eFSM = STATUS_STOP;
		}
	}
}


void vStatusCheckBoundary(MOWER_FSM_STR *pstr)
{
	uint8_t lu8Decotr;
	static uint8_t lu8Time=0;
	
	SELF_CHECK_STR *pstrSelfCheck;
	pstrSelfCheck = &pstr->SelfCheck;	
	
	lu8Decotr=*pstr->WalkMotor.In.peMowerPosition;
	
	if(lu8Time<250)
	{		
		lu8Time++;
		
		if(lu8Decotr==MOWER_LIN_RIN)
		{
			pstrSelfCheck->Boundary.u16BoundaryCnt++;
			
			if(pstrSelfCheck->Boundary.u16BoundaryCnt>200)
			{
					pstrSelfCheck->Flg.Bit.bBoundaryOk = 1;
					pstrSelfCheck->Boundary.u16BoundaryCnt=0;
					pstr->eFSM = STATUS_STOP;
			}
		}
		else
		{
			if(pstrSelfCheck->Boundary.u16BoundaryCnt>10)
			{
				pstrSelfCheck->Boundary.u16BoundaryCnt-=10;
			}
			else
			{
				pstrSelfCheck->Boundary.u16BoundaryCnt=0;
			}
		}
 }
	else
	{
			strFaultFun.pvFaultHandle(ERR_OUT_BOUNDARY, 0);
	}
	
}


/************************************************* 
 Function:	  vServoFaultClr
 Description: 为了兼容CIA402的系统状态机, 故障清除
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vServoFaultClr(MOWER_FSM_STR *pstrFSM, uint8_t u8AxisNum)
{
	static uint8_t u8FaultRstBak = 0;
	CTRL_LOGIC_STR *pstrCtrlLogic;

	pstrCtrlLogic = &pstrFSM->CtrlLogic;

	 
	/* 上升沿清除当前故障 */
	if(*pstrFSM->In.pbFaultRst == TRUE
		&& u8FaultRstBak == FALSE)
	{
		strFaultFun.pvFaultCurrentClear(u8AxisNum);
		pstrCtrlLogic->State.bFaultRst = TRUE;		/* 100us 信号 */
	}
	else
	{
		pstrCtrlLogic->State.bFaultRst = FALSE;
	}
	
	u8FaultRstBak = *pstrFSM->In.pbFaultRst;
}

/************************************************* 
 Function:	  vServoCtrlSeq
 Description: 为了兼容CIA402的系统状态机,解析使能，故障数据
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vServoCtrlSeq(MOWER_FSM_STR *pstrFSM)
{
  static uint8_t u8ServoEnableBak = 0;  /* 伺服使能指令的备份 */
	CTRL_LOGIC_STR *pstrCtrlLogic;
	
	pstrCtrlLogic = &pstrFSM->CtrlLogic;
	
  pstrCtrlLogic->State.bFaultOccur = *pstrFSM->In.pbFaultOccur;
    
    if((*pstrFSM->In.pbServoEnable) == FALSE/* 伺服下使能 */
       && (u8ServoEnableBak == TRUE))
    {
        pstrCtrlLogic->State.bServoOffReq = TRUE;
        pstrCtrlLogic->State.bServoOnReq = FALSE;
    }
    else if((*pstrFSM->In.pbServoEnable)			            /* 伺服使能指令上升沿 */
        && (u8ServoEnableBak == FALSE)
        && (pstrCtrlLogic->State.bFaultOccur == FALSE))   /* 并且在无故障的状态下 */
    {
        pstrCtrlLogic->State.bServoOnReq = TRUE;
        pstrCtrlLogic->State.bServoOffReq = FALSE;
    }
    
    u8ServoEnableBak = *pstrFSM->In.pbServoEnable;
}


void	vServoPowenOnOff(MOWER_FSM_STR *pstrFSM)
{
	
	
	
}
/************************************************* 
 Function:	  vWalkMotorCtrl
 Description: 行走电机控制
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
	
void vWalkMotorCtrl(WALK_MOTOR_STR *pstrWalkMotor)
{	
		static uint16_t u16tTimeRandomReal, u8WheelDir=0;	/*用于WALK*/
		static  uint8_t lDecotr,lDecotrBAK;	/*用于HOME*/
		
	
		if((UartPanel.Out.u8Command==CMD_START)&&(UartPanel.Out.u8CommandBAK!=CMD_START))
		{
			pstrWalkMotor->eMotorFSM=WALK;
		}
		else if((UartPanel.Out.u8Command==CMD_HOME)&&(UartPanel.Out.u8CommandBAK!=CMD_HOME))
		{
			pstrWalkMotor->eMotorFSM=HOME;
		}
		
		UartPanel.Out.u8CommandBAK=UartPanel.Out.u8Command;
	  
		
		switch (pstrWalkMotor->eMotorFSM)
		{
//			case STOP:  /*停止*/				
			case WALK:  /*直行 */		
                 
			        if(pstrWalkMotor->Para.bTurnFlag!=TRUE)
							{
								vStallCheck(pstrWalkMotor);
								vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,WALKSPEED,WALKSPEED);		
											
								if(pstrWalkMotor->Para.bTurnFlag==TRUE)
								{
									
									pstrWalkMotor->Para.u16TimeCount1MS=0;																								
									MotorSpeedCalc.s32AnglePu=MowerFSM.u8TimeRandom*46603;/*46603*360=_IQ(1)*/ 

									MotorSpeedCalc.s32SpeedRPM=TURNSPEED;
									vMotorSpeedCalcCtrl(&MotorSpeedCalc);
									u16tTimeRandomReal=MotorSpeedCalc.s32TurnMs+100;
									
									 if(MowerFSM.u8TimeRandom>(RANDOMMAX/2))   			 
										 u8WheelDir=TURNLEFT;		 
									 else   
										 u8WheelDir=TURNRIGHT;									 															
								}
								
								
								
								if(*pstrWalkMotor->In.peMowerPosition>MOWER_LIN_RIN)  	/*切换到转弯*/		
								{											
									pstrWalkMotor->Para.bTurnFlag=TRUE; /*延时一小会再停下来,然后再跳转状态*/
									pstrWalkMotor->Para.u16TimeCount1MS=0;
																	
									MotorSpeedCalc.s32AnglePu=_IQ(0.5);/*IQ1==360*/									
									MotorSpeedCalc.s32AnglePu=MowerFSM.u8TimeRandom*46603;/*46603*360=_IQ(1)*/ 

									MotorSpeedCalc.s32SpeedRPM=TURNSPEED;
									vMotorSpeedCalcCtrl(&MotorSpeedCalc);
									u16tTimeRandomReal=MotorSpeedCalc.s32TurnMs+100;
									

									#if  DECTOR == UP  
									 if(*pstrWalkMotor->In.peMowerPosition==MOWER_LIN_ROUT)   			 u8WheelDir=TURNLEFT;		 
									 else  if(*pstrWalkMotor->In.peMowerPosition==MOWER_LOUT_RIN)   u8WheelDir=TURNRIGHT;
									 else  u8WheelDir=BACKWARD;															
									#else 							
									 if(*pstrWalkMotor->In.peMowerPosition==MOWER_LIN_ROUT)   			 u8WheelDir=FORWARD;		
									 else  if(*pstrWalkMotor->In.peMowerPosition==MOWER_LOUT_RIN)   u8WheelDir=BACKWARD;
									 else  u8WheelDir=BACKWARD;
									#endif		
							  }
								
						   }
							 else if(pstrWalkMotor->Para.bTurnFlag==TRUE)/*只转弯*/
							 {
									pstrWalkMotor->Para.u16TimeCount1MS++;
					
									if(pstrWalkMotor->Para.u16TimeCount1MS<TIMES(0.3))/*刹车急停*/	//	1)立马刹车到0.2S   2)0.2-1.0 左方前进 右方后退 3)1.0-1.3刹车 4）正常启动
									{ 
										vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
									}
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.3)))/*一边前进，一边后退*/		
									{																
										if(u8WheelDir==TURNLEFT)
										{			
											vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TURNSPEED,TURNSPEED);										
										}
										else
										{
											vWalkMotorLoad(ENABLE,ENABLE,FORWARD,BACKWARD,TURNSPEED,TURNSPEED);																
										}						
									}
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.6)))/*刹车 TIMES(0.6)-TIMES(0.3)  */	
									{
											vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);										
									}
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(1.0)))/*左方前进，右方前进 TIMES(1.2)-TIMES(0.6)*/			/*转向一个随机的时间,从正转切换到反转*/						
									{
											vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
									}else
									{
											pstrWalkMotor->Para.u16TimeCount1MS=0;	
											pstrWalkMotor->Para.bTurnFlag=FALSE;
											pstrWalkMotor->Status.bStallFlag=FALSE;										
									}
								}
																										
						break;						
			case TURN:/*转弯+倒车+随机，转弯半径可以随机*/					
							pstrWalkMotor->Para.u16TimeCount1MS++;
			
			        if(pstrWalkMotor->Para.u16TimeCount1MS<TIMES(0.3))/*刹车急停*/	//	1)立马刹车到0.2S   2)0.2-1.0 左方前进 右方后退 3)1.0-1.3刹车 4）正常启动
							{ 
								vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
							}
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.3)))/*一边前进，一边后退*/		
							{																
								if(u8WheelDir==TURNLEFT)
								{			
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TURNSPEED,TURNSPEED);										
								}
								else
								{
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,BACKWARD,TURNSPEED,TURNSPEED);																
								}						
							}
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.6)))/*刹车 TIMES(0.6)-TIMES(0.3)  */	
							{
									vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);										
							}
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(1.0)))/*左方前进，右方前进 TIMES(1.2)-TIMES(0.6)*/			/*转向一个随机的时间,从正转切换到反转*/						
							{
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
							}else
							{
									pstrWalkMotor->Para.u16TimeCount1MS=0;								
									pstrWalkMotor->eMotorFSM=WALK;								
							}
													
					break;				
				case HOME:/*边界线走*/	
					       		

								/*如果回正 就不延时
				          如果偏掉了，立马转速降低，延时0.3S后开始变换方向
				        */				
								lDecotr=*pstrWalkMotor->In.peMowerPosition;
															
						    if((lDecotr==MOWER_LIN_RIN)&&(pstrWalkMotor->Para.bFindBoundary==FALSE))
								{									
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,2000,2000);											
                  break;									
								}
								else
								{
									pstrWalkMotor->Para.bFindBoundary=TRUE;
								}
							
				       				
				        if( pstrWalkMotor->Para.bChangeFlag==FALSE)/*如果不在延时*/	
								{
									#if  ALONG==ALONGFORWARD		
									if((lDecotr!=lDecotrBAK)&&(lDecotr!=MOWER_LOUT_RIN))/*新的边界检测信号与旧的不一致，置标志位*/	
									#else
									if((lDecotr!=lDecotrBAK)&&(lDecotr!=MOWER_LIN_ROUT))/*新的边界检测信号与旧的不一致，置标志位*/													
									#endif
									{
										 pstrWalkMotor->Para.bChangeFlag=TRUE;	/*置延时标志位*/																	 
									}
									else/*新的边界检测信号与旧的一致*/	
									{
										 pstrWalkMotor->Para.bChangeFlag=FALSE;/*不延时*/
									}
						  	}		
								else/*在延时计时*/				         
							  {
								 pstrWalkMotor->Para.u16Delaytime++;
								 if(pstrWalkMotor->Para.u16Delaytime<TIMES(0.3))  
								 {									 
									 break;											 							 
								 }
								 else
								 {
									 pstrWalkMotor->Para.u16Delaytime=0;
									  pstrWalkMotor->Para.bChangeFlag=FALSE;									 
								 }									 
							  }
								
						 lDecotrBAK=lDecotr;/*新的边界检测信号赋值*/	
							

#if   ALONG==ALONGFORWARD								 
							if(lDecotr==MOWER_LIN_ROUT)  /*左外右内*/
							 {							  	
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TRACESPEED);											 
							 }						 
					     else  if(lDecotr==MOWER_LOUT_RIN)  /*直行*/
							 {							 								 
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TRACESPEED,TRACESPEED);								 
							 } 
							 else  if(lDecotr==MOWER_LIN_RIN)  /*左内右内，左转*/
							 {
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TRACESPEED);										 						
							 }
							 else if(lDecotr==MOWER_LOUT_ROUT)  /*左外右外，右转*/
							 {							 
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,BACKWARD,TRACESPEED,TRACESPEED);									 															
							 }	
							 else	
							 {
//								 vWalkMotorLoad(DISABLE,DISABLE,FORWARD,BACKWARD,TURNSPEED,TURNSPEED);				
							 }					 
							break;		 
								
												 
#else   //ALONG==ALONGFORWARD				
							 if(lDecotr==MOWER_LIN_ROUT)  /*左内右外*/
							 {								  
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TRACESPEED,TRACESPEED);										 
							 }						 
					     else  if(lDecotr==MOWER_LOUT_RIN)  
							 {							 								 
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TURNSPEED);										 
							 } 
							 else  if(lDecotr==MOWER_LIN_RIN)  
							 {
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,BACKWARD,TRACESPEED,TRACESPEED);			
							 }
							 else if(lDecotr==MOWER_LOUT_ROUT) 
							 {
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TRACESPEED);																			
							 }								
							break;	
#endif
	
			default: 			
							vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);						

							break;
		}

}	



