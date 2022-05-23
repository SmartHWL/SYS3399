
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_gpio.h"
#include "UserConfig.h"
#include "SampleInfo.h"
#include "BSP_uart.h"
#include "UartToMotor.h"
#include "Control&Filter.h"
#include "IQmathLib.h"



/* 结构体指针声明 */
SAMPLE_INFO_STR strSampleInfo;

/* 函数声明 */
void vSampleParaInit(SAMPLE_INFO_STR *pstr,SELF_CHECK_STR *pstrSelfChek);
void vGetSampleInfo(SAMPLE_INFO_STR *pstr);

/* 函数指针声明 */
SAMPLE_INFO_FUN_STR strSampleInfoFun = 
{
	&vSampleParaInit,
	&vGetSampleInfo
};

void vSampleParaInit(SAMPLE_INFO_STR *pstrSample,SELF_CHECK_STR *pstrSelfChek)
{
	memset(&strSampleInfo,0,sizeof(strSampleInfo));   /*初始化*/
  pstrSelfChek->Vol.pu16UdcBigFlt = &pstrSample->BatteryVol.Out.u16UdcX10;
  pstrSample->ChargeCurr.In.ps32IChargeOffsetAd= &pstrSelfChek->CurOffset.s32IOffsetAd;
			
}



void vGetSampleInfo(SAMPLE_INFO_STR *pstr)
{
	  bool  bStopStatues;
	
		/* 电池母线电压计算 */
		pstr->BatteryVol.In.s16UdcAd = ADC_BATTERYVOL;
		pstr->BatteryVol.Out.s32UdcPU = _IQmpy(_IQ12toIQ(pstr->BatteryVol.In.s16UdcAd), VOL_MUL);
		pstr->BatteryVol.Out.s16UdcQ12PU = pstr->BatteryVol.Out.s32UdcPU >> 12;
		pstr->BatteryVol.Out.s32UdcFltPU += (pstr->BatteryVol.Out.s32UdcPU - pstr->BatteryVol.Out.s32UdcFltPU) >> 3;
		pstr->BatteryVol.Out.u16UdcX10 = _IQmpy(pstr->BatteryVol.Out.s32UdcFltPU, VOL_BASE * 10);
	
	  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE90)        pstr->BatteryVol.Out.u16UdcPercentage=100;
	  else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE80)  pstr->BatteryVol.Out.u16UdcPercentage=90;
		else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE70)  pstr->BatteryVol.Out.u16UdcPercentage=80;
		else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE60)  pstr->BatteryVol.Out.u16UdcPercentage=70;	
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE50)  pstr->BatteryVol.Out.u16UdcPercentage=60;	
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE40)  pstr->BatteryVol.Out.u16UdcPercentage=50;	
		else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE30)  pstr->BatteryVol.Out.u16UdcPercentage=40;	
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE20)  pstr->BatteryVol.Out.u16UdcPercentage=30;	
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE10)  pstr->BatteryVol.Out.u16UdcPercentage=20;		
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE5)   pstr->BatteryVol.Out.u16UdcPercentage=10;			
    else  if(pstr->BatteryVol.Out.u16UdcX10>PERCENTAGE0)   pstr->BatteryVol.Out.u16UdcPercentage=5;		
	  else     																							 pstr->BatteryVol.Out.u16UdcPercentage=0;		
	
		/* 充电桩电压计算 */
		pstr->ChargeVol.In.s16UChargeAd = ADC_CHARGEVOL;
		pstr->ChargeVol.Out.s32UChargePU = _IQmpy(_IQ12toIQ(pstr->ChargeVol.In.s16UChargeAd), VOL_MUL);	
		pstr->ChargeVol.Out.s16UChargeQ12PU = pstr->ChargeVol.Out.s32UChargePU >> 12;
		pstr->ChargeVol.Out.s32UChargeFltPU += (pstr->ChargeVol.Out.s32UChargePU - pstr->ChargeVol.Out.s32UChargeFltPU) >> 5;
		pstr->ChargeVol.Out.u16UChargeX10 = _IQmpy(pstr->ChargeVol.Out.s32UChargeFltPU,  VOL_BASE*10);			
	

		/* 充电电流计算 */
		pstr->ChargeCurr.In.s16IChargeAd = ADC_CHARGECURR;	
	  pstr->ChargeCurr.Out.s32IChargePU = _IQmpy(_IQ12toIQ(-pstr->ChargeCurr.In.s16IChargeAd + *pstr->ChargeCurr.In.ps32IChargeOffsetAd), ADC_ADJ);			
		pstr->ChargeCurr.Out.s16IChargeQ12PU = pstr->ChargeCurr.Out.s32IChargePU >> 12;
		pstr->ChargeCurr.Out.s32IChargeFltPU += (pstr->ChargeCurr.Out.s32IChargePU - pstr->ChargeCurr.Out.s32IChargeFltPU) >> 5;
		pstr->ChargeCurr.Out.s16IChargeX10 = _IQmpy((pstr->ChargeCurr.Out.s32IChargeFltPU), CURR_BASE * 10);	
    if(pstr->ChargeCurr.Out.s16IChargeX10<0) pstr->ChargeCurr.Out.s16IChargeX10=0;

		/* RAIN */
		pstr->Rain.In.s16RainAd = ADC_RAIN;
		pstr->Rain.Out.s16RainFlt += (pstr->Rain.In.s16RainAd - pstr->Rain.Out.s16RainFlt) >> 3;


		/* STOP1 */
		pstr->Stop1.In.s16StopAd = ADC_STOP1;
		pstr->Stop1.Out.s16StopFlt += (pstr->Stop1.In.s16StopAd - pstr->Stop1.Out.s16StopFlt) >> 2;

		/* STOP2 */
		pstr->Stop2.In.s16StopAd = ADC_STOP2;
		pstr->Stop2.Out.s16StopFlt += (pstr->Stop2.In.s16StopAd - pstr->Stop2.Out.s16StopFlt) >> 2;
	
	
		/* 开机按键 */
		#define DEF_SW_STATUS_OPEN      (1)
		#define DEF_SW_STATUS_CLOSED    (0)
		#define DEF_YES (1u)
		#define DEF_NO  (0u)

		pstr->Switch.start_handler_status =IO_FLITER( &pstr->Switch.S0, START_HANDLER, 100); /*主开关，按下为0*/
		
		if(pstr->Switch.start_handler_status == DEF_SW_STATUS_OPEN)/*抬升过100ms*/
		{
			pstr->Switch.start_handler_released_before = DEF_YES;
		}	
		if((pstr->Switch.start_handler_status == DEF_SW_STATUS_CLOSED)&&(pstr->Switch.start_handler_released_before == DEF_YES))
		{
			pstr->Switch.onoff_handler_status_out=TRUE;	/*输出关机使能位*/			
		}
	
		/* 左倾倒开关 */			
		pstr->Switch.LeftLiftStatus  =IO_FLITER( &pstr->Switch.S2, !LEFTLIFT_HANDLER, 100);		
		/* 右倾倒开关 */		
		pstr->Switch.RightLiftStatus =IO_FLITER( &pstr->Switch.S3, !RIGHTLIFT_HANDLER, 100);		
	
}



uint8_t IO_FLITER(tIOfliter * ios, int8_t ioval, uint16_t cpval)
{
    uint16_t cnt = ios->cnt;
    uint8_t io_val0 = ios->val;
    
    if(ioval == -1/*端口值无效*/)
    {
        return io_val0;
    }
    
    if(io_val0 != ioval)
    {
        cnt++;
        if(cnt >= cpval/*定值*/)
        {
            ios->val = ioval;/*更新值*/
            cnt = 0;
            return ioval;
        }
    }else
    {
        cnt = 0;
    }
    ios->cnt = cnt;
    return io_val0;
}