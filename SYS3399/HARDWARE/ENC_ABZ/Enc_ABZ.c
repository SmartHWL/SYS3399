/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: Enc_ABZ.c
 Author:     Version :   Date: 
 Sgk         1.0         2021年8月10日
 Description: ABZ编码器解析
 Version: // 版本信息
 History: 
 <author>    <time>          <version >   <desc> 
 Sgk         2021年8月10日       1.0          初版
***********************************************************/

#include "Enc_ABZ.h"
#include "IQmathLib.h"
#include "BSP_gpio.h"
//#include "TypeDef.h"
//#include "Fault.h"
#include "BSP_qep.h"
//#include "SystemPU.h"
//#include "CpuTime.h"

/* 变量声明 */
ENC_ABZ_STR strEncABZ;

/* 函数声明 */
void  vEncZRevise(ENC_ABZ_STR *pstrABZ);
void vEncZErr(ENC_ABZ_STR *pstrABZ,  int8_t *s8ZLossCircleCnt, int32_t s32SpdFbkFlt);
void vEncZDisturb(ENC_ABZ_STR *pstrABZ, int32_t s32SpdFbkFlt);

/* 函数指针声明 */
ENC_ABZ_FUN_STR strEncABZFun =
{
	&vEncZRevise,
	&vEncZErr
};
#if 0
/************************************************* 
 Function:	  vEncABZInit
 Description: ABZ参数初始化
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncABZInit(ENC_ABZ_STR *pstrABZ)
{
	/* 带UVW编码器处理 */
    #if ENC_TYPE == ENC_ABZ
	pstrABZ->In.uABZStatus.Bit.bUVW = (GET_HALL_U_STATUS() << 2) | (GET_HALL_V_STATUS() << 1) | GET_HALL_W_STATUS();
    pstrABZ->Out.ps32ElcThetaComp = +1610612;//1476395;//2584725;//8200000;//6873019;//993211;//2892392;//1930000;//1050000;	    
    if((pstrABZ->In.uABZStatus.Bit.bUVW > 6 )||(pstrABZ->In.uABZStatus.Bit.bUVW < 1 ))
    {
    if (DEBUG_MOTOR_TEST ==0)
        {
            strFaultFun.pvFaultHandle(ERR_ENC_CRC_UVW, 0);		/* 编码器UVW错误 */
        }
    }
	#endif
    switch(pstrABZ->In.uABZStatus.Bit.bUVW)
	{
        #if USER_CONFIG_SEL == USER_CONFIG_HC_CH_02 /* 之前相序反了 */
		/* MT6825 HALL表 CW 6825UVW口与DIDO复用 逻辑为反 */
		case 5:
			pstrABZ->Out.s32UVWAngle = _IQ(210.0/360.0);
			break;
		case 4:
			pstrABZ->Out.s32UVWAngle = _IQ(270.0/360.0);
			break;
		case 6:
			pstrABZ->Out.s32UVWAngle = _IQ(330.0/360.0);
			break;
		case 2:
			pstrABZ->Out.s32UVWAngle = _IQ(30.0/360.0);
			break;
		case 3:
			pstrABZ->Out.s32UVWAngle = _IQ(90.0/360.0);
			break;
		case 1:
			pstrABZ->Out.s32UVWAngle = _IQ(150.0/360.0);
			break;
		#elif USER_CONFIG_SEL == USER_CONFIG_TUODA_AGV_02
            case 5:
                pstrABZ->Out.s32UVWAngle = _IQ(0.0/360.0);//_IQ(60.0/360.0);//_IQ(150.0/360.0);//
                break;
            case 4:
                pstrABZ->Out.s32UVWAngle = _IQ(300.0/360.0);//_IQ(120.0/360.0);//_IQ(90.0/360.0);
                break;
            case 6:
                pstrABZ->Out.s32UVWAngle = _IQ(240.0/360.0);//_IQ(180.0/360.0);//_IQ(30.0/360.0);
                break;
            case 2:
                pstrABZ->Out.s32UVWAngle = _IQ(180.0/360.0);//_IQ(240.0/360.0);//_IQ(330.0/360.0);
                break;
            case 3:
                pstrABZ->Out.s32UVWAngle = _IQ(120.0/360.0);//_IQ(300.0/360.0);//_IQ(270.0/360.0);
                break;
            case 1:
                pstrABZ->Out.s32UVWAngle = _IQ(60.0/360.0);//_IQ(0.0/360.0);//_IQ(210.0/360.0);
                break;
        #else
		/* MT6825 HALL表 CW 6825UVW口与DIDO复用 逻辑为反 */
        case 5:
			pstrABZ->Out.s32UVWAngle = _IQ(150.0/360.0);  // _IQ(0.0/360.0)拓达;//_IQ(60.0/360.0);//_IQ(150.0/360.0);//
			break;
		case 4:
			pstrABZ->Out.s32UVWAngle = _IQ(90.0/360.0);// _IQ(300.0/360.0)拓达;//_IQ(120.0/360.0);//_IQ(90.0/360.0);
			break;
		case 6:
			pstrABZ->Out.s32UVWAngle = _IQ(30.0/360.0);//_IQ(240.0/360.0)拓达;//_IQ(180.0/360.0);//_IQ(30.0/360.0);
			break;
		case 2:
			pstrABZ->Out.s32UVWAngle = _IQ(330.0/360.0);//_IQ(180.0/360.0)拓达;//_IQ(240.0/360.0);//_IQ(330.0/360.0);
			break;
		case 3:
			pstrABZ->Out.s32UVWAngle = _IQ(270.0/360.0);//_IQ(120.0/360.0)拓达;//_IQ(300.0/360.0);//_IQ(270.0/360.0);
			break;
		case 1:
			pstrABZ->Out.s32UVWAngle = _IQ(210.0/360.0);//_IQ(60.0/360.0)拓达;//_IQ(0.0/360.0);//_IQ(210.0/360.0);
			break;
		#endif
		default:
			break;
	}
    

}
#endif
/************************************************* 
 Function:	  vEncZRevise
 Description: 根据Z相对电角度的校正, 中断需要尽量简短
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
//#include "MotorDebug.h"
uint32_t offset;
uint32_t LookCnt;
FAKE_STR strFake;
void  vEncZRevise(ENC_ABZ_STR *pstrABZ)
{    
	if(pstrABZ->Out.u8ZCntForDisturb < 0xff)				/* 用于Z相干扰判断标志 */
	{
		pstrABZ->Out.u8ZCntForDisturb++;
	}
	offset = strFake.IdLock.s32ElcTheta;
	pstrABZ->In.uABZStatus.Bit.bZForLoss = 1;					/* 用于Z相丢失判断标志 */
	
	pstrABZ->Out.s16QepCntBak = pstrABZ->Out.s16QepCntOffset;
	pstrABZ->Out.s16QepCntOffset = BSP_GetQepCnt(0);
    LookCnt = BSP_GetQepCnt(0);
    
	if(pstrABZ->Para.u8EnterTime == 0)
	{     
        /* 第一次要备份 */
		pstrABZ->Out.s16QepCntBak = pstrABZ->Out.s16QepCntOffset;		
		pstrABZ->Out.s32UVWAngle = 0;
//        pstrABZ->Out.ps32ElcThetaComp = 0;
		pstrABZ->Para.u8EnterTime = 1;	/* 第一次进入 */
	}
	else if(pstrABZ->Para.u8EnterTime == 1)
	{
		pstrABZ->Para.u8EnterTime = 2; /* 第二次进入 */
	}
	
}

/************************************************* 
 Function:	  vEncZShifErr
 Description: Z相偏移错误判断
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncZErr(ENC_ABZ_STR *pstrABZ, int8_t *s8ZLossCircleCnt, int32_t s32SpdFbkFlt)
{
	#if 0
	static uint8_t u8Time = 0;
	
	/* Z相偏移故障  	   偏移6°电角度认为故障          */
	if((abs(pstrABZ->Out.s16QepCntOffset - pstrABZ->Out.s16QepCntBak) > (6.0 * ENC_PPR / 360.0 / POLES_PAIR))
        && (pstrABZ->Para.u8EnterTime > 1)) /* 第二次以后才开始比较 因为第一次值是未知的 */
	{
        strFaultFun.pvFaultHandle(ERR_ENC_Z_SHIFT, 0);
	}


	/* Z相丢失 4圈对Z信号做一个判断 */
	if(abs(*s8ZLossCircleCnt) > 3) 
	{
		if(pstrABZ->In.uABZStatus.Bit.bZForLoss == 0)
		{
            #if DEBUG_MODE
            strFaultFun.pvFaultHandle(ERR_ENC_Z_LOSS, 0);
            #endif
		}
		else 
		{
			*s8ZLossCircleCnt = 0;		/* 注意这边清零不能用形参 */
			pstrABZ->In.uABZStatus.Bit.bZForLoss = 0;
		}
	}

	/* Z干扰判断 */
	vEncZDisturb(pstrABZ, s32SpdFbkFlt);
#endif
}
#if 0
/************************************************* 
 Function:	  vEncZDisturb
 Description: Z相干扰判断
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncZDisturb(ENC_ABZ_STR *pstrABZ, int32_t s32SpdFbkFlt)
{
	/* 换向可能会接收到大于1个Z信号 不做判断处理 */
	if((s32SpdFbkFlt ^ pstrABZ->Para.s32SpdFbkBak) < 0 )
	{
		pstrABZ->Out.u8ZCntForDisturb = 0;
	}
	/* 不做初始化处理 少判断没关系 */
	pstrABZ->Para.s32SpdFbkBak = s32SpdFbkFlt;

	
	/* Z干扰滤波时间 */
	#define ENC_ABZ_Z_DISTURB_FLT_CNT	50	/* 5ms  12000rpm */
	
	if(pstrABZ->Para.u8DisturbFltCnt < ENC_ABZ_Z_DISTURB_FLT_CNT)
	{
        strEncABZ.Para.u8DisturbFltCnt ++;						 /* ABZ干扰滤波计时 */

		if(pstrABZ->Out.u8ZCntForDisturb > 2)		/* 单位时间内 Z信号 超过1个 认为Z受到干扰 */
		{
			strFaultFun.pvFaultHandle(ERR_ENC_Z_DISTURB, 0);
		}
	}
	else
	{
		pstrABZ->Para.u8DisturbFltCnt = 0;
		pstrABZ->Out.u8ZCntForDisturb = 0;
	}
}
#endif
