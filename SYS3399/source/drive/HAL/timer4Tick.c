/**************************************************************************//**
 * @file     timer4Tick.c
 * @brief    Timer for Tick Source File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/


/******************************************************************************
* include file
*******************************************************************************/
#include "timer4Tick.h"
#include "UserConfig.h"
//#include "SilanM0.h"  
//#include "devInit.h"
//#include "timer.h"
//#include "sl_niv.h"


/******************************************************************************
* global data for the project
*******************************************************************************/
s_auxTmr_t g_Tmr4C0, g_Tmr4C1;
uint32_t   g_u32SysTickNumb;

/******************************************************************************
* loacal data for the current file
*******************************************************************************/
static s_auxTmr mg_Tmr4C0[MAX_TMR_NUMB4C0];
static s_auxTmr mg_Tmr4C1[MAX_TMR_NUMB4C1];

/******************************************************************************
* local prototypes function
******************************************************************************/

/******************************************************************************
* loacal function for the current file
*******************************************************************************/


/******************************************************************************
* global function for the current project
*******************************************************************************/
/**
  * @brief     init all data for timer .
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerInit4C0();
  * @endcode
  * @Note
*/ 
void TICK_TimerInit4C0(s_auxTmr_t * pTmr)
{
  pTmr->u32Cnt4Ms = 0;
  pTmr->psAuxTmr = mg_Tmr4C0;
  pTmr->u32MaxTmr = MAX_TMR_NUMB4C0;
  pTmr->u32TickCnt = 0;
  pTmr->uTmrFlg.all = 0;
  TICK_TimerTickInit(pTmr);
}

/**
  * @brief     init all data for timer .
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerInit4C1();
  * @endcode
  * @Note:
*/ 
void TICK_TimerInit4C1(s_auxTmr_t * pTmr)
{
  pTmr->u32Cnt4Ms = 0;
  pTmr->psAuxTmr = mg_Tmr4C1;
  pTmr->u32MaxTmr = MAX_TMR_NUMB4C1;
  pTmr->u32TickCnt = 0;
  pTmr->uTmrFlg.all = 0;
  TICK_TimerTickInit(pTmr);
}

/**
  * @brief     init all data for the function.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerTickInit();
  * @endcode
  * @Note:
*/ 
void TICK_TimerTickInit(s_auxTmr_t * pTmr)
{
  uint8_t i;

  s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

  for(i = 0; i < pTmr->u32MaxTmr; i++)
  {
    psAuxTmr->eState = TRM_CLOSED;
    psAuxTmr->u16Dat = 0;
    psAuxTmr++;
  }
}

/**
  * @brief     init all data for the function.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerTickInit();
  * @endcode
  * @Note:
*/ 
void TICK_TimerTickRunning(s_auxTmr_t * pTmr)
{
  uint8_t i;

  s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

  if(g_u32SysTickNumb == pTmr->u32TickCnt)
		return;

  pTmr->u32TickCnt = g_u32SysTickNumb;
  pTmr->u32Cnt4Ms ++;
  pTmr->uTmrFlg.bit.MS1_FLG = 1;
  for(i = 0; i < pTmr->u32MaxTmr; i++)
  {
    if(psAuxTmr->eState == TMR_RUN)
    {
      if(psAuxTmr->u16Dat != 0)   psAuxTmr->u16Dat--;
      else                        psAuxTmr->eState = TMR_RUN_END;
    }
    psAuxTmr++;
  }
  if((pTmr->u32Cnt4Ms%10)== 0)// 10ms reach
  {
    pTmr->uTmrFlg.bit.MS10_FLG = 1;
    if(pTmr->u32Cnt4Ms >= 100)// 100ms reach, 100 tick is one period
    {
      pTmr->u32Cnt4Ms = 0;
      pTmr->uTmrFlg.bit.MS100_FLG = 1;
    }
  }
}


/**
  * @brief     set timer for begin run.
  * @param[in]  timer timer channel 
  * @param[in]  value timer counter ,unit 1mS
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerSet(1,100);    //set 1 channel 100ms and begin work
  * @endcode
  * @Note:
*/ 
void TICK_TimerSet(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value)
{
  s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

  psAuxTmr += timer;
  if(timer < pTmr->u32MaxTmr)
  {
    if(psAuxTmr->eState > TMR_RUN_END)
    {
      psAuxTmr->eState = TMR_RUN;
      psAuxTmr->u16Dat = value;
    }
  }
}

/**
  * @brief     reset timer for begin run.
  * @param[in]  timer timer channel 
  * @param[in]  value timer counter ,unit 1mS
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerReset(1,100);    //set 1 channel 100ms and begin work
  * @endcode
  * @Note:
*/ 
void TICK_TimerReset(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value)
{
   s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

   psAuxTmr += timer;
   if(timer < pTmr->u32MaxTmr)
   {
      psAuxTmr->eState = TMR_RUN;
      psAuxTmr->u16Dat = value;     
   }     
}

/**
  * @brief     reset timer for begin run.
  * @param[in]  timer timer channel 
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerClose(1);    //stop 1 channel  timer
  * @endcode
  * @Note:
*/ 
void TICK_TimerClose(s_auxTmr_t * pTmr,uint8_t timer,uint16_t value)
{
   s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

   psAuxTmr += timer;
   if(timer < pTmr->u32MaxTmr)
   {
      psAuxTmr->eState = TRM_CLOSED;
   }     
}


/**
  * @brief     check timer run or not.
  * @param[in]  timer timer channel 
  * @retval      0: this timer is not running
  * @retval      1: this timer is  running
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerChk(1);    //stop 1 channel  timer
  * @endcode
  * @Note:
*/ 
uint8_t TICK_TimerChk(s_auxTmr_t * pTmr,uint8_t timer)
{
  uint8_t i = FALSE;

  s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

  psAuxTmr += timer;
  if(timer < pTmr->u32MaxTmr)
  {
    if(psAuxTmr->eState == TMR_RUN)
			i = TRUE;
  }
  return i;   
}

/**
  * @brief     get time current count value.
  * @param[in]  timer timer channel 
  * @retval      uint16_t: get timer current vount vlaue
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * TICK_TimerChk(1);    //stop 1 channel  timer
  * @endcode
  * @Note:
*/ 
uint16_t TICK_TimerChkCnt(s_auxTmr_t * pTmr,uint8_t timer)
{
  s_auxTmr * psAuxTmr = pTmr->psAuxTmr;

  psAuxTmr += timer;
  return psAuxTmr->u16Dat;   
}
/*---------------------------------- End of the file ---------------------------------*/
