#ifndef __OSC_H__
#define __OSC_H__
#include "stdint.h"

extern void DBG_ParamInit(void);
extern void WAVE_Init(void);
extern void WAVE_FillWaveBuff(void);
extern void DBG_SetRxData(uint8_t u8UartData);
extern void DBG_RxPackageDeal(void);
extern void DBG_WaveTrigRw(uint32_t u32TrigData);
extern uint8_t  g_u8TxBuffState;
extern int8_t   g_rest_system;

// #define TRIG_SINGLE_MODE (0x80000000)
#define WAV_BUF_SIZE     2000     //max WAVE buff size for all wave 4000

/*---------------------------------- End of the file ---------------------------------*/
#endif
