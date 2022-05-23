#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_
#include "stdint.h"


/*====================================================================
软件版本声明规范：
首字母：		d:DEBUG 测试版本
			r:RELEASE 正式版本
			
第一位（1）：主版本号。当功能模块有较大的变动，比如增加多个模块或者整体
			架构发生变化。		此版本号由项目决定是否修改。
第二位（2）：子版本号。当功能有一定的增加或变化，比如增加了对权限控制、
			增加自定义视图等功能。此版本号由项目决定是否修改。
第三位（3）：修订版本号。一般是 Bug 修复或是一些小的变动，要经常发布修
			订版，时间间隔不限，修复一个严重的bug即可发布一个修订版。
			此版本号由项目经理决定是否修改。
日期版本号（210706）： 用于记录修改项目的当前日期，每天对项目的修改
			都需要更改日期版本号。此版本号由开发人员决定是否修改
=====================================================================*/

/*  电机驱动版本				 主版本+子版本+修订版本+日期版本	*/
//#define CORE_SOFTWARE_VERSION 		20101220217LL
//#define CORE_SOFTWARE_MAIN_VERSION  (CORE_SOFTWARE_VERSION / 10000000000LL)
//#define CORE_SOFTWARE_SUB_VERSION   (CORE_SOFTWARE_VERSION / 100000000 % 100)
//#define CORE_SOFTWARE_MAIN_SUB_VERSION (CORE_SOFTWARE_VERSION/100000000)
//#define CORE_SOFTWARE_DATA_VERSION 	(CORE_SOFTWARE_VERSION % 10000)


#define CORE_SOFTWARE_MAIN_VERSION   0
#define CORE_SOFTWARE_SUB_VERSION    1

#if DEBUG_MODE
#define CORE_SOFTWARE_MODE          "D"
#elif RELEASE_MODE
#define CORE_SOFTWARE_MODE          "R"
#endif


/*============================================Board Info==========================================================*/
#define MCU_VOL								3.3		/* 芯片供电电压 V*/
#define VOL_BASE						  30.0 	/* 母线基准电压 V */
#define VOL_DIV_RES						20.0  /* 母线分压电阻 */
#define VOL_SAMP_RES					2.0		/* 母线采样电阻 */

#define CURR_BASE						  10.0 	/* 充电基准电流 */
#define RES_SAM								0.08	/* 采样电阻ohm */
#define OPA_AMP							  2.5	/* OPA放大倍数 */

#define	VOL_MUL						_IQ((MCU_VOL/VOL_SAMP_RES)*(VOL_DIV_RES+VOL_SAMP_RES)/VOL_BASE) /* 母线基准电压校正 */
#define ADC_ADJ           _IQ(MCU_VOL/(CURR_BASE*RES_SAM*OPA_AMP))	/* ADC定标 */


/*============================================Protect Info==========================================================*/
#define TIMEMS(x)      (x)        		 /**MS**/
#define TIMES(x)       (x*1000)        /**S**/
#define TIMEMIN(x)     (60*TIMES(x))   /**MINNUTE**/
#define TIMEHOUR(x)    (60*TIMEMIN(x)) /**HOUR**/

#define PRO_VOL_UD						140		/* 欠压保护值 *0.1V */
#define PRO_VOL_UD_TIME				TIMES(1)  
#define PRO_VOL_OV						235		/* 过压保护值 *0.1V */
#define PRO_VOL_OV_TIME				TIMES(1)

#define PRO_STOP_THRESHOLD   (ADC_MAX*0.3)  /* STOP按下的阈值 */
#define PRO_STOP_TIME				  TIMES(0.5)          /* STOP按下的时间 */

#define PRO_BOUNDARY_TIME			TIMES(1)		/* 边界错误的阈值 */

#define PRO_CURR_CHARGE				20					/* 过流保护值2A *0.1A */
#define PRO_CURR_CHARGE_TIME	TIMES(0.2)	/* 过流保护的时间*/

#define PRO_ROLL_ANGLE				((90-20)*(4096/90))	/* 保护角度25 */
#define PRO_ROLL_TIME         TIMES(0.35)		      /* 过流保护的时间*/

/*============================================ADC GPIO Info==========================================================*/
#define ADC_MAX			(1<<12)
/* 检测左右调换*/
#define ADC_RN      ADC->ADCRESULT0
#define ADC_RP      ADC->ADCRESULT1
#define ADC_LN			ADC->ADCRESULT2
#define ADC_LP			ADC->ADCRESULT3

#define ADC_CHARGECURR    ADC->ADCRESULT4
#define ADC_STOP1					ADC->ADCRESULT5
#define ADC_STOP2					ADC->ADCRESULT6
#define ADC_RAIN					ADC->ADCRESULT7
#define ADC_CHARGEVOL 		ADC->ADCRESULT8
#define ADC_BATTERYVOL    ADC->ADCRESULT9

/*============================================TIME&&SPEPD Info==========================================================*/
#define OFFTIMEDELAY 		1000/* 关机延时时间 ms */

#define RANDOMMAX       180  /* 随机转向的最大角度*/
#define RANDOMMMIN      20   /* 随机转向的最小角度*/\

#define TRACESPEED      2000    /* 边界走速度*/
#define TURNSPEED       1000   /* 随机转向速度*/
#define WALKSPEED       3500   /* 行走的速度*/

#define CUTSPEED        5000   /* 割草的速度*/


/*============================================MOWER WALK Info==========================================================*/
/*返回充电是顺时针还是逆时针 */
#define  ALONGFORWARD           0
#define  ALONGBACKWARD          1
#define  ALONG    ALONGFORWARD


/*上下倾倒调换 */
#define  UP        0  /**机器头部面朝边界线负极 P->N**/
#define  DOWM      1  /**机器头部面朝边界线正极 N->P**/
#define  DECTOR    UP  

/*============================================Battery  Info==========================================================*/
#define BATTERYSEREIS   5
#define PERCENTAGE100  (42.0*BATTERYSEREIS)
#define PERCENTAGE90   (40.6*BATTERYSEREIS)
#define PERCENTAGE80   (39.8*BATTERYSEREIS)
#define PERCENTAGE70   (39.2*BATTERYSEREIS)
#define PERCENTAGE60   (38.7*BATTERYSEREIS)
#define PERCENTAGE50   (38.2*BATTERYSEREIS)
#define PERCENTAGE40   (37.9*BATTERYSEREIS)
#define PERCENTAGE30   (37.7*BATTERYSEREIS)
#define PERCENTAGE20   (37.4*BATTERYSEREIS)
#define PERCENTAGE10   (36.8*BATTERYSEREIS)
#define PERCENTAGE5    (34.5*BATTERYSEREIS)
#define PERCENTAGE0    (30.0*BATTERYSEREIS)

/*============================================MOWER Info==========================================================*/
#define RADIUS_WHEEL    (0.105)  /*轮子半径*/
#define RADIUS_MOWER    (0.14)/*车距的半径*/

#define RATIO_OF_REDUCTION  100/*减速比*/


#endif



