/**
  ******************************************************************************
  * @file    SC32F58128_qep.h
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/08/08
  * @brief   Qep Module Header File
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
 * Define to prevent recursive inclusion
 ******************************************************************************/
#ifndef __SC32F58128_QEP_H__
#define __SC32F58128_QEP_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include <stdint.h>
#include "IQmathLib.h"
/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/
#define GPIO_QEP0_A                   GPIO_20
#define GPIO_QEP0_B                   GPIO_21
#define GPIO_QEP0_I                   GPIO_23

/**
  * @brief Define the structure of the CALC_CFG_USER Object
	*/
typedef struct {
  uint8_t *id;            // eQEP index
  uint32_t *CCPS;  
  uint32_t *UPPS;
  uint32_t *unitTr;
  int32_t  i32PolePairs;
  int32_t  i32LineEncoder;
	uint32_t u32BaseRpm;      // Parameter: Scaler converting GLOBAL_Q speed to rpm (Q0) speed (unit:rpm)

  
  uint32_t u32rsd0;
  uint32_t *pu32SpdHightestRpm;
  uint32_t *pu32SpdLowestRpm; 
} CALC_CFG_USER;

/**
  * @brief Define the structure of the QEP_CFG Object
	*/
typedef struct {
  uint8_t index;            // eQEP index
	uint32_t CCPS;             // eQEP capture timer clock prescaler
	uint32_t UPPS;             // Unit position event prescaler
	uint32_t unitTr;          // unit timer (us)
	int32_t  lineEncoder;     // lineEncoder

  uint32_t u32SpdHightestRpm;
  uint32_t u32SpdLowestRpm;
} QEP_CFG;
/**
  * @brief Define the structure of the POSPEED Object
	*/
typedef struct {
	_iq       IQThetaMech;              // Output: Motor Mechanical Angle (Q24)(unit:number of pulse)
  _iq       IQThetaElec;              // Output: Motor Electrical Angle (Q24)(unit:number of pulse)
  _iq       IQSpeedM_Mech;            // Output: speed in per-unit (Q24)(pu)
	_iq       IQSpeedT_Mech;            // Output: speed in per-unit (Q24)(pu)
  _iq       IQSpeedM_Elec;            // Output: speed in per-unit (Q24)(pu)
  _iq       IQSpeedT_Elec;            // Output: speed in per-unit (Q24)(pu)
	int32_t   i32SpeedMRpm;             // Output: speed in rpm (Q0)(unit:rpm)
	int32_t   i32SpeedTRpm;             // Output: Speed in rpm (Q0)(unit:rpm)
	uint32_t  u32Status;                // Output: QEP status; bit0: find zero flag; bit1: get Des flag; bit2: Position counter error flag; bit3~bit7: Reserved
  uint32_t  u32IndexFlg;              // Output: Index has alreadly across flag

  uint32_t  u32rsd0[1];  
	int32_t   i32PolePairs;             // Parameter: Number of pole pairs (Q0)
  int32_t   i32LineEncoder;           // Parameter: lineEncoder
  int32_t   i32LineEncoderHalf;
	uint32_t  u32BaseRpm;               // Parameter: Scaler converting GLOBAL_Q speed to rpm (Q0) speed (unit:rpm)
  uint32_t  u32SpeedScaler;           // Parameter: Scaler converting 1/N cycles to a GLOBAL_Q speed (Q24)(pu)
	_iq       IQMechRatio;              // Parameter: 0.9999/total count, total count = 4095 (Q24)(pu)
  _iq       IQMechRatioRpm;           // Parameter: PU of RPM
  uint32_t  CCPS;                     // Parameter: CCPS
  uint32_t  UPPS;                     // Parameter: UPPS
  uint8_t   id;                       // Parameter: QEO ID
  uint8_t   u8CaliEn;                 // Parameter: Calibrate enable bit

  uint32_t  u32rsd1[1];
  _iq       tmp;

  uint32_t  u32rsd2[1];
	uint32_t  u32Dir;                   // Output: Motor rotation direction (Q0)
  uint32_t  QPOSCNT;                  // Output: Encoder counter Value from the QPOSCNT
  int32_t   i32Cnt;                   // Output: finally QEP pluse cnt
  int32_t   i32CntPre;                // Output: previous QEP cnt
	int32_t   i32IndexCnt;              // Output: Encoder counter index (Q0)(unit:number of index)
  
  uint32_t  u32rsd3[1];
 
} POSPEED;

/********************************************************************************
 * Constants and macros
 *******************************************************************************/

/**
  * @defgroup Default initializer for the QEP_CFG Object.
	* @{
	* param1: eQEP capture timer clock prescaler
	* param2: Unit position event prescaler
	* param3: unit timer (us)
	* param4: lineEncoder
  * param5: lowest speed for calc speed(rpm) on T mode
	*/
//#define QEP0_CFG_DEFAULTS {0, 7, 2, 2000, 4095, 1200, 5}
//#define QEP1_CFG_DEFAULTS {1, 7, 2, 2000, 4095, 1200, 5}
//#define QEP0_CFG_DEFAULTS {0, 3, 2, 800, 4096, 1200, 5}
//#define QEP1_CFG_DEFAULTS {1, 3, 2, 800, 4096, 1200, 5}
#define QEP0_CFG_DEFAULTS {0, 2, 2, 1000, 4000, 1800, 5}
#define QEP1_CFG_DEFAULTS {1, 2, 2, 1000, 4000, 1800, 5}
/**
  * @brief      angle and Speed Calcultation MACRO
  * @param[in]  index  : QEP module index; 0 or 1
  * @param[in]  spdPos : The type of POSPEED 
  * @retval            : void
	*
*/
#define QEP_ANGLE_MACRO(index, spdPos)                       \
{                                                            \
/*  uint32_t CNT = 0;                                         */ \
  /* get the direction for QEP encoder. 1:CW; 0:CCW */       \
  spdPos.i32Cnt = EQEP##index->QPOSCNT;                                \
/*  spdPos.i32Cnt = *spdPos.pQPOSCNT; */                                    \
  if(spdPos.i32Cnt >= spdPos.i32LineEncoder){                \
    spdPos.i32Cnt = spdPos.i32Cnt - spdPos.i32LineEncoder;   \
  }                                                          \
  else if(spdPos.i32Cnt < 0){                                \
    spdPos.i32Cnt = spdPos.i32Cnt + spdPos.i32LineEncoder;   \
  }                                                          \
  spdPos.IQThetaMech = spdPos.i32Cnt * spdPos.IQMechRatio;   \
  spdPos.IQThetaElec = (spdPos.IQThetaMech * spdPos.i32PolePairs) & 16777215; \
}

// *************************************************
//  Reset QEP controller to zero initial conditions
// *************************************************
#define RESET_QEP_MACRO(v)   \
  v.IQThetaMech = 0;                \
  v.IQThetaElec  = 0;                \
  v.i32Cnt = 0;                \
  v.i32CntPre = 0;            \
  v.u32Dir = 0;               \
  v.i32IndexCnt = 0;

/*******************************************************************************
 * Prototypes of the functions
 ******************************************************************************/
extern QEP_CFG sQepCfg;
extern CALC_CFG_USER sCalcCfgUser;
extern volatile EQEP0_Type *eQEP[];

extern void QepCfgInit(uint8_t index, QEP_CFG sCfgDat);
extern void QEP_SPEED_MACRO1(uint8_t index, POSPEED * spdPos);
extern void ResetQep(uint8_t index);
extern void BSP_QepInit(void);
extern int32_t BSP_GetQepCnt(uint8_t index);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __SC32F58128_QEP_H__ */


