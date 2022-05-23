/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName:   MowerFSM.c
 Author:     Version :   Date: 
 xyb         1.0         2022��1��11��
 Description: ������Ϣ��ȡ
 Version: �汾��Ϣ
 History: 
 <author>    <time>          <version >   <desc> 
 xyb        2022��1��11��       1.0        ����
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

/* �ṹ��ָ������ */
MOWER_FSM_STR  MowerFSM;  

/* �������� */
void vMowerFSMInit(MOWER_FSM_STR *pstrFSM,BOUNDARY_STR *pstrBoundary);
void vMowerFSMCtrl(MOWER_FSM_STR *pstrMowerFSM);
void vStatusLowPower(MOWER_FSM_STR *pstr);
void vStatusCheckCurOffset(MOWER_FSM_STR *pstr);
void vStatusCheckBoundary(MOWER_FSM_STR *pstr);
void vServoCtrlSeq(MOWER_FSM_STR *pstrFSM);
void vWalkMotorCtrl(WALK_MOTOR_STR *pstrWalkMotor);
void vServoFaultClr(MOWER_FSM_STR *pstrFSM, uint8_t u8AxisNum);
void	vServoPowenOnOff(MOWER_FSM_STR *pstrFSM);
/* ����ָ������ */
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
void vMowerFSMCtrl(MOWER_FSM_STR *pstr)/*1MSִ��һ��*/			
{
	static uint16_t u16OffTimeCount=0;
	static uint32_t u32WorkTimeCount=0;
	
	
	CTRL_LOGIC_STR *pstrCtrlLogic;
	pstrCtrlLogic = &pstr->CtrlLogic;
	
	
	
/************************************************* 
 Function:	  
 Description: �����ػ��İ�������
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/	
	if(strSampleInfo.Switch.onoff_handler_status_out) 
	{			 
			pstr->eFSM=STATUS_STOP;	  		
			pstr->u8OffFlag=TRUE;	  	
		  pstr->u8onoff=LCD_BYEBYE;/*�ı䴮��onff��ֵ*/	
	}
	else
	{
			pstr->u8OffFlag=0;	
  }
	
	if(pstr->u8OffFlag){ 
		 u16OffTimeCount++;
	  if(u16OffTimeCount>OFFTIMEDELAY) /*0.5S��ػ�*/	
		 SET_CTRL_POWER_RLY_LOW();		
	}

	
// 	SET_CTRL_POWER_RLY_HIGH();/*������*/
//	SET_CTRL_BUZZER_ENABLE();		

	if((UartPanel.Out.u8Command==CMD_HOME)||(UartPanel.Out.u8Command==CMD_START))
	{				
		
		pstr->CtrlLogic.Out.bMowerOn=TRUE;			
		
		if(UartPanel.Out.u8Command==CMD_START)/*�ȸ�ݺ��ж��Ƿ񷵻س���վ*/	
		{
			u32WorkTimeCount++;		
			if(u32WorkTimeCount>UartPanel.Out.u32WorkTimeOfOneShotTime)
			{
				u32WorkTimeCount=UartPanel.Out.u32WorkTimeOfOneShotTime;
				if(UartPanel.Out.bReturnofOneShotTime==FALSE)/*�����ػ�վ���*/
				{			
						pstr->CtrlLogic.Out.bMowerOn=FALSE;	;/*ͣ��*/			
					  UartPanel.Out.u8Command=CMD_STOP;
				}
				else/*���ػ�վ���*/
				{
					UartPanel.Out.u8Command=CMD_HOME;  				
				}				
			}		
		}	
		else/*ֱ�ӷ��ػ�վ*/	
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
 Description: ��紦��
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
		  /*���ʹ�� */					 
			SET_CTRL_CHARGE_RLY_ENABLE(); 	
		 }
		 else
		 {
			/*�رճ�� */	
		  SET_CTRL_CHARGE_RLY_DISABLE(); 	
		 }
	 
//		 SET_CTRL_CHARGE_RLY_ENABLE(); 
	 }

	 
/************************************************* 
 Function:	  
 Description: ���ʱ���ʱ
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
 Function:����״̬������״̬�����
 Description: ��������˸
 Input: 	 
 Output:	  
 Return: 
 Others:  
*************************************************/		
	 
	 #define BUZZERONTIME  500//500ms
	 #define BUZZERALLTIME  (6*BUZZERONTIME) //���η���

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
 Description: �������
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
 Description: �����ʾ��״̬����
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/	 
	if(pstr->eFSM==STATUS_MOWING)
	{
	  	if(pstr->WalkMotor.Status.bStallFlag==TRUE)
			{
				/*�����ʾ��ת */
				pstrCtrlLogic->State.eMowerDisplayStatus=TRAPRECOVER;
			}
			else if(pstr->WalkMotor.eMotorFSM==HOME)
			{
				/*�����ʾ���ڸ�� */
				pstrCtrlLogic->State.eMowerDisplayStatus=BACKTOHOME;		
			}	
			else if( pstr->TimeStatistics.Para.u32StartTimeCnt<TIMES(3)) 
			{
				/*������ǰ3S�������ʾ׼����� */
				pstrCtrlLogic->State.eMowerDisplayStatus=READYCUTTING;
			}
			else
			{
				/*�����ʾ���ڸ�� */
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

			/* �Լ� OK */
      pstr->Flg.Bit.bSelfCheckOk = TRUE;
			/* ���ʹ������ */
			pstrCtrlLogic->State.bServoOffReq = FALSE;
			/* �ŷ��Ͽ�ʹ�� */
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
		   

		   
		
		
			memset(&pstr->WalkMotor,0,sizeof(WALK_MOTOR_STR));   /*��ʼ��*/
			memset(&pstr->CutMotor,0,sizeof(CUT_MOTOR_STR));     /*��ʼ��*/		
		
			pstr->WalkMotor.In.peMowerPosition=&Boundary.Out.eMowerPosition;				
			pstr->WalkMotor.In.pu16Q14LeftCurrent=&UartMotor.Out.u16Q14LeftCurrent;
			pstr->WalkMotor.In.pu16Q14RightCurrent=&UartMotor.Out.u16Q14RightCurrent;		
			pstr->CutMotor.In.pu16Q14CutCurrent=&UartMotor.Out.u16Q14CutCurrent;		
				
		  pstr->TimeStatistics.Para.u32StartTimeCnt=0;
		
			vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);				
			vCutMotorLoad(DISABLE,CUTSPEED);
		
			pstrCtrlLogic->State.bBuzzerOn= FALSE;		/*������*/
		  
		
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
           		
			pstrCtrlLogic->State.bServoOnReq = FALSE;	/* ��ʹ��ָ������ */

			if((pstrCtrlLogic->State.bServoOffReq == FALSE) 
			&& (pstrCtrlLogic->State.bFaultOccur == FALSE))
			{
			  /* �ñ�־λ */
				pstr->Flg.Bit.bServOn = TRUE;
				pstrCtrlLogic->State.bServoOn = TRUE;
				/*���ο�����ݵ��ܼ�ʱ */
				pstr->TimeStatistics.Para.u32CutTimeCnt++; 				
				/*���θ�ݼ�ʱ */
				pstr->TimeStatistics.Para.u32StartTimeCnt++;	
				
				/*���ߵ������ */
        if(pstr->TimeStatistics.Para.u32StartTimeCnt>TIMES(5))
				{
					vWalkMotorCtrl(&pstr->WalkMotor);				
				}
				else
				{
					pstrCtrlLogic->State.bBuzzerOn= TRUE;		/*������*/
				}
						
	      /*��ݵ������ */
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
				/* �ñ�־λ */
				pstr->Flg.Bit.bServOn = FALSE;
				pstrCtrlLogic->State.bServoOn = FALSE;
				pstrCtrlLogic->State.bServoOffReq = FALSE;
				pstr->eFSM = STATUS_STOP;						
			}
										
				break;	 
				
		case STATUS_CHARGE:	
   	
				/*��ݵ�����ߵ������ */		
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
 Description: ��ѹ״̬����
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
		
		case 1:/* �ϵ� */
				
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
			/* �ȴ���Դ�ȶ� */
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
          strFaultFun.pvFaultClearByNO(ERR_VOL_UD);   /* ��ֹ�µ�����ϵ� */
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

		case 3: //20ms��ʱ
		   
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
 Description: �������ƫ�ü��״̬
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

		
		/*ƫ�õ�ѹ1.65V  �Ŵ�2.5    0.4 - 0.6 ƫ����ֵ�ж� */
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
 Description: Ϊ�˼���CIA402��ϵͳ״̬��, �������
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

	 
	/* �����������ǰ���� */
	if(*pstrFSM->In.pbFaultRst == TRUE
		&& u8FaultRstBak == FALSE)
	{
		strFaultFun.pvFaultCurrentClear(u8AxisNum);
		pstrCtrlLogic->State.bFaultRst = TRUE;		/* 100us �ź� */
	}
	else
	{
		pstrCtrlLogic->State.bFaultRst = FALSE;
	}
	
	u8FaultRstBak = *pstrFSM->In.pbFaultRst;
}

/************************************************* 
 Function:	  vServoCtrlSeq
 Description: Ϊ�˼���CIA402��ϵͳ״̬��,����ʹ�ܣ���������
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vServoCtrlSeq(MOWER_FSM_STR *pstrFSM)
{
  static uint8_t u8ServoEnableBak = 0;  /* �ŷ�ʹ��ָ��ı��� */
	CTRL_LOGIC_STR *pstrCtrlLogic;
	
	pstrCtrlLogic = &pstrFSM->CtrlLogic;
	
  pstrCtrlLogic->State.bFaultOccur = *pstrFSM->In.pbFaultOccur;
    
    if((*pstrFSM->In.pbServoEnable) == FALSE/* �ŷ���ʹ�� */
       && (u8ServoEnableBak == TRUE))
    {
        pstrCtrlLogic->State.bServoOffReq = TRUE;
        pstrCtrlLogic->State.bServoOnReq = FALSE;
    }
    else if((*pstrFSM->In.pbServoEnable)			            /* �ŷ�ʹ��ָ�������� */
        && (u8ServoEnableBak == FALSE)
        && (pstrCtrlLogic->State.bFaultOccur == FALSE))   /* �������޹��ϵ�״̬�� */
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
 Description: ���ߵ������
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
	
void vWalkMotorCtrl(WALK_MOTOR_STR *pstrWalkMotor)
{	
		static uint16_t u16tTimeRandomReal, u8WheelDir=0;	/*����WALK*/
		static  uint8_t lDecotr,lDecotrBAK;	/*����HOME*/
		
	
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
//			case STOP:  /*ֹͣ*/				
			case WALK:  /*ֱ�� */		
                 
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
								
								
								
								if(*pstrWalkMotor->In.peMowerPosition>MOWER_LIN_RIN)  	/*�л���ת��*/		
								{											
									pstrWalkMotor->Para.bTurnFlag=TRUE; /*��ʱһС����ͣ����,Ȼ������ת״̬*/
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
							 else if(pstrWalkMotor->Para.bTurnFlag==TRUE)/*ֻת��*/
							 {
									pstrWalkMotor->Para.u16TimeCount1MS++;
					
									if(pstrWalkMotor->Para.u16TimeCount1MS<TIMES(0.3))/*ɲ����ͣ*/	//	1)����ɲ����0.2S   2)0.2-1.0 ��ǰ�� �ҷ����� 3)1.0-1.3ɲ�� 4����������
									{ 
										vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
									}
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.3)))/*һ��ǰ����һ�ߺ���*/		
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
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.6)))/*ɲ�� TIMES(0.6)-TIMES(0.3)  */	
									{
											vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);										
									}
									else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(1.0)))/*��ǰ�����ҷ�ǰ�� TIMES(1.2)-TIMES(0.6)*/			/*ת��һ�������ʱ��,����ת�л�����ת*/						
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
			case TURN:/*ת��+����+�����ת��뾶�������*/					
							pstrWalkMotor->Para.u16TimeCount1MS++;
			
			        if(pstrWalkMotor->Para.u16TimeCount1MS<TIMES(0.3))/*ɲ����ͣ*/	//	1)����ɲ����0.2S   2)0.2-1.0 ��ǰ�� �ҷ����� 3)1.0-1.3ɲ�� 4����������
							{ 
								vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
							}
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.3)))/*һ��ǰ����һ�ߺ���*/		
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
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(0.6)))/*ɲ�� TIMES(0.6)-TIMES(0.3)  */	
							{
									vWalkMotorLoad(DISABLE,DISABLE,FORWARD,FORWARD,0,0);										
							}
							else if(pstrWalkMotor->Para.u16TimeCount1MS<(u16tTimeRandomReal+TIMES(1.0)))/*��ǰ�����ҷ�ǰ�� TIMES(1.2)-TIMES(0.6)*/			/*ת��һ�������ʱ��,����ת�л�����ת*/						
							{
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TURNSPEED,TURNSPEED);										
							}else
							{
									pstrWalkMotor->Para.u16TimeCount1MS=0;								
									pstrWalkMotor->eMotorFSM=WALK;								
							}
													
					break;				
				case HOME:/*�߽�����*/	
					       		

								/*������� �Ͳ���ʱ
				          ���ƫ���ˣ�����ת�ٽ��ͣ���ʱ0.3S��ʼ�任����
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
							
				       				
				        if( pstrWalkMotor->Para.bChangeFlag==FALSE)/*���������ʱ*/	
								{
									#if  ALONG==ALONGFORWARD		
									if((lDecotr!=lDecotrBAK)&&(lDecotr!=MOWER_LOUT_RIN))/*�µı߽����ź���ɵĲ�һ�£��ñ�־λ*/	
									#else
									if((lDecotr!=lDecotrBAK)&&(lDecotr!=MOWER_LIN_ROUT))/*�µı߽����ź���ɵĲ�һ�£��ñ�־λ*/													
									#endif
									{
										 pstrWalkMotor->Para.bChangeFlag=TRUE;	/*����ʱ��־λ*/																	 
									}
									else/*�µı߽����ź���ɵ�һ��*/	
									{
										 pstrWalkMotor->Para.bChangeFlag=FALSE;/*����ʱ*/
									}
						  	}		
								else/*����ʱ��ʱ*/				         
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
								
						 lDecotrBAK=lDecotr;/*�µı߽����źŸ�ֵ*/	
							

#if   ALONG==ALONGFORWARD								 
							if(lDecotr==MOWER_LIN_ROUT)  /*��������*/
							 {							  	
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TRACESPEED);											 
							 }						 
					     else  if(lDecotr==MOWER_LOUT_RIN)  /*ֱ��*/
							 {							 								 
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,FORWARD,TRACESPEED,TRACESPEED);								 
							 } 
							 else  if(lDecotr==MOWER_LIN_RIN)  /*�������ڣ���ת*/
							 {
									vWalkMotorLoad(ENABLE,ENABLE,BACKWARD,FORWARD,TRACESPEED,TRACESPEED);										 						
							 }
							 else if(lDecotr==MOWER_LOUT_ROUT)  /*�������⣬��ת*/
							 {							 
									vWalkMotorLoad(ENABLE,ENABLE,FORWARD,BACKWARD,TRACESPEED,TRACESPEED);									 															
							 }	
							 else	
							 {
//								 vWalkMotorLoad(DISABLE,DISABLE,FORWARD,BACKWARD,TURNSPEED,TURNSPEED);				
							 }					 
							break;		 
								
												 
#else   //ALONG==ALONGFORWARD				
							 if(lDecotr==MOWER_LIN_ROUT)  /*��������*/
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



