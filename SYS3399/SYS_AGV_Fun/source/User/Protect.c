/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: Protect.c
 Author:     Version :   Date: 
 Sgk         1.0         2021��8��10��
 Description: �����ļ�
 Version: // �汾��Ϣ
 History: 
 <author>    <time>          <version >   <desc> 
 XYB        2022��2��24��       1.0          ����
***********************************************************/
#include "Protect.h"
#include "Fault.h"
#include "f_FuncCode.h"
#include "f_FuncCode.h"
#include "BSP_gpio.h"
#include "Boundary.h"
#include "qmi8658c_iic.h"
#include "UartToMotor.h"

PROTECT_STR Protect;				/* ���� */	

/* �������� */
void vProtectParaInit(PROTECT_STR *pstr, SAMPLE_INFO_STR *pstrSample,MOWER_FSM_STR *pstrCtrlFSM); 			
void vProtectFast(PROTECT_FAST_STR *pstr);
void vProtect5Ms(PROTECT_5MS_STR *pstr);


/* ����ָ������ */
PROTECT_FUN_STR strProtectFun = 
{
	&vProtectParaInit,
	&vProtectFast,
	&vProtect5Ms
};

void vProtectParaInit(PROTECT_STR *pstr, SAMPLE_INFO_STR *pstrSample,MOWER_FSM_STR *pstrCtrlFSM) 							
{
  
	  pstr->Fast.BatteryVol.In.pu16UdcX10 = &pstrSample->BatteryVol.Out.u16UdcX10;
    pstr->Fast.BatteryVol.In.pu8FSM = &pstrCtrlFSM->eFSM;
	
	  pstr->Fast.Stop.In.pu8FSM= &pstrCtrlFSM->eFSM;
	
	  pstr->Fast.Stop.In.ps16StopADC1=&pstrSample->Stop1.In.s16StopAd;
	  pstr->Fast.Stop.In.ps16StopADC2=&pstrSample->Stop2.In.s16StopAd;
	
    pstr->Fast.Boundary.In.pu8eMowerPosition=&Boundary.Out.eMowerPosition;
	
	  pstr->Fast.ChargeCurrent.In.ps16ChargeCurrentX10=&pstrSample->ChargeCurr.Out.s16IChargeX10;
	
	  pstr->Fast.Roll.In.ps16RollAngle=&qmi_get_date.accelz;
	
	  pstr->Fast.MotorError.In.pu8ErrorCodeLeft =&UartMotor.Out.u8ErrorCodeLeft;
	  pstr->Fast.MotorError.In.pu8ErrorCodeCut  =&UartMotor.Out.u8ErrorCodeCut;
	  pstr->Fast.MotorError.In.pu8ErrorCodeRight=&UartMotor.Out.u8ErrorCodeRight;
}



#if 0

/************************************************* 
 Function:	  vProtectParaInit
 Description: �����������ʼ�� 
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vProtectParaInit(PROTECT_STR *pstr, ASR_STR *pstrAsr, ENCODER_STR *pstrEncoder, SAMPLE_INFO_STR *pstrSample, 
								CTRL_FSM_STR *pstrCtrlFSM, ACR_STR *pstrAcr)
{
    pstr->Fast.MotorFly.In.pstrFlg = &pstrCtrlFSM->Flg;
	pstr->Fast.MotorFly.In.ps32Ref = pstrAsr->SpdPdff.In.ps32Ref;
	pstr->Fast.MotorFly.In.ps32Fbk = pstrAsr->SpdPdff.In.ps32Fbk;
	pstr->Fast.MotorFly.In.ps32Err = &pstrAsr->SpdPdff.Para.s32ErrI;

	pstr->Fast.MotorBlock.In.ps32SpdFbk = pstrAsr->SpdPdff.In.ps32Fbk;
	pstr->Fast.MotorBlock.In.ps32IqRef = &pstrAsr->SpdPdff.Out.s32Out;
	pstr->Fast.MotorBlock.In.ps32IqMax = &pstrAsr->SpdPdff.In.s32OutMax;
	pstr->Fast.MotorBlock.In.ps32IqMin = &pstrAsr->SpdPdff.In.s32OutMin;
	pstr->Fast.MotorBlock.In.pbServoRun = &pstrCtrlFSM->CtrlLogic.State.bServoRun;
	
	pstr->Fast.MotorOverSpd.In.ps32SpdFbk = &pstrEncoder->Out.s32SpeedFbkOrg;
    
    pstr->Fast.DriverTmp.In.ps16Temp = &pstrSample->Temp.Out.s16DriverTemp;
	#if defined (PRO_MOTOR_TRMP_OV) && defined (PRO_MOTOR_TRMP_OV_TIME)
    pstr->Fast.MotorTmp.In.ps16Temp = &pstrSample->Temp.Out.s16MotorTemp;
	#endif
    pstr->Fast.DriverVol.In.pu16UdcX10 = &pstrSample->Vol.Out.u16UdcX10;
    pstr->Fast.DriverVol.In.pu8FSM = &pstrCtrlFSM->eFSM;

    #if defined PRO_MOTOR_LOAD_OV_P15 && defined PRO_MOTOR_LOAD_OV_P25 && defined PRO_MOTOR_LOAD_OV_P30
	/* ������ز��� ��ʼ�� */
	pstr->Ms5.MotorOvLoad.In.s16CircleUs = 5000;
	pstr->Ms5.MotorOvLoad.In.ps32IsFbk = pstrAcr->IqPi.In.ps32Fbk;
	
	pstr->Ms5.MotorOvLoad.In.s32OvLoadP0 = _IQ(1.0) >> 14;  /* Q24 -> Q10 */
	pstr->Ms5.MotorOvLoad.In.s32OvLoadP15 = _IQ(2.25) >> 14;
	pstr->Ms5.MotorOvLoad.In.s32OvLoadP25 = _IQ(6.25) >> 14;	
	pstr->Ms5.MotorOvLoad.In.s32OvLoadP30 = _IQ(9.0) >> 14;

	pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP15 = PRO_MOTOR_LOAD_OV_P15;
	pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP25 = PRO_MOTOR_LOAD_OV_P25;
	pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP30 = PRO_MOTOR_LOAD_OV_P30;

	pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP15 = pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP15 * 1000 / pstr->Ms5.MotorOvLoad.In.s16CircleUs;
	pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP25 = pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP25 * 1000 / pstr->Ms5.MotorOvLoad.In.s16CircleUs;
	pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP30 = pstr->Ms5.MotorOvLoad.In.s32OvLoadTimeP30 * 1000 / pstr->Ms5.MotorOvLoad.In.s16CircleUs;
	
	pstr->Ms5.MotorOvLoad.Para.s32HeartWarnP15 = (pstr->Ms5.MotorOvLoad.In.s32OvLoadP15 - pstr->Ms5.MotorOvLoad.In.s32OvLoadP0)
													* pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP15;
	pstr->Ms5.MotorOvLoad.Para.s32HeartWarnP25 = (pstr->Ms5.MotorOvLoad.In.s32OvLoadP25 - pstr->Ms5.MotorOvLoad.In.s32OvLoadP15)
													* pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP25;
	pstr->Ms5.MotorOvLoad.Para.s32HeartWarnP30 = (pstr->Ms5.MotorOvLoad.In.s32OvLoadP30 - pstr->Ms5.MotorOvLoad.In.s32OvLoadP25)
													* pstr->Ms5.MotorOvLoad.Para.s32OvLoadCntP30;
    #endif
}




/************************************************* 
 Function:	  vProtectParaUpd
 Description: һЩ������Ҫ����
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vProtectParaUpd(PROTECT_STR *pstr, ASR_STR *pstrAsr)
{
    pstr->Fast.MotorFly.In.ps32Ref = pstrAsr->SpdPdff.In.ps32Ref;
}

/************************************************* 
 Function:	  vMotorFlyPro
 Description: ����ɳ�����
 Input: 	  
 Output:	  
 Return: 
 Others: 
    1.λ�û�Ŀǰû�� Ŀǰ�ǿ�ƥ����ɲ��س��ַɳ���� 
 		2.�ٶ�Ŀ���ȶ������Խ��Խ����Ϊ�ɳ�
 		3.�ٶ�Ŀ�겻�ȶ����ٶ����������뷴������
 		�����෴����Ϊ�ɳ�	2022��2��22�� Sgk
*************************************************/
void vMotorFlyPro(MOTOR_FLY_STR *pstr)
{
	static uint8_t i;
	int32_t l_s32Ref;
	int32_t l_s32Fbk;

	int32_t l_s32ErrDeta;

	/* ����� */
    if((pstr->In.pstrFlg)->Bit.bServOn == FALSE || unFuncCode.Code.Group11.u16MotorFlyChk == 0)
    {
		pstr->Para.u16Time = 0;
        return;
    }

	l_s32ErrDeta = (*pstr->In.ps32Err) - pstr->Para.s32ErrBak;
	pstr->Para.s32ErrBak = (*pstr->In.ps32Err);

	#define L_FILT_COEFF				8
	pstr->Para.s32ErrDetaFlt += ((l_s32ErrDeta - pstr->Para.s32ErrDetaFlt) >> L_FILT_COEFF);
	pstr->Para.s32ErrFlt += (((*pstr->In.ps32Err) - pstr->Para.s32ErrFlt) >> L_FILT_COEFF);

	/* �������ж� */
	if((pstr->Para.s32ErrFlt > (1<<(L_FILT_COEFF +1)) &&  pstr->Para.s32ErrDetaFlt > (1<<(L_FILT_COEFF +1)))
		|| (pstr->Para.s32ErrFlt < -(1<<(L_FILT_COEFF +1)) &&  pstr->Para.s32ErrDetaFlt < -(1<<(L_FILT_COEFF +1)))
		)
	{
		/* �˲����ͺ� �ж�ʱ����Ҫ��һ�� */
		if(pstr->Para.u16Time < PRO_MOTOR_FLY_TIME/10)
		{
			pstr->Para.u16Time ++;
		}
		else
		{			
			pstr->Para.u16Time = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_FLY, 0);
		}
	}
	else
	{
		pstr->Para.u16Time = 0;
	}

}

/************************************************* 
 Function:	  vMotorOverSpd
 Description: ������ٱ���
 Input: 	  
 Output:	  
 Return: 
 Others:  ���ٱ�������ֵҪ�������жֵ
*************************************************/
void vMotorOverSpd(MOTOR_OVER_SPD_STR *pstr)
{
	if(abs(_IQmpy((*pstr->In.ps32SpdFbk), BASE_RPM)) > unFuncCode.Code.Group11.u16OverSpdThreshold)
	{
		if(pstr->Para.u16Time < unFuncCode.Code.Group11.u16OverSpdTime)
		{
			pstr->Para.u16Time ++;
		}
		else
		{
			pstr->Para.u16Time = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_SPD_OV, 0);
		}
	}
	else
	{
		pstr->Para.u16Time = 0;
	}

}

/************************************************* 
 Function:	  vMotorBlock
 Description: �����ת����
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vMotorBlock(MOTOR_BLOCK_STR *pstr)
{
	
	if(!*pstr->In.pbServoRun
		|| unFuncCode.Code.Group2.u16CtrlMode == CTRL_TRQ_MODE	/* ����ģʽ�� ת�ٿ���Ϊ0 ���Max �ʲ�����ת */
		|| unFuncCode.Code.Group11.u16MotorBlockChk == 0
		)
	{
		pstr->Para.u16Time = 0;
		return;
	}
	
	/* ת�ٵ�����ֵ ���ٶȻ������� */
	if(abs(*pstr->In.ps32SpdFbk) < _IQ(PRO_MOTOR_BLOCK_SPD / BASE_RPM)
		&& ((*pstr->In.ps32IqMax == *pstr->In.ps32IqRef)
		|| (*pstr->In.ps32IqMin == *pstr->In.ps32IqRef))
		)
	{
		if(pstr->Para.u16Time < PRO_MOTOR_BLOCK_TIME/10)
		{
			pstr->Para.u16Time ++;
		}
		else
		{
			pstr->Para.u16Time = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_BLOCK, 0);
		}

	}
	else
	{
		pstr->Para.u16Time = 0;
	}
}

/************************************************* 
 Function:	  vMotorOverLoad
 Description: ������أ��ȹ��ɣ����� ��ʱ�ޱ���
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vMotorOverLoad(MOTOR_OVER_LOAD_STR *pstr)
{
	if(unFuncCode.Code.Group11.u16OverLoadMotorChk == 0)
	{
		pstr->Para.s32HeartSumP0 = 0;
		pstr->Para.s32HeartSumP15 = 0;
		pstr->Para.s32HeartSumP25 = 0;
		return;
	}
	
	/* ����Isƽ�� */
	pstr->Para.s32RealIs2 = _IQmpy(*pstr->In.ps32IsFbk, *pstr->In.ps32IsFbk);
	pstr->Para.s32RealIs2 = pstr->Para.s32RealIs2 >> 14; /* Q24->Q10 */
	
	/* ����Is2���� */
	pstr->Para.s32HeartIncP0 = pstr->Para.s32RealIs2 - pstr->In.s32OvLoadP0;
	pstr->Para.s32HeartIncP15 = pstr->Para.s32RealIs2 - pstr->In.s32OvLoadP15;	
	pstr->Para.s32HeartIncP25 = pstr->Para.s32RealIs2 - pstr->In.s32OvLoadP25;


	/* �����Is2ʱ����� */
	pstr->Para.s32HeartSumP0 += pstr->Para.s32HeartIncP0;
	pstr->Para.s32HeartSumP15 += pstr->Para.s32HeartIncP15;	
	pstr->Para.s32HeartSumP25 += pstr->Para.s32HeartIncP25;

    if(pstr->Para.s32HeartSumP0 < 0) 
	{
		pstr->Para.s32HeartSumP0 = 0;
	}
	if(pstr->Para.s32HeartSumP15 < 0)
	{
		pstr->Para.s32HeartSumP15 = 0;
	}
	if(pstr->Para.s32HeartSumP25 < 0) 
	{ 
		pstr->Para.s32HeartSumP25 = 0;
	}
    
	if(pstr->Para.s32HeartSumP0 > pstr->Para.s32HeartWarnP15
		|| pstr->Para.s32HeartSumP15 > pstr->Para.s32HeartWarnP15
		|| pstr->Para.s32HeartSumP25 > pstr->Para.s32HeartWarnP25)
	{
		strFaultFun.pvFaultHandle(ERR_MOTOR_LOAD_OV, 0);
	}
	else
	{

	}

}

#endif
/************************************************* 
 Function:	  vUdcPro
 Description: ��������ѹ����
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vUdcPro(UDC_STR *pstrUdc)
{
    /* �Լ�״̬ ���ж� ��Ҫ���� ���ϵ��ѹ������������ */
	if(*pstrUdc->In.pu8FSM == STATUS_LOW_POWER)
	{
			return;
	}
        
	if((*pstrUdc->In.pu16UdcX10) < PRO_VOL_UD)
	{
		if(pstrUdc->Para.u16UdTime < PRO_VOL_UD_TIME)
		{
			pstrUdc->Para.u16UdTime++;
		}
		else
		{
			pstrUdc->Para.u16UdTime = 0;
			strFaultFun.pvFaultHandle(ERR_VOL_UD, 0);		/* ĸ��Ƿѹ */
		}
	}
	else
	{
		pstrUdc->Para.u16UdTime = 0;
	}

	if((*pstrUdc->In.pu16UdcX10) > PRO_VOL_OV)
	{
		if(pstrUdc->Para.u16OvTime < PRO_VOL_OV_TIME)
		{
			pstrUdc->Para.u16OvTime++;
		}
		else
		{
			pstrUdc->Para.u16OvTime = 0;
			strFaultFun.pvFaultHandle(ERR_VOL_OV, 0); /* ĸ�߹�ѹ */
		}
	}
	else
	{
		pstrUdc->Para.u16OvTime = 0;
	}
}

void vStop(STOP_STR *pstrSTOP)
{
//	if(*pstrSTOP->In.pu8FSM !=STATUS_MOWING)
//	{
//			return;
//	}
		
	if((*pstrSTOP->In.ps16StopADC1<PRO_STOP_THRESHOLD)||(*pstrSTOP->In.ps16StopADC2<PRO_STOP_THRESHOLD))
	{
		if(pstrSTOP->Para.u16StopCnt<PRO_STOP_TIME)
		{
			pstrSTOP->Para.u16StopCnt++;
		}
		else
		{
			pstrSTOP->Para.u16StopCnt=0;
			strFaultFun.pvFaultHandle(ERR_STOP, 0);		
		}				
	}
	else
	{
		pstrSTOP->Para.u16StopCnt=0;
	}
}


void vLift(LIFT_STR *pstrLIFT)
{

		pstrLIFT->In.pbLiftLeft = LEFTLIFT_HANDLER;
   	pstrLIFT->In.pbLiftRight= RIGHTLIFT_HANDLER;
	
	
	if((pstrLIFT->In.pbLiftLeft==TRUE)||(pstrLIFT->In.pbLiftRight==TRUE))
	{
		if(pstrLIFT->Para.u16LiftCnt<PRO_STOP_TIME)
		{
			pstrLIFT->Para.u16LiftCnt++;
		}
		else
		{
			pstrLIFT->Para.u16LiftCnt=0;
			strFaultFun.pvFaultHandle(ERR_LIFT, 0);		
		}				
	}
	else
	{
		pstrLIFT->Para.u16LiftCnt=0;
	}
	
	
	
}

void vRoll(ROLL_STR *pstrROLL)
{
   
	if(*pstrROLL->In.ps16RollAngle<PRO_ROLL_ANGLE)
	{
		if(pstrROLL->Para.u16RollCnt<PRO_ROLL_TIME)
		{
			pstrROLL->Para.u16RollCnt++;
		}
		else
		{
			pstrROLL->Para.u16RollCnt=0;
			strFaultFun.pvFaultHandle(ERR_ROLL, 0);		
		}				
	}
	else
	{
		pstrROLL->Para.u16RollCnt=0;
	}
	
}

void	vOverLoad(MOTOR_ERROR_STR *pstrMotorErr)
{
	
	
	if((*pstrMotorErr->In.pu8ErrorCodeLeft==ERR_TMP_OV)||(*pstrMotorErr->In.pu8ErrorCodeCut==ERR_TMP_OV))
	{
		strFaultFun.pvFaultHandle(ERROR_WALK_OVERLOAD, 0);			
	}
	else	if(*pstrMotorErr->In.pu8ErrorCodeCut==ERR_TMP_OV)
	{
		strFaultFun.pvFaultHandle(ERROR_CUT_OVERLOAD, 0);				
	}
	else if((*pstrMotorErr->In.pu8ErrorCodeLeft !=ERR_NO_CODE)
		    ||(*pstrMotorErr->In.pu8ErrorCodeCut  !=ERR_NO_CODE)
	      ||(*pstrMotorErr->In.pu8ErrorCodeRight!=ERR_NO_CODE))
	{
				strFaultFun.pvFaultHandle(ERROR_HARDWARE, 0);	
	}
	
}


void OverChargeCurrent(CHARGECURRENT_STR *pstrCHGCURR)
{
	if((*pstrCHGCURR->In.ps16ChargeCurrentX10)>PRO_CURR_CHARGE)
	{
		if(pstrCHGCURR->Para.u16OverChargeCurrentCnt<PRO_CURR_CHARGE_TIME)
		{
			pstrCHGCURR->Para.u16OverChargeCurrentCnt++;
		}
		else
		{
			strFaultFun.pvFaultHandle(ERROR_OVERCHARGECURRENT, 0);	
			pstrCHGCURR->Para.u16OverChargeCurrentCnt=0;
		}
	}
  else
  {
		pstrCHGCURR->Para.u16OverChargeCurrentCnt=0;
  }	
}


void OutOfBoundary(OUTOFBOUNDARY_STR *pstrBOUNDARY)
{
//	if(*pstrBOUNDARY->In.pu8eMowerPosition==MOWER_NONE)  
		
	if((Boundary.L.Out.u8statues==DETECTORNONE)&&(Boundary.R.Out.u8statues==DETECTORNONE))
	{
		if(pstrBOUNDARY->Para.u16OutOfBoundaryCnt<PRO_BOUNDARY_TIME)
		{
			pstrBOUNDARY->Para.u16OutOfBoundaryCnt++;
		}
		else
		{
			strFaultFun.pvFaultHandle(ERR_OUT_BOUNDARY, 0);	
			pstrBOUNDARY->Para.u16OutOfBoundaryCnt=0;
		}
	}
  else
  {
		pstrBOUNDARY->Para.u16OutOfBoundaryCnt=0;
  }	
	
}





#if 0
/************************************************* 
 Function:	  vDrvTempPro
 Description: ���������±���
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vDrvTempPro(DRV_TEMP_PRO_STR *pstr)
{
	/* ���߼�� */
	if(*pstr->In.ps16Temp == -127)
	{
		if(pstr->Para.u16DiscontCnt < 100)
		{
			pstr->Para.u16DiscontCnt ++;
		}
		else
		{
			pstr->Para.u16DiscontCnt = 0;
			strFaultFun.pvFaultHandle(ERR_TMP_DISCONT, 0);
		}
	}
	else
	{
		pstr->Para.u16DiscontCnt = 0;
	}

	/* ��·��� */
	if(*pstr->In.ps16Temp == 127)
	{
		if(pstr->Para.u16ShortCnt < 100)
		{
			pstr->Para.u16ShortCnt ++;
		}
		else
		{
			pstr->Para.u16ShortCnt = 0;
			strFaultFun.pvFaultHandle(ERR_TMP_SHORT, 0);
		}
	}
	else
	{
		pstr->Para.u16ShortCnt = 0;
	}	
	
	/* ���¼�� */
	if(*pstr->In.ps16Temp > PRO_TEMP_OV && *pstr->In.ps16Temp < 127)
	{
		if(pstr->Para.u16OvTime < PRO_TEMP_OV_TIME/10)
		{
			pstr->Para.u16OvTime++;
		}
		else
		{
			pstr->Para.u16OvTime = 0;
			strFaultFun.pvFaultHandle(ERR_TMP_OV, 0);
		}
	}
	else
	{
		if(pstr->Para.u16OvTime)
		{
			pstr->Para.u16OvTime--;
		}
	}
}

/************************************************* 
 Function:	  vMotorTempPro
 Description: ������±���
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
#if defined (PRO_MOTOR_TRMP_OV) && defined (PRO_MOTOR_TRMP_OV_TIME)
void vMotorTempPro(MOTOR_TEMP_PRO_STR *pstr)
{
	/* ���߼�� */
	if(*pstr->In.ps16Temp >= 260)
	{
		if(pstr->Para.u16DiscontCnt < 100)
		{
			pstr->Para.u16DiscontCnt ++;
		}
		else
		{
			pstr->Para.u16DiscontCnt = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_TMP_DISCONT, 0);
		}
	}
	else
	{
		pstr->Para.u16DiscontCnt = 0;
	}

	/* ��·��� */
	if(*pstr->In.ps16Temp == -32678)
	{
		if(pstr->Para.u16ShortCnt < 100)
		{
			pstr->Para.u16ShortCnt ++;
		}
		else
		{
			pstr->Para.u16ShortCnt = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_TMP_SHORT, 0);
		}
	}
	else
	{
		pstr->Para.u16ShortCnt = 0;
	}	
	
	/* ���¼�� */
	if(*pstr->In.ps16Temp > PRO_MOTOR_TRMP_OV && (*pstr->In.ps16Temp <= 260))
	{
		if(pstr->Para.u16OvTime < PRO_MOTOR_TRMP_OV_TIME/10)
		{
			pstr->Para.u16OvTime++;
		}
		else
		{
			pstr->Para.u16OvTime = 0;
			strFaultFun.pvFaultHandle(ERR_MOTOR_TMP_OV, 0);
		}
	}
	else
	{
		if(pstr->Para.u16OvTime)
		{
			pstr->Para.u16OvTime--;
		}
	}

}
#endif
/************************************************* 
 Function:	  vBrkFaultPro
 Description: ��բ�쳣����
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vBrkFaultPro(BRK_FAULT_STR *pstr)
{
	if(*pstr->In.pbBrkSpdEn == BRK_OFF
		&& abs(*pstr->In.ps32SpdFbk) > _IQ(PRO_BRK_FAULT_SPD/BASE_RPM)
		)
	{
		if(pstr->Para.u16BrkFaultTime < PRO_BRK_FAULT_TIME/10)
		{
			pstr->Para.u16BrkFaultTime ++;
		}
		else
		{
			pstr->Para.u16BrkFaultTime = 0;			
			strFaultFun.pvFaultHandle(ERR_MOTOR_BRK_FAULT, 0);
		}
	}
	else
	{
		pstr->Para.u16BrkFaultTime = 0;
	}
}
#endif
/************************************************* 
 Function:	  vProtectFast
 Description: ���ٱ�����
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vProtectFast(PROTECT_FAST_STR *pstr)
{ 
	
	vUdcPro(&pstr->BatteryVol);
	vStop(&pstr->Stop);
	vLift(&pstr->Lift);	
	vRoll(&pstr->Roll);		
	vOverLoad(&pstr->MotorError);	
	OverChargeCurrent(&pstr->ChargeCurrent);
  OutOfBoundary(&pstr->Boundary);

}

/************************************************* 
 Function:	  vProtect5Ms
 Description: 5Ms�����Ա���
 Input: 	  
 Output:	  
 Return: 
 Others:  
*************************************************/
void vProtect5Ms(PROTECT_5MS_STR *pstr)
{

}

