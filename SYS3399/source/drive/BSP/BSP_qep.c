/**
  ******************************************************************************
  * @file    SC32F58128_qep.c
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/08/08
  * @brief   Qep Module Source File
  * @details None
	* @note    None
  ******************************************************************************
  * @attention
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SILAN MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
  ******************************************************************************
	* Change History
	* -# 2017/08/08 | v1.0.0 | Lijianbo | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "BSP_qep.h"
#include <SC32F58128.h>
#include <SC32F58128_gpio.h>
#include "BSP_gpio.h"
#include <stdlib.h>
#include <math.h>
//#include "UserConfig.h"
/*******************************************************************************
 * Global data for the project
 ******************************************************************************/
volatile EQEP0_Type *eQEP[] = {EQEP0,EQEP1,};

QEP_CFG sQepCfg = QEP0_CFG_DEFAULTS;
QEP_CFG sQepCfg1 = QEP1_CFG_DEFAULTS;

/*******************************************************************************
 * Local prototypes function
 ******************************************************************************/

/*******************************************************************************
 * Lacal function for the current file
 ******************************************************************************/
void QepCfgInit(uint8_t index, QEP_CFG sCfgDat);
void PositionCtlDeal(uint8_t index, POSPEED *spdPos);
void InitQep0Gpio(void);

/**
  * @brief     QEP BSP Initialization.
  * @param[in] void : void
  */
void BSP_QepInit(void)
{
//  sQepCfg.index = 0;
//  sQepCfg.lineEncoder = ENC_PPR;
//  sQepCfg.unitTr = 1000;
//  sQepCfg.CCPS = 0;
//  sQepCfg.UPPS = 0;
//  QepCfgInit(sQepCfg.index, sQepCfg);
	
	sQepCfg.index = 1;
  sQepCfg.lineEncoder = ENC_PPR;
  sQepCfg.unitTr = 1000;
  sQepCfg.CCPS = 0;
  sQepCfg.UPPS = 0;
  QepCfgInit(sQepCfg.index, sQepCfg);
  InitQep0Gpio();

}

/**
  * @brief      eQEP config param initialization
  * @param[in]  index   : QEP module index; 0 or 1
  * @param[in]  sCfgDat : the type of QEP_CFG
  * @retval             : void
	*
*/
void QepCfgInit(uint8_t index, QEP_CFG sCfgDat)
{
  uint64_t SysClkMhz = 0, tmp = 0;
  SysClkMhz = SystemCoreClock;
	(*eQEP[index]).QUPRD              = sCfgDat.unitTr * SysClkMhz / 1000000;   // Unit Timer at SYSCLKOUT
	
	(*eQEP[index]).QDECCTL_b.QSRC     = 0;        // QEP mode for Direction-count modE
	
	(*eQEP[index]).QEPCTL_b.FREE_SOFT = 0;        // position counter, Watchdog counter, Unit timer and Capture timer is unaffected by emulation suspend 
	(*eQEP[index]).QEPCTL_b.PCRM      = 1;        // 01 mode -- QPOSCNT reset on MAX
	(*eQEP[index]).QEPCTL_b.IEL       = 3;
	(*eQEP[index]).QEPCTL_b.UTE       = 1;        // Unit Timeout Enable
	(*eQEP[index]).QEPCTL_b.QCLM      = 1;        // Latch on unit time out.
									                              // Position counter, capture timer and capture period values are latched into 
                                                // QPOSLAT, QCTMRLAT and QCPRDLAT registers on unit time out
	(*eQEP[index]).QEPCTL_b.QPEN      = 1;        // eQEP position counter is enable
	
	(*eQEP[index]).QCAPCTL_b.UPPS     = sCfgDat.UPPS ;       // 1/8 for unit position, Unit position event prescaler
	(*eQEP[index]).QCAPCTL_b.CCPS     = sCfgDat.CCPS;        // 7:1/128 for CAP clock, eQEP capture timer clock prescaler
	(*eQEP[index]).QCAPCTL_b.CEN      = 1;                   // QEP Capture Enable
	
	(*eQEP[index]).QPOSMAX            = sCfgDat.lineEncoder - 1;
	
	(*eQEP[index]).QPOSCNT            = 0;
	
	(*eQEP[index]).QEINT_b.IEL        = 0;        // Index event latch interrupt enable
	(*eQEP[index]).QEINT_b.PCO        = 0;        // Position counter overflow interrupt enable
	(*eQEP[index]).QEINT_b.PCU        = 0;        // Position counter underflow interrupt enable 
//	(*eQEP[index]).QEINT_b.PCE      = 1;        // Position counter error interrupt enable
	(*eQEP[index]).QEINT_b.UTO        = 1;        // UTO interrupt enable
  
	(*eQEP[index]).QEPSTS_b.FIMF      = 1;
}

void ResetQep(uint8_t index){
  eQEP[index]->QPOSCNT = 0;
  eQEP[index]->QEPCTL_b.UTE = 0;
  eQEP[index]->QCLR = 0x0800;               /* Clear UTO interrupt flag */ 
  eQEP[index]->QUTMR = 0;
  eQEP[index]->QPOSLAT = 0;
  eQEP[index]->QEPCTL_b.UTE = 1;
}

int32_t BSP_GetQepCnt(uint8_t index)
{
   #if USER_CONFIG_SEL == USER_CONFIG_HC_CH_02/* 之前相序反了 */
   return (eQEP[index]->QPOSCNT);
   #else
   return (ENC_PPR - 1 - eQEP[index]->QPOSCNT);
   #endif
}

void InitQep0Gpio(void)
{
  /* QEP Gpio Config */
  GPIO_MuxSel(GPIO_QEP0_A, EQEP0_AIN_20);
  GPIO_MuxSel(GPIO_QEP0_B, EQEP0_BIN_21);
  GPIO_MuxSel(GPIO_QEP0_I, EQEP0_I_23);
	
	GPIO_MuxSel(GPIO_QEP1_A, EQEP1_AIN_31);
  GPIO_MuxSel(GPIO_QEP1_B, EQEP1_BIN_30);
  GPIO_MuxSel(GPIO_QEP1_I, PWM_TZ2_29);
	
}
