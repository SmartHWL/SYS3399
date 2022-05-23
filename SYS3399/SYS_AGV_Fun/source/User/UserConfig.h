#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_
#include "stdint.h"


/*====================================================================
����汾�����淶��
����ĸ��		d:DEBUG ���԰汾
			r:RELEASE ��ʽ�汾
			
��һλ��1�������汾�š�������ģ���нϴ�ı䶯���������Ӷ��ģ���������
			�ܹ������仯��		�˰汾������Ŀ�����Ƿ��޸ġ�
�ڶ�λ��2�����Ӱ汾�š���������һ�������ӻ�仯�����������˶�Ȩ�޿��ơ�
			�����Զ�����ͼ�ȹ��ܡ��˰汾������Ŀ�����Ƿ��޸ġ�
����λ��3�����޶��汾�š�һ���� Bug �޸�����һЩС�ı䶯��Ҫ����������
			���棬ʱ�������ޣ��޸�һ�����ص�bug���ɷ���һ���޶��档
			�˰汾������Ŀ��������Ƿ��޸ġ�
���ڰ汾�ţ�210706���� ���ڼ�¼�޸���Ŀ�ĵ�ǰ���ڣ�ÿ�����Ŀ���޸�
			����Ҫ�������ڰ汾�š��˰汾���ɿ�����Ա�����Ƿ��޸�
=====================================================================*/

/*  ��������汾				 ���汾+�Ӱ汾+�޶��汾+���ڰ汾	*/
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
#define MCU_VOL								3.3		/* оƬ�����ѹ V*/
#define VOL_BASE						  30.0 	/* ĸ�߻�׼��ѹ V */
#define VOL_DIV_RES						20.0  /* ĸ�߷�ѹ���� */
#define VOL_SAMP_RES					2.0		/* ĸ�߲������� */

#define CURR_BASE						  10.0 	/* ����׼���� */
#define RES_SAM								0.08	/* ��������ohm */
#define OPA_AMP							  2.5	/* OPA�Ŵ��� */

#define	VOL_MUL						_IQ((MCU_VOL/VOL_SAMP_RES)*(VOL_DIV_RES+VOL_SAMP_RES)/VOL_BASE) /* ĸ�߻�׼��ѹУ�� */
#define ADC_ADJ           _IQ(MCU_VOL/(CURR_BASE*RES_SAM*OPA_AMP))	/* ADC���� */


/*============================================Protect Info==========================================================*/
#define TIMEMS(x)      (x)        		 /**MS**/
#define TIMES(x)       (x*1000)        /**S**/
#define TIMEMIN(x)     (60*TIMES(x))   /**MINNUTE**/
#define TIMEHOUR(x)    (60*TIMEMIN(x)) /**HOUR**/

#define PRO_VOL_UD						140		/* Ƿѹ����ֵ *0.1V */
#define PRO_VOL_UD_TIME				TIMES(1)  
#define PRO_VOL_OV						235		/* ��ѹ����ֵ *0.1V */
#define PRO_VOL_OV_TIME				TIMES(1)

#define PRO_STOP_THRESHOLD   (ADC_MAX*0.3)  /* STOP���µ���ֵ */
#define PRO_STOP_TIME				  TIMES(0.5)          /* STOP���µ�ʱ�� */

#define PRO_BOUNDARY_TIME			TIMES(1)		/* �߽�������ֵ */

#define PRO_CURR_CHARGE				20					/* ��������ֵ2A *0.1A */
#define PRO_CURR_CHARGE_TIME	TIMES(0.2)	/* ����������ʱ��*/

#define PRO_ROLL_ANGLE				((90-20)*(4096/90))	/* �����Ƕ�25 */
#define PRO_ROLL_TIME         TIMES(0.35)		      /* ����������ʱ��*/

/*============================================ADC GPIO Info==========================================================*/
#define ADC_MAX			(1<<12)
/* ������ҵ���*/
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
#define OFFTIMEDELAY 		1000/* �ػ���ʱʱ�� ms */

#define RANDOMMAX       180  /* ���ת������Ƕ�*/
#define RANDOMMMIN      20   /* ���ת�����С�Ƕ�*/\

#define TRACESPEED      2000    /* �߽����ٶ�*/
#define TURNSPEED       1000   /* ���ת���ٶ�*/
#define WALKSPEED       3500   /* ���ߵ��ٶ�*/

#define CUTSPEED        5000   /* ��ݵ��ٶ�*/


/*============================================MOWER WALK Info==========================================================*/
/*���س����˳ʱ�뻹����ʱ�� */
#define  ALONGFORWARD           0
#define  ALONGBACKWARD          1
#define  ALONG    ALONGFORWARD


/*�����㵹���� */
#define  UP        0  /**����ͷ���泯�߽��߸��� P->N**/
#define  DOWM      1  /**����ͷ���泯�߽������� N->P**/
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
#define RADIUS_WHEEL    (0.105)  /*���Ӱ뾶*/
#define RADIUS_MOWER    (0.14)/*����İ뾶*/

#define RATIO_OF_REDUCTION  100/*���ٱ�*/


#endif



