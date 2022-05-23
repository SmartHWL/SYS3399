/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: Fault.c
 Author:     Version :   Date: 
 Sgk         1.0         2021��8��10��
 Description: ���Ϲ���
 Version: // �汾��Ϣ
 History: 
 <author>    <time>          <version >   <desc> 
 Sgk         2021��8��10��       1.0          ����
***********************************************************/

#include "Fault.h"
//#include "TypeDef.h"
//#include "f_Eeprom.h"  /*������EEROM�����*/
#include "f_FuncCode.h"
#include "Control&Filter.h"
#include "stdint.h"
#include "string.h"

/* �������� */		
ERROR_SCENE_STR s16FaultRecordList[ERROR_SCENE_DEPTH];	/* ����ʱ��������� */

uint16_t u16FaultHistoryList[MOTOR_AXIS][FAULT_RCD_NUM];	/* ��ʷ���� */

const FAULT_INFO_STR strFaultList[255] =			/* ���ϲ�ѯ�� */
{

	/*   ������                ������ 	  	    	����	   �����	    ����ֹͣ      	�������� */
	[ERR_OPA_OFFSET_A]      = {ERR_OPA_OFFSET_A,		FALSE,		FALSE,		TRUE},		/* A���˷�ƫ�õ�ѹ���� */	
	[ERR_OPA_OFFSET_B]      = {ERR_OPA_OFFSET_B,		FALSE,		FALSE,		TRUE},		/* B���˷�ƫ�õ�ѹ���� */	
	[ERR_OPA_OFFSET_C]      = {ERR_OPA_OFFSET_C,		FALSE,		FALSE,		TRUE},		/* C���˷�ƫ�õ�ѹ���� */
	[ERR_OC_FAULT]          = {ERR_OC_FAULT,			  TRUE,		  FALSE,		TRUE},		    /* Ӳ������ */
	[ERR_VOL_UD]            = {ERR_VOL_UD, 			    FALSE,  	TRUE,  		TRUE},		/* ĸ��Ƿѹ���� */	
	[ERR_VOL_OV]            = {ERR_VOL_OV, 			    TRUE,  		TRUE,  		TRUE},		/* ĸ�߹�ѹ���� */
	
	[ERR_TMP_OV]            = {ERR_TMP_OV,			    TRUE,		TRUE,		TRUE},			
	[ERR_TMP_DISCONT]       = {ERR_TMP_DISCONT,		  TRUE,		TRUE,		TRUE},			
	[ERR_TMP_SHORT]         = {ERR_TMP_SHORT,			  TRUE,		TRUE,		TRUE},

	[ERR_MOTOR_SPD_OV]      = {ERR_MOTOR_SPD_OV,		  TRUE,		TRUE,		TRUE},	
	[ERR_MOTOR_BLOCK]       = {ERR_MOTOR_BLOCK,		    TRUE,		TRUE,		TRUE},
	[ERR_MOTOR_FLY]         = {ERR_MOTOR_FLY,			    TRUE,		TRUE,		TRUE},	
	[ERR_MOTOR_LACKPHASE_U] = {ERR_MOTOR_LACKPHASE_U,	TRUE,		TRUE,		TRUE},
	[ERR_MOTOR_LACKPHASE_V] = {ERR_MOTOR_LACKPHASE_V,	TRUE,		TRUE,		TRUE},	
	[ERR_MOTOR_LACKPHASE_W] = {ERR_MOTOR_LACKPHASE_W,	TRUE,		TRUE,		TRUE},
	[ERR_MOTOR_TMP_OV]      = {ERR_MOTOR_TMP_OV,		TRUE,		TRUE,		TRUE},	
	[ERR_MOTOR_TMP_DISCONT] = {ERR_MOTOR_TMP_DISCONT,	TRUE,		TRUE,		TRUE},
	[ERR_MOTOR_TMP_SHORT]   = {ERR_MOTOR_TMP_SHORT, 	TRUE,		TRUE,		TRUE},	
	[ERR_MOTOR_LOAD_OV]     = {ERR_MOTOR_LOAD_OV, 	    TRUE,		TRUE,		TRUE},
	[ERR_MOTOR_BRK_FAULT]   = {ERR_MOTOR_BRK_FAULT, 	TRUE,		TRUE,		TRUE},	

	[ERR_ENC_CRC_6813]      = {ERR_ENC_CRC_6813,		TRUE,		TRUE,		TRUE},
	[ERR_ENC_Z_SHIFT]       = {ERR_ENC_Z_SHIFT, 		TRUE,		TRUE,		TRUE},	
	[ERR_ENC_Z_DISTURB]     = {ERR_ENC_Z_DISTURB, 	    TRUE,		TRUE,		TRUE},
	[ERR_ENC_Z_LOSS]        = {ERR_ENC_Z_LOSS, 		    TRUE,		TRUE,		TRUE},

	[ERR_ENC_RES_OFFSET_SIN]= {ERR_ENC_RES_OFFSET_SIN,  TRUE,		TRUE,		TRUE},
	[ERR_ENC_RES_OFFSET_COS]= {ERR_ENC_RES_OFFSET_COS,  TRUE,		TRUE,		TRUE},	
	[ERR_ENC_RES_DISC_SIN_0]= {ERR_ENC_RES_DISC_SIN_0,  TRUE,		TRUE,		TRUE},
	[ERR_ENC_RES_DISC_COS_0]= {ERR_ENC_RES_DISC_COS_0,  TRUE,		TRUE,		TRUE},
	[ERR_ENC_RES_DISC_SIN_1]= {ERR_ENC_RES_DISC_SIN_1,  TRUE,		TRUE,		TRUE},
	[ERR_ENC_RES_DISC_COS_1]= {ERR_ENC_RES_DISC_COS_1,  TRUE,		TRUE,		TRUE},
	
	[ERR_CAN_COMM_LOSE]     = {ERR_CAN_COMM_LOSE,		TRUE,		TRUE,		TRUE},		
	[ERR_CAN_RX_FIFO_BUSY]  = {ERR_CAN_RX_FIFO_BUSY,	TRUE,		TRUE,		TRUE},		/* CAN���չ��� */
	[ERR_CAN_TX_FIFO_BUSY]  = {ERR_CAN_TX_FIFO_BUSY,	TRUE,		TRUE,		TRUE},		/* CAN���͹��� */
	[ERR_CAN_BUS_STOP]      = {ERR_CAN_BUS_STOP,		TRUE,		TRUE,		TRUE},		/* CANֹͣ���� */

  [ERR_HOME_TIME_OUT]     = {ERR_HOME_TIME_OUT,	    TRUE,		TRUE,		TRUE},

	[ERR_TIME_AD_ISR_OV]    = {ERR_TIME_AD_ISR_OV,	    TRUE,		TRUE,		TRUE},
	[ERR_TIME_MAIN05MS_OV]  = {ERR_TIME_MAIN05MS_OV,    TRUE,		TRUE,		TRUE},	


	[ERR_E2_WRITE]          = {ERR_E2_WRITE,			TRUE,		TRUE,		TRUE},
	[ERR_E2_READ]           = {ERR_E2_READ,			    TRUE,		TRUE,		TRUE},	
	[ERR_E2_RW_TIME_OV]     = {ERR_E2_RW_TIME_OV,		TRUE,		TRUE,		TRUE},
	[ERR_E2_WRITE_NUM_OV]   = {ERR_E2_WRITE_NUM_OV, 	TRUE,		TRUE,		TRUE},	
	
	
	[ERR_OUT_BOUNDARY]   		= {ERR_OUT_BOUNDARY, 	TRUE,		TRUE,		TRUE},		
	[ERR_STOP]   						= {ERR_STOP, 	TRUE,		TRUE,		TRUE},		
	[ERR_LIFT]  					  = {ERR_LIFT, 	TRUE,		TRUE,		TRUE},	
	[ERR_ROLL]   						= {ERR_ROLL, 	TRUE,		TRUE,		TRUE},	
	[ERROR_WALK_OVERLOAD]   = {ERROR_WALK_OVERLOAD, 	TRUE,		TRUE,		TRUE},	
	[ERROR_CUT_OVERLOAD]    = {ERROR_CUT_OVERLOAD, 	TRUE,		TRUE,		TRUE},	
  [ERROR_HARDWARE]        =   {ERROR_HARDWARE, 	TRUE,		TRUE,		TRUE},	
  [ERROR_OVERCHARGECURRENT] = {ERROR_OVERCHARGECURRENT, 	TRUE,		TRUE,		TRUE},		
	
	
};

FAULT_STR strFault[MOTOR_AXIS];
uint16_t u16FaultListLineNum = sizeof(strFaultList) / sizeof(FAULT_INFO_STR);

/* �������� */
void vFaultInfoRecord(uint16_t u16ErrCode);
static  const FAULT_INFO_STR* xFaultIndexSearch(uint16_t u16ErrCode);
void vFaultSaveToHistory(uint16_t u16ErrCode, uint8_t u8AxisNum);
void vFaultHandle(uint16_t u16ErrCode, uint8_t u8AxisNum);
void vFaultCurrentClear(uint8_t u8AxisNum);
void vFaultClearByNO(uint16_t u16ErrCode);
void vFaultParaInit(void);

/* ����ָ������ */
FAULT_FUN_STR strFaultFun =
{
	vFaultParaInit,
	vFaultHandle,
	vFaultCurrentClear,
  vFaultClearByNO
};

/************************************************* 
 Function:	  vFaultParaInit
 Description: ���ϲ�����ʼ��  
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultParaInit(void)
{
}

/************************************************* 
 Function:	  vFaultHandle
 Description: �����¼����� 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/


void vFaultHandle(uint16_t u16ErrCode, uint8_t u8AxisNum)
{
	uint8_t i;
	const FAULT_INFO_STR *pstrFault;
	uint16_t u16RankBuf[FAULT_DEPTH + 1];
	uint8_t u8NeedSave = 1;
	FAULT_STR *pstr;
	
	pstr = &strFault[u8AxisNum];

	/* ��������������Ϣ */
	pstrFault = xFaultIndexSearch(u16ErrCode);   

	/* ���ݴ���洢 */
	for(i = 0; i < FAULT_DEPTH; i++ )
  {
		u16RankBuf[i] = pstr->Out.u16FaultCurrentList[i];
	}	
	u16RankBuf[FAULT_DEPTH] = pstrFault->u16ErrCode;
    


    /* ֮ǰû���������� ��¼������Ϣ */
	if(pstr->Out.Flg.bFaultOccur == 0)
	{
		/* ��¼������Ϣ */
		vFaultInfoRecord(u16ErrCode);
	}

	pstr->Out.Flg.bFaultOccur = 1;
	
	/* ������� �ظ����������һ��*/
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(pstrFault->u16ErrCode == pstr->Out.u16FaultCurrentList[i])
		{
			u8NeedSave = 0;
			break;
		}

	}

	if(u8NeedSave)
	{
		if(u16RankBuf[0] == 0)
		{
			u16RankBuf[0] = u16RankBuf[FAULT_DEPTH];
		}
		else
		{
			/* �������� ���µ���Զ��Buf[0] */
			for(i = 0; i < FAULT_DEPTH -1; i ++)
			{
				u16RankBuf[FAULT_DEPTH - 1 -i] = u16RankBuf[FAULT_DEPTH - 2-i];
			}
			u16RankBuf[0] = u16RankBuf[FAULT_DEPTH];			
		}

		for(i = 0; i < FAULT_DEPTH; i++)
        {
			pstr->Out.u16FaultCurrentList[i] = (enum ERR_CODE)u16RankBuf[i]; /* ˢ�´���� */
		}
		
		/* ��Ҫ��ӵ���ʷ�����б� */
		if(pstrFault->u8IsAddHistory)
		{
			vFaultSaveToHistory(pstrFault->u16ErrCode, u8AxisNum);	
		}		
	}

    /* �������б� �ڸ��µ�ǰ���� ��Ϊ��Ҫ�õ��б���� */
    if(pstrFault == 0) /* û�в鵽���Ϻ� */
	{
		pstr->Out.CurrentFault.u16ErrCode = (enum ERR_CODE)u16ErrCode;
	}
    else
	{
		/* �������Ը�ֵ */
		pstr->Out.CurrentFault = *xFaultIndexSearch(pstr->Out.u16FaultCurrentList[0]);//*pstrFault;
	}
	/* ��ǰ�����б��ڴ��ڵĹ�����Ŀ */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(pstr->Out.u16FaultCurrentList[i] == 0)
			break;
	}
	pstr->Out.u8FaultAmountCur = i;

}

/************************************************* 
 Function:	  xFaultIndexSearch
 Description: ���Ϻ����� 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
static  const FAULT_INFO_STR* xFaultIndexSearch(uint16_t u16ErrCode)
{
//	int i;
//    FAULT_INFO_STR* pTemp;
//	pTemp = strFaultList;

	if(u16ErrCode == 0)
	{
		return 0;
	}
    
    /* ��ֹû��������ϱ��� */
    if(strFaultList[u16ErrCode].u16ErrCode == 0)
    {
        return 0;
    }
//	for(i = 0; i < u16FaultListLineNum; i++)
//	{
//		if((pTemp + i)->u16ErrCode == u16ErrCode)
//		{
//			break;
//		}
//		if(i == u16FaultListLineNum - 1 && (pTemp + i)->u16ErrCode != u16ErrCode)
//		{
//			return 0;
//		}
//	}
//	return (pTemp + i);
    
    return &strFaultList[u16ErrCode];
}

/************************************************* 
 Function:	  vFaultToE2
 Description: ����E2�洢 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultInfoToE2(void)
{
    uint8_t u8ErrRecordIndexTmp;
    uint16_t l_u16EndIndex;
    if(unFuncCode.Code.u16ErrRecordIndex == 0)
    {
        u8ErrRecordIndexTmp = 3;
    }
    else
    {
        u8ErrRecordIndexTmp = unFuncCode.Code.u16ErrRecordIndex - 1;
    }
    l_u16EndIndex = GetCodeIndex(unFuncCode.Code.ErrScene[u8ErrRecordIndexTmp].u16DoStatus);
    
    //l_u16EndIndex = GetCodeIndex(unFuncCode.Code.ErrScene[9].u16DoStatus);
//    strEepromFun.pvSetFuncCodeRwMode(FUNCCODE_RW_MODE_WRITE_SERIES);


//    strEepromFun.pvSetSaveFuncCodeIndex(GetCodeIndex(unFuncCode.Code.ErrScene[u8ErrRecordIndexTmp].u16ErrCode), l_u16EndIndex);

//	strEepromFun.pvSaveOneFuncCode(GetCodeIndex(unFuncCode.Code.u16ErrRecordIndex));
}

/************************************************* 
 Function:	  vFaultInfoRecord
 Description: �����ֳ���¼ 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultInfoRecord(uint16_t u16ErrCode)
{

    /* ֱ���ñ��¼ */
    ERROR_SCENE_STR *punFuncCodeErrSence;
	  uint16_t u8FaultRecordIndex = unFuncCode.Code.u16ErrRecordIndex;
	
    punFuncCodeErrSence = unFuncCode.Code.ErrScene;
	
    punFuncCodeErrSence[u8FaultRecordIndex].u16ErrCode = strFault[0].Out.CurrentFault.u16ErrCode;
//    punFuncCodeErrSence[u8FaultRecordIndex].u16TimeL = strTime.u32HourX01;
//    punFuncCodeErrSence[u8FaultRecordIndex].u16TimeH = strTime.u32HourX01 >> 16;
    punFuncCodeErrSence[u8FaultRecordIndex].s16SpdRefL = 0;//*strMotorFoc[0].Asr.SpdPdff.In.ps32Ref;
    punFuncCodeErrSence[u8FaultRecordIndex].s16SpdRefH = 0;//*strMotorFoc[0].Asr.SpdPdff.In.ps32Ref >> 16;
    punFuncCodeErrSence[u8FaultRecordIndex].s16SpdFbkL = 0;//*strMotorFoc[0].Asr.SpdPdff.In.ps32Fbk;
    punFuncCodeErrSence[u8FaultRecordIndex].s16SpdFbkH = 0;//*strMotorFoc[0].Asr.SpdPdff.In.ps32Fbk >> 16;
    
    punFuncCodeErrSence[u8FaultRecordIndex].s16TrqRefL = 0;//*strMotorFoc[0].Acr.IqPi.In.ps32Ref;
    punFuncCodeErrSence[u8FaultRecordIndex].s16TrqRefH = 0;//*strMotorFoc[0].Acr.IqPi.In.ps32Ref >> 16;
    punFuncCodeErrSence[u8FaultRecordIndex].s16TrqFbkL = 0;//*strMotorFoc[0].Acr.IqPi.In.ps32Fbk;
    punFuncCodeErrSence[u8FaultRecordIndex].s16TrqFbkH = 0;//*strMotorFoc[0].Acr.IqPi.In.ps32Fbk >> 16;
    
    punFuncCodeErrSence[u8FaultRecordIndex].s16DriverTemp = 0;//strMotorFoc[0].SampleInfo.Temp.Out.s16DriverTemp;
    
    punFuncCodeErrSence[u8FaultRecordIndex].u16DcBusVol = 0;//strMotorFoc[0].SampleInfo.Vol.Out.u16UdcX10;

    punFuncCodeErrSence[u8FaultRecordIndex].u16DiStatus = 0;
    punFuncCodeErrSence[u8FaultRecordIndex].u16DoStatus = 0;

	unFuncCode.Code.u16ErrRecordIndex = (unFuncCode.Code.u16ErrRecordIndex + 1) & (ERROR_SCENE_DEPTH - 1);

	/* �浽e2 */
	vFaultInfoToE2();
	
}


/************************************************* 
 Function:	  vFaultSaveToHistory
 Description: ������ʷ�����¼ 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultSaveToHistory(uint16_t u16ErrCode, uint8_t u8AxisNum)
{
	int i;
	uint16_t* p16Temp;
	
	for(i = FAULT_RCD_NUM-1; i > 0; i--)
	{
		u16FaultHistoryList[u8AxisNum][i] = u16FaultHistoryList[u8AxisNum][i-1];
	}
	u16FaultHistoryList[u8AxisNum][0] = u16ErrCode;	
	

}

/************************************************* 
 Function:	  vFaultCurrentClear
 Description: �����ǰ����
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultCurrentClear(uint8_t u8AxisNum)
{
	uint8_t i;
  uint8_t j;
	uint8_t u8FaultOccFlgCnt = 0;	
	uint16_t u16TempFault;
	FAULT_INFO_STR *pstrCurrentFault;
	FAULT_STR *pstrFault;
	const FAULT_INFO_STR *pstrFaultList;
		
	pstrFault = &strFault[u8AxisNum];
	pstrCurrentFault = &strFault[u8AxisNum].Out.CurrentFault;
	
	/* �жϵ�ǰ�����Ƿ����� */
	if(pstrCurrentFault->u8IsClearable)
	{
		memset(pstrCurrentFault, 0, sizeof(FAULT_INFO_STR));
	}

	/* �����ǰ�Ķ������ */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		/* ��ǰ�д����� */
		if(pstrFault->Out.u16FaultCurrentList[i] != 0)
		{
			/* ��ô��������Ϣ�� */
			pstrFaultList = xFaultIndexSearch(pstrFault->Out.u16FaultCurrentList[i]);

			/* �жϿɷ�������´����б� */
			if(pstrFaultList->u8IsClearable)
			{
				pstrFault->Out.u16FaultCurrentList[i] = (enum ERR_CODE)0;
			}
			else
			{
				u8FaultOccFlgCnt += 1;
			}
		}
	}

	/* ���ŵ�ǰ�����б� */
	for(i = 0; i <FAULT_DEPTH; i++)
	{
		if(pstrFault->Out.u16FaultCurrentList[i] == 0)
		{			
			for(j = i; j < FAULT_DEPTH - 1; j ++)
			{
				u16TempFault = pstrFault->Out.u16FaultCurrentList[j];
				pstrFault->Out.u16FaultCurrentList[j] = pstrFault->Out.u16FaultCurrentList[j + 1];
				pstrFault->Out.u16FaultCurrentList[j + 1] =  (enum ERR_CODE)u16TempFault;
			}
		}
	}

	/* ���µ�ǰ�����б��ڴ��ڵĹ�����Ŀ */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(pstrFault->Out.u16FaultCurrentList[i] == 0)
			break;
	}
	pstrFault->Out.u8FaultAmountCur = i;

	/* ���¹��ϱ�־λ */
	if(u8FaultOccFlgCnt > 0)
	{
		pstrFault->Out.Flg.bFaultOccur = 1;
	}
	else
	{
		pstrFault->Out.Flg.bFaultOccur = 0;
	}
}

/************************************************* 
 Function:	  vFaultCurrentClear
 Description: �����ʷ����
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultHistoryClear(uint8_t u8AxisNum)
{
	uint8_t i;
	/* �����ǰ���ϼ�����ϱ� */
	vFaultCurrentClear(u8AxisNum);

	/* ����ʷ���� */
	for(i = 0; i < FAULT_RCD_NUM; i++)
	{
		u16FaultHistoryList[u8AxisNum][i] = 0;
	}
}

/************************************************* 
 Function:	  vFaultClearByNO
 Description: ���ָ������
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultClearByNO(uint16_t u16ErrCode)
{
	uint8_t i,j;
	uint8_t cnt1 = 0,cnt2 = 0;
	 const FAULT_INFO_STR* pFaultMsg;
	
	/* �����ǰ�Ķ������ */
	for(i = 0; i <FAULT_DEPTH; i++)
	{	
		if(strFault[0].Out.u16FaultCurrentList[i] == u16ErrCode)
		{
			/* ��� */
			strFault[0].Out.u16FaultCurrentList[i] = (enum ERR_CODE)0;
			/* �ƶ����� */
			for(j = i; j < FAULT_DEPTH - 1; j++)
			{
				strFault[0].Out.u16FaultCurrentList[j] = strFault[0].Out.u16FaultCurrentList[j + 1];
			}
			
			/* �������� */
			strFault[0].Out.u8FaultAmountCur -= 1;
		}
	}
	if(strFault[0].Out.CurrentFault.u16ErrCode == u16ErrCode)
	{
		strFault[0].Out.CurrentFault.u16ErrCode = ERR_NO_CODE;
		strFault[0].Out.CurrentFault.u8IsAddHistory = 0;
		strFault[0].Out.CurrentFault.u8IsClearable = 0;
		strFault[0].Out.CurrentFault.u8IsImmediateStop = 0;
	}	
	/* ������ϱ�־λ�͸߼�����Ϸ�����־λ */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(strFault[0].Out.u16FaultCurrentList[i] != 0)
		{
			cnt1++;
		}
        else    /* ������û�д��� */
        {
            break;
        }
		
		pFaultMsg = xFaultIndexSearch(strFault[0].Out.u16FaultCurrentList[i]);
        if(i == 0)
        {
            strFault[0].Out.CurrentFault = *pFaultMsg;
        }
		if(pFaultMsg->u8IsImmediateStop == FALSE)
		{
			cnt2 += 1; 
		}		
	}
    

	if(cnt1 != 0)
	{
		strFault[0].Out.Flg.bFaultOccur = 1;
	}
	else
	{
		strFault[0].Out.Flg.bFaultOccur = 0;
	}
	
}
