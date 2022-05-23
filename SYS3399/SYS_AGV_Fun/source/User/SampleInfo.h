#ifndef __SAMPLE_INFO_H_
#define __SAMPLE_INFO_H_
#include "stdint.h"
#include "MowerFSM.h"



typedef struct
{
	struct
	{
		int16_t s16UdcAd;
		int32_t s32AdToUdcCoeff;
	}In;

	struct
	{
		int32_t s32UdcPU;
		int16_t s16UdcQ12PU;
		int32_t s32UdcFltPU;
		uint16_t u16UdcX10;
		uint16_t u16UdcPercentage;		
	}Out;
}SAMPLE_BATTERYVOL_INFO_STR;


typedef struct
{
	struct
	{
		int16_t s16UChargeAd;
		int32_t s32AdToUdcCoeff;
	}In;

	struct
	{
		int32_t s32UChargePU;
		int16_t s16UChargeQ12PU;
		int32_t s32UChargeFltPU;
		uint16_t u16UChargeX10;
	}Out;
}SAMPLE_CHARGEVOL_INFO_STR;



	typedef struct
{
	struct
	{
		int16_t s16IChargeAd;
		int32_t *ps32IChargeOffsetAd;
	}In;

	struct
	{
	 int32_t	s32IChargePU;
	 int16_t  s16IChargeQ12PU;		
	 int32_t	s32IChargeFltPU;
	 int16_t	s16IChargeX10;
	}Out;

}SAMPLE_CHARGECUR_INFO_STR;


typedef struct
{
	struct
	{
		int16_t s16RainAd;
	}In;

	struct
	{
	 int16_t	s16RainFlt;
	}Out;
	
}SAMPLE_RAIN_INFO_STR;


typedef struct
{
	struct
	{
		int16_t s16StopAd;
	}In;

	struct
	{
		int16_t s16StopFlt;
	}Out;
	
}SAMPLE_STOP_INFO_STR;



typedef struct
{
  uint8_t val;
  uint8_t cnt;

}tIOfliter;

typedef struct
{
  tIOfliter S0; /***启动开关****/
  uint8_t start_handler_released_before;
	uint8_t start_handler_status;
	uint8_t onoff_handler_status_out;
	
	
	tIOfliter S1;	/***刹车开关****/ 	
	uint8_t stop_handler_status_out;

	tIOfliter S2;	/***刹车开关****/ 	
	uint8_t LeftLiftStatus;	
	
	tIOfliter S3;	/***刹车开关****/ 	
	uint8_t RightLiftStatus;		
	
}StartStopSwitch_TypeDef;


typedef  struct
{
	
	SAMPLE_BATTERYVOL_INFO_STR BatteryVol;	
	SAMPLE_CHARGECUR_INFO_STR  ChargeCurr;
	SAMPLE_CHARGEVOL_INFO_STR  ChargeVol;
	SAMPLE_RAIN_INFO_STR 			 Rain;
	SAMPLE_STOP_INFO_STR 			 Stop1;	
	SAMPLE_STOP_INFO_STR 			 Stop2;		
	StartStopSwitch_TypeDef    Switch;
	
}SAMPLE_INFO_STR;


typedef struct 
{	
	void (*pvSampleParaInit)(SAMPLE_INFO_STR *pstrSample,SELF_CHECK_STR *pstrSelfChek);
	void (*pvGetSampleInfo)(SAMPLE_INFO_STR *pstr);
}SAMPLE_INFO_FUN_STR;


extern SAMPLE_INFO_STR strSampleInfo;
extern SAMPLE_INFO_FUN_STR strSampleInfoFun;


extern uint8_t IO_FLITER(tIOfliter * ios, int8_t ioval, uint16_t cpval);

#endif


