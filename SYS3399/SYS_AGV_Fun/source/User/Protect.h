#ifndef __PROTECT_H_
#define __PROTECT_H_
#include "stdint.h"

//#include "Acr.h"
//#include "Asr.h"
//#include "Encoder.h"
#include "SampleInfo.h"
#include "MowerFSM.h"


typedef struct
{
	struct
	{
		uint16_t *pu16UdcX10;
    enum MOWER_CTRL_FSM *pu8FSM;
	}In;

	struct
	{
		uint16_t u16UdTime;		
		uint16_t u16OvTime;
	}Para;
}UDC_STR;



typedef struct
{
	struct
	{
		int16_t *ps16StopADC1;
		int16_t *ps16StopADC2;		
    enum MOWER_CTRL_FSM *pu8FSM;
	}In;

	struct
	{
		uint16_t u16StopCnt;		
	}Para;
}STOP_STR;


typedef struct
{
	struct
	{
		bool pbLiftLeft;
		bool pbLiftRight;		
	}In;

	struct
	{
		uint16_t u16LiftCnt;		
	}Para;
	
}LIFT_STR;

typedef struct
{
	struct
	{
		int16_t *ps16RollAngle;
	}In;

	struct
	{
		uint16_t u16RollCnt;		
	}Para;
}ROLL_STR;


typedef struct
{
	struct
	{
		uint8_t *pu8ErrorCodeLeft;
		uint8_t *pu8ErrorCodeCut;		
		uint8_t *pu8ErrorCodeRight;	
	}In;

	struct
	{
		uint16_t u16WalkOverLoadCnt;		
		uint16_t u16CutOverLoadCnt;					
	}Para;
	
}MOTOR_ERROR_STR;


typedef struct
{
	struct
	{
		int16_t *ps16ChargeCurrentX10;
	}In;

	struct
	{
		uint16_t u16OverChargeCurrentCnt;					
	}Para;
	
}CHARGECURRENT_STR;


typedef struct
{
	struct
	{
		enum MOWER_DETECTOR_FSM  *pu8eMowerPosition;	
		
		
		
	}In;

	struct
	{
		uint16_t u16OutOfBoundaryCnt;					
	}Para;
	
}OUTOFBOUNDARY_STR;




typedef struct
{
	struct
	{
		int16_t *ps16Temp;
	}In;
	struct
	{
		uint16_t u16OvTime;
		uint16_t u16DiscontCnt;
		uint16_t u16ShortCnt;
	}Para;
	
}DRV_TEMP_PRO_STR;


typedef struct
{
	UDC_STR BatteryVol;
	UDC_STR ChargeVol;	
	STOP_STR Stop;
	LIFT_STR Lift;	
	ROLL_STR Roll;	
	MOTOR_ERROR_STR MotorError;
	CHARGECURRENT_STR  ChargeCurrent;
	DRV_TEMP_PRO_STR DriverTmp;
  OUTOFBOUNDARY_STR Boundary;
	
}PROTECT_FAST_STR;

typedef struct
{
	DRV_TEMP_PRO_STR DriverTmp;
}PROTECT_5MS_STR;

typedef struct
{
	PROTECT_FAST_STR Fast;
	PROTECT_5MS_STR Ms5;
}PROTECT_STR;

typedef struct
{
	void (*pvProtectParaInit)(PROTECT_STR *pstr, SAMPLE_INFO_STR *pstrSample,MOWER_FSM_STR *pstrCtrlFSM);
	void (*pvProtectFast)(PROTECT_FAST_STR *pstr);	
	void (*pvProtect5Ms)(PROTECT_5MS_STR *pstr);
}PROTECT_FUN_STR;


extern PROTECT_STR Protect;				/* ±£»¤ */
extern PROTECT_FUN_STR strProtectFun;


#endif

