/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: Fault.c
 Author:     Version :   Date: 
 Sgk         1.0         2021年8月10日
 Description: 故障管理
 Version: // 版本信息
 History: 
 <author>    <time>          <version >   <desc> 
 Sgk         2021年8月10日       1.0          初版
***********************************************************/

#include "Fault.h"
//#include "TypeDef.h"
//#include "f_Eeprom.h"  /*屏蔽了EEROM的软件*/
#include "f_FuncCode.h"
#include "Control&Filter.h"
#include "stdint.h"
#include "string.h"

/* 变量声明 */		
ERROR_SCENE_STR s16FaultRecordList[ERROR_SCENE_DEPTH];	/* 错误时保存的数组 */

uint16_t u16FaultHistoryList[MOTOR_AXIS][FAULT_RCD_NUM];	/* 历史故障 */

const FAULT_INFO_STR strFaultList[255] =			/* 故障查询表 */
{

	/*   索引号                错误编号 	  	    	履历	   可清除	    立刻停止      	报警名称 */
	[ERR_OPA_OFFSET_A]      = {ERR_OPA_OFFSET_A,		FALSE,		FALSE,		TRUE},		/* A相运放偏置电压错误 */	
	[ERR_OPA_OFFSET_B]      = {ERR_OPA_OFFSET_B,		FALSE,		FALSE,		TRUE},		/* B相运放偏置电压错误 */	
	[ERR_OPA_OFFSET_C]      = {ERR_OPA_OFFSET_C,		FALSE,		FALSE,		TRUE},		/* C相运放偏置电压错误 */
	[ERR_OC_FAULT]          = {ERR_OC_FAULT,			  TRUE,		  FALSE,		TRUE},		    /* 硬件过流 */
	[ERR_VOL_UD]            = {ERR_VOL_UD, 			    FALSE,  	TRUE,  		TRUE},		/* 母线欠压保护 */	
	[ERR_VOL_OV]            = {ERR_VOL_OV, 			    TRUE,  		TRUE,  		TRUE},		/* 母线过压保护 */
	
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
	[ERR_CAN_RX_FIFO_BUSY]  = {ERR_CAN_RX_FIFO_BUSY,	TRUE,		TRUE,		TRUE},		/* CAN接收过载 */
	[ERR_CAN_TX_FIFO_BUSY]  = {ERR_CAN_TX_FIFO_BUSY,	TRUE,		TRUE,		TRUE},		/* CAN发送过载 */
	[ERR_CAN_BUS_STOP]      = {ERR_CAN_BUS_STOP,		TRUE,		TRUE,		TRUE},		/* CAN停止工作 */

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

/* 函数声明 */
void vFaultInfoRecord(uint16_t u16ErrCode);
static  const FAULT_INFO_STR* xFaultIndexSearch(uint16_t u16ErrCode);
void vFaultSaveToHistory(uint16_t u16ErrCode, uint8_t u8AxisNum);
void vFaultHandle(uint16_t u16ErrCode, uint8_t u8AxisNum);
void vFaultCurrentClear(uint8_t u8AxisNum);
void vFaultClearByNO(uint16_t u16ErrCode);
void vFaultParaInit(void);

/* 函数指针声明 */
FAULT_FUN_STR strFaultFun =
{
	vFaultParaInit,
	vFaultHandle,
	vFaultCurrentClear,
  vFaultClearByNO
};

/************************************************* 
 Function:	  vFaultParaInit
 Description: 故障参数初始化  
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
 Description: 故障事件处理 
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

	/* 搜索故障属性信息 */
	pstrFault = xFaultIndexSearch(u16ErrCode);   

	/* 备份错误存储 */
	for(i = 0; i < FAULT_DEPTH; i++ )
  {
		u16RankBuf[i] = pstr->Out.u16FaultCurrentList[i];
	}	
	u16RankBuf[FAULT_DEPTH] = pstrFault->u16ErrCode;
    


    /* 之前没发生过故障 记录故障信息 */
	if(pstr->Out.Flg.bFaultOccur == 0)
	{
		/* 记录故障信息 */
		vFaultInfoRecord(u16ErrCode);
	}

	pstr->Out.Flg.bFaultOccur = 1;
	
	/* 保存错误 重复错误仅保存一次*/
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
			/* 错误排序 最新的永远在Buf[0] */
			for(i = 0; i < FAULT_DEPTH -1; i ++)
			{
				u16RankBuf[FAULT_DEPTH - 1 -i] = u16RankBuf[FAULT_DEPTH - 2-i];
			}
			u16RankBuf[0] = u16RankBuf[FAULT_DEPTH];			
		}

		for(i = 0; i < FAULT_DEPTH; i++)
        {
			pstr->Out.u16FaultCurrentList[i] = (enum ERR_CODE)u16RankBuf[i]; /* 刷新错误表 */
		}
		
		/* 需要添加到历史故障列表 */
		if(pstrFault->u8IsAddHistory)
		{
			vFaultSaveToHistory(pstrFault->u16ErrCode, u8AxisNum);	
		}		
	}

    /* 更新完列表 在更新当前错误 因为需要用到列表错误 */
    if(pstrFault == 0) /* 没有查到故障号 */
	{
		pstr->Out.CurrentFault.u16ErrCode = (enum ERR_CODE)u16ErrCode;
	}
    else
	{
		/* 故障属性赋值 */
		pstr->Out.CurrentFault = *xFaultIndexSearch(pstr->Out.u16FaultCurrentList[0]);//*pstrFault;
	}
	/* 当前故障列表内存在的故障数目 */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(pstr->Out.u16FaultCurrentList[i] == 0)
			break;
	}
	pstr->Out.u8FaultAmountCur = i;

}

/************************************************* 
 Function:	  xFaultIndexSearch
 Description: 故障号搜索 
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
    
    /* 防止没有纳入故障表中 */
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
 Description: 故障E2存储 
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
 Description: 故障现场记录 
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultInfoRecord(uint16_t u16ErrCode)
{

    /* 直接用表记录 */
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

	/* 存到e2 */
	vFaultInfoToE2();
	
}


/************************************************* 
 Function:	  vFaultSaveToHistory
 Description: 故障历史代码记录 
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
 Description: 清除当前故障
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
	
	/* 判断当前故障是否可清除 */
	if(pstrCurrentFault->u8IsClearable)
	{
		memset(pstrCurrentFault, 0, sizeof(FAULT_INFO_STR));
	}

	/* 清除当前的多个故障 */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		/* 当前有错误码 */
		if(pstrFault->Out.u16FaultCurrentList[i] != 0)
		{
			/* 获得错误码的信息表 */
			pstrFaultList = xFaultIndexSearch(pstrFault->Out.u16FaultCurrentList[i]);

			/* 判断可否清除更新错误列表 */
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

	/* 重排当前故障列表 */
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

	/* 更新当前故障列表内存在的故障数目 */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(pstrFault->Out.u16FaultCurrentList[i] == 0)
			break;
	}
	pstrFault->Out.u8FaultAmountCur = i;

	/* 更新故障标志位 */
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
 Description: 清除历史故障
 Input: 	 
 Output:	  
 Return: 
 Others: 
*************************************************/
void vFaultHistoryClear(uint8_t u8AxisNum)
{
	uint8_t i;
	/* 清除当前故障及多故障表 */
	vFaultCurrentClear(u8AxisNum);

	/* 清历史故障 */
	for(i = 0; i < FAULT_RCD_NUM; i++)
	{
		u16FaultHistoryList[u8AxisNum][i] = 0;
	}
}

/************************************************* 
 Function:	  vFaultClearByNO
 Description: 清除指定故障
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
	
	/* 清除当前的多个故障 */
	for(i = 0; i <FAULT_DEPTH; i++)
	{	
		if(strFault[0].Out.u16FaultCurrentList[i] == u16ErrCode)
		{
			/* 清除 */
			strFault[0].Out.u16FaultCurrentList[i] = (enum ERR_CODE)0;
			/* 移动数组 */
			for(j = i; j < FAULT_DEPTH - 1; j++)
			{
				strFault[0].Out.u16FaultCurrentList[j] = strFault[0].Out.u16FaultCurrentList[j + 1];
			}
			
			/* 更新数量 */
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
	/* 处理故障标志位和高级别故障发生标志位 */
	for(i = 0; i < FAULT_DEPTH; i++)
	{
		if(strFault[0].Out.u16FaultCurrentList[i] != 0)
		{
			cnt1++;
		}
        else    /* 接下来没有错误 */
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
