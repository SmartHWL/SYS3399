#ifndef __MOWER_FSM_H_
#define __MOWER_FSM_H_

#include "stdint.h"
#include "string.h"
#include "Boundary.h"
#include "Control&Filter.h"
#include "stdbool.h"

enum MOWER_CTRL_FSM{
	STATUS_LOW_POWER=0,
	STATUS_CHECK_CHARGECURR_OFFSET,
	STATUS_CHECK_BOUNDARY,
	STATUS_STOP,
	STATUS_STOPTOMOWING,	
	STATUS_MOWING,
	STATUS_CHARGE,
};


enum WALK_MOTOR_FSM{
	STOP=0,
	WALK,
	TURN,
	HOME,
};

enum MOWER_STATUS_STR{
	DISPLAYNONE=0,/***ͣ��**/
	READYCUTTING,/***׼����ݣ�С�ĵ���**/
	CUTTING,     /***���ڸ��**/
	TRAPRECOVER, /***�����ϰ���**/
	BACKTOHOME,	 /***�������**/
	CHARGING,  	 /***���ڳ��**/
};



union SELF_CHECK_FLG
{
	uint16_t All;
	struct
	{
		uint16_t bVolOk : 1;
		uint16_t bCurOffsetOk : 1;
    uint16_t bPowerOnOk : 1;
	  uint16_t bBoundaryOk : 1;
	}Bit;
};

typedef struct
{	
	uint8_t u8Step;
	
	struct
	{
		uint16_t *pu16UdcBigFlt;
		uint16_t u16WaiteTime;	
		uint16_t u16ErrTime;
		uint16_t u16UdcOld;
	}Vol;

	struct
	{
		int32_t s32IOffsetAd;		
		int32_t s32IOffSetAdSum;		
		uint16_t u16CurOffSetCnt;

	}CurOffset;

	struct
	{
		uint8_t  *u8DectorStatues;		
		uint16_t u16BoundaryCnt;
	}Boundary;
	
	union SELF_CHECK_FLG Flg;
}SELF_CHECK_STR;

struct CTRL_FSM_FLG
{
	struct
	{
		bool bSvmEn ;
		bool bAcrEn ;
		bool bAsrEn ;
		bool bAprEn ;
		bool bObsEn ;
		bool bServOn;
		bool bSelfCheckOk;
	}Bit;
};

typedef struct
{
//	BRK_CTRL_STR BrkCtrl;
	
	struct
	{
			bool bMowerOn;          /* ָʾ�ŷ��Ͽ�ʹ��*/ 
			bool bMowerstate;       /* ָʾ�ŷ��Ͽ�ʹ��*/ 			
	}Out;
		
	struct
	{
			bool bServoOnReq;       /* �ŷ���ʹ��ָ�� */
			bool bServoOffReq;      /* �ŷ���ʹ��ָ�� */
			bool bFaultOccur;       /* ���Ϸ���       */
			bool bPowerOffFlag;     /* ����Դ�ؼ���־ */
			bool bServoOn;          /* ָʾ�ŷ��Ͽ�ʹ��       */
			bool bServoRun;         /* ָʾ�ŷ����б�־��1.�����У�0��ֹͣ���С���ʹ�ܻ��ʹ��ʱδ�ϱ�բ���� */
			bool bFaultRst;					/* ���ϸ�λ */
			bool bBuzzerOn;					/* ���ϸ�λ */		
		  enum MOWER_STATUS_STR eMowerDisplayStatus;
	}State;

}CTRL_LOGIC_STR;

typedef struct
{
	struct 
	{
		enum MOWER_DETECTOR_FSM *peMowerPosition;		
	  uint16_t *pu16Q14LeftCurrent;
	  uint16_t *pu16Q14RightCurrent;		
	}In;
	
	struct 
	{
		uint8_t  u8LEnable;
		uint16_t u16LSpeed;		
		uint8_t  u8LDir;		
		
		uint8_t  u8REnable;
		uint16_t u16RSpeed;	
		uint8_t  u8RDir;	
	}Out;	
	
	struct 
	{
		uint16_t u16StallCnt;		
		uint16_t u16TimeCount1MS;
		bool bTurnFlag;		
		bool bFindBoundary;
		bool bChangeFlag;	
    uint16_t u16Delaytime;		
	}Para;	
	
	struct 
	{
		bool bStallFlag;		
	}Status;	
	
enum WALK_MOTOR_FSM  eMotorFSM;	
	
}WALK_MOTOR_STR;


typedef struct
{
		
	struct 
	{
	  uint16_t *pu16Q14CutCurrent;	
	}In;
	
	
	struct 
	{
		uint8_t u8CutStatues;
		uint32_t u32WorkTimeCnt;				
	}Para;
	
	struct 
	{
		bool  bEnable;			
		bool  bDir;
		uint16_t u16Speed;	
	}Out;	
	
}CUT_MOTOR_STR;

typedef struct
{
	struct
	{
    uint32_t u32StartTimeCnt;/*���뱾�θ��״̬�ļ�ʱ*/
    uint32_t u32CutTimeCnt;/*���ο�����ݵļ�ʱ*/ 	
    uint16_t u16BuzzerCnt;/*���ο�����ݵļ�ʱ*/     		
	}Para;
	
 	struct
	{
		uint16_t  u8TimeMs;
		uint8_t   u8TimeMin;
		uint16_t  u16TimeHour;		
	}Out;
	

		
}TIME_STATISTICS_STR;


typedef struct
{	
	struct
	{
		const bool *pbFaultOccur;
		const bool *pbServoEnable;
		const bool *pbFaultRst;
	}In;
	

	enum MOWER_CTRL_FSM eFSM;		
	SELF_CHECK_STR SelfCheck;		
	struct CTRL_FSM_FLG Flg;
  CTRL_LOGIC_STR CtrlLogic;	
	
	WALK_MOTOR_STR WalkMotor;
	CUT_MOTOR_STR  CutMotor;	
	
	TIME_STATISTICS_STR TimeStatistics;
	
	uint8_t u8TimeRandom;
	uint8_t u8OffFlag;
	uint8_t u8onoff;	

}MOWER_FSM_STR; 



typedef struct
{
	void (*pvMowerFSMInit)(MOWER_FSM_STR *pstrFSM,BOUNDARY_STR *pstrBoundary);
	void (*pvMowerFSMCtrl)(MOWER_FSM_STR *pstrMowerFSM);
	void (*pvWalkMotorCtrl)(WALK_MOTOR_STR *pstrWalkMotor);	
}MOWERFSM_FUN_STR;
  

extern MOWER_FSM_STR  MowerFSM;
extern MOWERFSM_FUN_STR strMowerFSMFun; 
extern uint8_t u8tTimeRandom;

#endif



