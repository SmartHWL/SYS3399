/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: Enc_ABZ.c
 Author:     Version :   Date: 
 Sgk         1.0         2021��8��10��
 Description: ABZ����������
 Version: // �汾��Ϣ
 History: 
 <author>    <time>          <version >   <desc> 
 Sgk         2021��8��10��       1.0          ����
***********************************************************/

#include "Enc_ABZ.h"
#include "IQmathLib.h"
#include "BSP_gpio.h"
//#include "TypeDef.h"
//#include "Fault.h"
#include "BSP_qep.h"
//#include "SystemPU.h"
//#include "CpuTime.h"

/* �������� */
ENC_ABZ_STR strEncABZ;

/* �������� */
void  vEncZRevise(ENC_ABZ_STR *pstrABZ);
void vEncZErr(ENC_ABZ_STR *pstrABZ,  int8_t *s8ZLossCircleCnt, int32_t s32SpdFbkFlt);
void vEncZDisturb(ENC_ABZ_STR *pstrABZ, int32_t s32SpdFbkFlt);

/* ����ָ������ */
ENC_ABZ_FUN_STR strEncABZFun =
{
	&vEncZRevise,
	&vEncZErr
};
#if 0
/************************************************* 
 Function:	  vEncABZInit
 Description: ABZ������ʼ��
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncABZInit(ENC_ABZ_STR *pstrABZ)
{
	/* ��UVW���������� */
    #if ENC_TYPE == ENC_ABZ
	pstrABZ->In.uABZStatus.Bit.bUVW = (GET_HALL_U_STATUS() << 2) | (GET_HALL_V_STATUS() << 1) | GET_HALL_W_STATUS();
    pstrABZ->Out.ps32ElcThetaComp = +1610612;//1476395;//2584725;//8200000;//6873019;//993211;//2892392;//1930000;//1050000;	    
    if((pstrABZ->In.uABZStatus.Bit.bUVW > 6 )||(pstrABZ->In.uABZStatus.Bit.bUVW < 1 ))
    {
    if (DEBUG_MOTOR_TEST ==0)
        {
            strFaultFun.pvFaultHandle(ERR_ENC_CRC_UVW, 0);		/* ������UVW���� */
        }
    }
	#endif
    switch(pstrABZ->In.uABZStatus.Bit.bUVW)
	{
        #if USER_CONFIG_SEL == USER_CONFIG_HC_CH_02 /* ֮ǰ������ */
		/* MT6825 HALL�� CW 6825UVW����DIDO���� �߼�Ϊ�� */
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
		/* MT6825 HALL�� CW 6825UVW����DIDO���� �߼�Ϊ�� */
        case 5:
			pstrABZ->Out.s32UVWAngle = _IQ(150.0/360.0);  // _IQ(0.0/360.0)�ش�;//_IQ(60.0/360.0);//_IQ(150.0/360.0);//
			break;
		case 4:
			pstrABZ->Out.s32UVWAngle = _IQ(90.0/360.0);// _IQ(300.0/360.0)�ش�;//_IQ(120.0/360.0);//_IQ(90.0/360.0);
			break;
		case 6:
			pstrABZ->Out.s32UVWAngle = _IQ(30.0/360.0);//_IQ(240.0/360.0)�ش�;//_IQ(180.0/360.0);//_IQ(30.0/360.0);
			break;
		case 2:
			pstrABZ->Out.s32UVWAngle = _IQ(330.0/360.0);//_IQ(180.0/360.0)�ش�;//_IQ(240.0/360.0);//_IQ(330.0/360.0);
			break;
		case 3:
			pstrABZ->Out.s32UVWAngle = _IQ(270.0/360.0);//_IQ(120.0/360.0)�ش�;//_IQ(300.0/360.0);//_IQ(270.0/360.0);
			break;
		case 1:
			pstrABZ->Out.s32UVWAngle = _IQ(210.0/360.0);//_IQ(60.0/360.0)�ش�;//_IQ(0.0/360.0);//_IQ(210.0/360.0);
			break;
		#endif
		default:
			break;
	}
    

}
#endif
/************************************************* 
 Function:	  vEncZRevise
 Description: ����Z��Ե�Ƕȵ�У��, �ж���Ҫ�������
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
	if(pstrABZ->Out.u8ZCntForDisturb < 0xff)				/* ����Z������жϱ�־ */
	{
		pstrABZ->Out.u8ZCntForDisturb++;
	}
	offset = strFake.IdLock.s32ElcTheta;
	pstrABZ->In.uABZStatus.Bit.bZForLoss = 1;					/* ����Z�ඪʧ�жϱ�־ */
	
	pstrABZ->Out.s16QepCntBak = pstrABZ->Out.s16QepCntOffset;
	pstrABZ->Out.s16QepCntOffset = BSP_GetQepCnt(0);
    LookCnt = BSP_GetQepCnt(0);
    
	if(pstrABZ->Para.u8EnterTime == 0)
	{     
        /* ��һ��Ҫ���� */
		pstrABZ->Out.s16QepCntBak = pstrABZ->Out.s16QepCntOffset;		
		pstrABZ->Out.s32UVWAngle = 0;
//        pstrABZ->Out.ps32ElcThetaComp = 0;
		pstrABZ->Para.u8EnterTime = 1;	/* ��һ�ν��� */
	}
	else if(pstrABZ->Para.u8EnterTime == 1)
	{
		pstrABZ->Para.u8EnterTime = 2; /* �ڶ��ν��� */
	}
	
}

/************************************************* 
 Function:	  vEncZShifErr
 Description: Z��ƫ�ƴ����ж�
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncZErr(ENC_ABZ_STR *pstrABZ, int8_t *s8ZLossCircleCnt, int32_t s32SpdFbkFlt)
{
	#if 0
	static uint8_t u8Time = 0;
	
	/* Z��ƫ�ƹ���  	   ƫ��6���Ƕ���Ϊ����          */
	if((abs(pstrABZ->Out.s16QepCntOffset - pstrABZ->Out.s16QepCntBak) > (6.0 * ENC_PPR / 360.0 / POLES_PAIR))
        && (pstrABZ->Para.u8EnterTime > 1)) /* �ڶ����Ժ�ſ�ʼ�Ƚ� ��Ϊ��һ��ֵ��δ֪�� */
	{
        strFaultFun.pvFaultHandle(ERR_ENC_Z_SHIFT, 0);
	}


	/* Z�ඪʧ 4Ȧ��Z�ź���һ���ж� */
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
			*s8ZLossCircleCnt = 0;		/* ע��������㲻�����β� */
			pstrABZ->In.uABZStatus.Bit.bZForLoss = 0;
		}
	}

	/* Z�����ж� */
	vEncZDisturb(pstrABZ, s32SpdFbkFlt);
#endif
}
#if 0
/************************************************* 
 Function:	  vEncZDisturb
 Description: Z������ж�
 Input: 	  
 Output:	  
 Return: 
 Others: 
*************************************************/
void vEncZDisturb(ENC_ABZ_STR *pstrABZ, int32_t s32SpdFbkFlt)
{
	/* ������ܻ���յ�����1��Z�ź� �����жϴ��� */
	if((s32SpdFbkFlt ^ pstrABZ->Para.s32SpdFbkBak) < 0 )
	{
		pstrABZ->Out.u8ZCntForDisturb = 0;
	}
	/* ������ʼ������ ���ж�û��ϵ */
	pstrABZ->Para.s32SpdFbkBak = s32SpdFbkFlt;

	
	/* Z�����˲�ʱ�� */
	#define ENC_ABZ_Z_DISTURB_FLT_CNT	50	/* 5ms  12000rpm */
	
	if(pstrABZ->Para.u8DisturbFltCnt < ENC_ABZ_Z_DISTURB_FLT_CNT)
	{
        strEncABZ.Para.u8DisturbFltCnt ++;						 /* ABZ�����˲���ʱ */

		if(pstrABZ->Out.u8ZCntForDisturb > 2)		/* ��λʱ���� Z�ź� ����1�� ��ΪZ�ܵ����� */
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
