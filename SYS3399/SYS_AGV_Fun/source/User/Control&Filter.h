#ifndef __CONTROL_FILTER_H_
#define __CONTROL_FILTER_H_

#include "stdint.h"

#define	Filter1(x,total)    (x)
#define	Filter2(x,total)   (( (((Ulong)total)<<16) + (((Ulong)x)<<15) - (((Ulong)total)<<15) )>>16)
#define	Filter4(x,total)   (( (((Ulong)total)<<16) + (((Ulong)x)<<14) - (((Ulong)total)<<14) )>>16)
#define	Filter8(x,total)   (( (((Ulong)total)<<16) + (((Ulong)x)<<13) - (((Ulong)total)<<13) )>>16)
#define	Filter16(x,total)  (( (((Ulong)total)<<16) + (((Ulong)x)<<12) - (((Ulong)total)<<12) )>>16)
#define	Filter32(x,total)  (( (((Ulong)total)<<16) + (((Ulong)x)<<11) - (((Ulong)total)<<11) )>>16)
#define	Filter64(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<10) - (((Ulong)total)<<10) )>>16)
#define Filter128(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<9 ) - (((Ulong)total)<<9 ) )>>16)
#define Filter256(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<8 ) - (((Ulong)total)<<8 ) )>>16)

#define Max(x,y)	(((x)>(y))?(x):(y))             //取大值函数(适用所有数据类型)
#define Min(x,y)	(((x)<(y))?(x):(y))             //取小值函数(适用所有数据类型)
#define abs(x)  	(((x)>0)?(x):-(x))             //取绝对值的函数(适用所有数据类型)


enum
{
  DISABLE = 0,
  ENABLE ,
} ;

enum
{
	FORWARD=0,
	BACKWARD,
};


enum
{
	TURNLEFT=0,
	TURNRIGHT,
};

enum 
{
	FALSE = 0,
	TRUE=1 ,
} ;


extern uint16_t Modbus_CRC16(uint8_t *Pushdata,uint8_t length);  
extern void DelayTime_ms(uint32_t delay);
#endif


