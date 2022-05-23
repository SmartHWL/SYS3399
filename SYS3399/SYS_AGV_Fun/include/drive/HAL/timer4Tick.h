/**************************************************************************//**
 * @file     timer4Tick.c
 * @brief    Timer for Tick Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/

/******************************************************************************/

#ifndef __TIMER4TICK_H__
#define __TIMER4TICK_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include "stdint.h"

/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/
/**< timer name */ 
typedef enum
{
  TMR_UART = 0,       /**< timer for uart                           */
	TMR_MOTOR_LOCK ,    /**< timer for motor lock function            */
	TMR_OPEN_LOOP,      /**< timer for motor open loop function       */
	TMR_STOP_MOTOR,     /**< timer for motor stop slowly              */
  TMR_SELF_PULLUP,    /**< timer for motor IGBT driver slef pull up */
  TMR_HW_CHK,         /**< timer for motor hardware check           */
}e_TmrName;  

/**< timer work sate */ 
typedef enum
{
  TMR_RUN = 0,        /**<                                          */
  TMR_RUN_END,        /**<                                          */
  TRM_CLOSED,         /**<                                          */
  TRM_ERROR           /**<                                          */   
}e_TmrState;  

/**< timer data and state */
typedef struct
{
  uint16_t    u16Dat; /**<                                          */
  e_TmrState  eState; /**<                                          */
}s_auxTmr;    
  
/** timer bit define */  
struct TIMER_BITS {       // bits   description
  uint16_t MS1_FLG  :1;   // 0      1ms flag Bit
  uint16_t MS10_FLG :1;   // 1      10ms flag Bit
  uint16_t MS100_FLG:2;   // 2      100 ms flag Bit
  uint16_t rsvd1    :12;  // 15:3   reserved
};

union TIMER_REG {
   uint16_t           all;
   struct TIMER_BITS  bit;
};

/** timer data struct */
typedef struct
{
  s_auxTmr * psAuxTmr;    /**<  aux timer                          */
  uint32_t  u32MaxTmr;    /**<  max channel for aux timer          */
  uint32_t  u32Cnt4Ms;    /**<  counter for 1ms,10ms and 100ms     */
  uint32_t  u32TickCnt;   /**<  tick count for aux cnt             */
  union TIMER_REG uTmrFlg;/**<  time flag for 1ms 10ms and 100ms   */
}s_auxTmr_t; 

/********************************************************************************
 * Constants and macros
 *******************************************************************************/
//#define FALSE   0
//#define TRUE    1

#define MAX_TMR_NUMB4C0   6     //max timer number for control
#define MAX_TMR_NUMB4C1   5     //max timer number for control
#define MAX_UART_TIMEROUT 100  //
//#define TEST_CNT  10

/**< timer judge flag */
#define CLR_1MS_FLG(v)        (v.uTmrFlg.bit.MS1_FLG   = 0 )
#define CLR_10MS_FLG(v)       (v.uTmrFlg.bit.MS10_FLG  = 0 )
#define CLR_100MS_FLG(v)      (v.uTmrFlg.bit.MS100_FLG = 0 )
#define IS_1MS_FLG_SET(v)     (v.uTmrFlg.bit.MS1_FLG   == 1)
#define IS_10MS_FLG_SET(v)    (v.uTmrFlg.bit.MS10_FLG  == 1)
#define IS_100MS_FLG_SET(v)   (v.uTmrFlg.bit.MS100_FLG == 1)

/********************************************************************************
 * Variables
 *******************************************************************************/
extern s_auxTmr_t g_Tmr4C0, g_Tmr4C1;
extern uint32_t   g_u32SysTickNumb;

/********************************************************************************
 * prototypes of the functions
 *******************************************************************************/
extern void TICK_TimerTickInit(s_auxTmr_t * pTmr);
extern void TICK_TimerTickRunning(s_auxTmr_t * pTmr);
extern void TICK_TimerSet(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value);
extern void TICK_TimerReset(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value);
extern void TICK_TimerClose(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value);
extern uint8_t TICK_TimerChk(s_auxTmr_t * pTmr,uint8_t timer);
extern void TICK_TimerInit4C0(s_auxTmr_t * pTmr);
extern void TICK_TimerInit4C1(s_auxTmr_t * pTmr);
extern uint16_t TICK_TimerChkCnt(s_auxTmr_t * pTmr,uint8_t timer);

/*---------------------------------- End of the file ---------------------------------*/
#endif /*__TIMER4TICK_H__ */
