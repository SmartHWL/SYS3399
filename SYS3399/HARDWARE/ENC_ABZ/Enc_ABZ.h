#ifndef __ENC_ABZ_H_
#define __ENC_ABZ_H_
#include "stdint.h"

typedef union
{
	uint8_t All;
	struct
	{
		uint8_t bUVW				: 3;
		uint8_t bZForLoss			: 1;
	}Bit;
}ABZ_STATUS_STR;

typedef struct
{
	struct
	{
		ABZ_STATUS_STR uABZStatus;
	}In;

	struct
	{
		int32_t s32SpdFbkBak;
		uint8_t u8EnterTime;
		uint8_t u8DisturbFltCnt;
	}Para;
	
	struct
	{
		int32_t s32UVWAngle;
        int32_t ps32ElcThetaComp;
		int16_t s16QepCntOffset;
		int16_t s16QepCntBak;
		uint8_t u8ZCntForDisturb;
	}Out;
}ENC_ABZ_STR;

typedef struct
{
	void  (*pvEncZRevise)(ENC_ABZ_STR *pstrABZ);	
	void (*pvEncZErr)(ENC_ABZ_STR *pstrABZ,  int8_t *s8ZLossCircleCnt, int32_t s32SpdFbkFlt);
}ENC_ABZ_FUN_STR;

typedef struct
{
	struct
	{
		int32_t s32Vd;
		int32_t s32Vq;
		int32_t s32ElcTheta;
	}VF;

	struct
	{
		int32_t s32Id;
		int32_t s32Iq;
		int32_t s32ElcTheta;
	}IdLock;
    
    struct
    {   
        int32_t s32SpdRef;
    }SpdStep;
    
    struct
    {
		int32_t s32Id;
		int32_t s32Iq;
		int32_t s32ElcTheta;
        uint8_t u8Step;
        uint32_t u32TimeDelay;
        int32_t s32QepBak[3];
    }SearchZ;

	struct
	{
		int32_t s32Id;
		int32_t s32Iq;
		int32_t s32ElcTheta;
        uint8_t u8Step;
        uint16_t u16TimeDelay;
		
		int32_t s32SinMax;
		int32_t s32SinMin;
		int32_t s32CosMax;
		int32_t s32CosMin;
		uint8_t u8CircleCnt;

		uint16_t u16SinOffset;		
		uint16_t u16CosOffset;
		uint32_t u32SinGain;		
		uint32_t u32CosGain;
	}ResolveCalibrate;
}FAKE_STR;

extern void vEncABZInit(ENC_ABZ_STR *pstrABZ);
extern ENC_ABZ_STR strEncABZ;
extern ENC_ABZ_FUN_STR strEncABZFun;

#endif

