#ifndef __FAULT_H_
#define __FAULT_H_
#include "stdint.h"
#include "stdbool.h"
#include "ErrCode.h"


#define FAULT_DEPTH				5
#define FAULT_RCD_NUM 			10

typedef struct
{
	/* 编号 */
	
	enum ERR_CODE u16ErrCode;
	/* 履历（错误记录） */
	uint8_t u8IsAddHistory;
	/* 可清除 */
	uint8_t u8IsClearable;
	/* 即时停止 */
	uint8_t u8IsImmediateStop;
}FAULT_INFO_STR;


typedef struct
{
	struct
	{
		uint16_t u16FaultListLineNum;
	}Para;
	
	struct
	{
		struct
		{
			bool bFaultOccur;
		}Flg;
		
		enum ERR_CODE u16FaultCurrentList[FAULT_DEPTH]; /* 正发生的多个故障 */
		FAULT_INFO_STR CurrentFault;
		uint8_t u8FaultAmountCur;
	}Out;

}FAULT_STR;

typedef struct
{
	void (*pvFaultParaInit)(void);
	void (*pvFaultHandle)(uint16_t u16ErrCode, uint8_t u8AxisNum);
	void (*pvFaultCurrentClear)(uint8_t u8AxisNum);
  void (*pvFaultClearByNO)(uint16_t u16ErrCode);
}FAULT_FUN_STR;

extern FAULT_FUN_STR strFaultFun;
extern FAULT_STR strFault[MOTOR_AXIS];
#endif

