#ifndef __ERR_CODE_H_
#define __ERR_CODE_H_

/* 
����������淶��
1.�����������Ӧ�����л����ϴ����´�����룬����ɾ�����д��롣
2.�����������Ҫ��֤Ψһ�ԣ��������ڳ�����ͬһ��������ж����ڡ�
3.������������Ӧ��ʱ�ϴ�����������֤������뼰ʱˢ�£���ֹ����ͬʱ����ͬһ������롣
*/
enum ERR_CODE
{
	ERR_NO_CODE		 =   				0,
	ERR_OPA_OFFSET_A =                  10,		/* �˷Ų���ƫ��A���� */
	ERR_OPA_OFFSET_B =                  11,		/* �˷Ų���ƫ��B���� */
	ERR_OPA_OFFSET_C =                  12,		/* �˷Ų���ƫ��C���� */

	ERR_OC_FAULT     =                  20,		/* Ӳ������ */

	ERR_VOL_OV       =                  30,		/* ������ѹ */
	ERR_VOL_UD       =                  31,		/* ����Ƿѹ */


	ERR_TMP_OV		 =					40,		/* ���������� */
	ERR_TMP_DISCONT	 =					41,		/* �¶ȴ��������� */
	ERR_TMP_SHORT	 =					42,		/* �¶ȴ�������· */


	ERR_MOTOR_SPD_OV =					50,		/* ������� */
	ERR_MOTOR_BLOCK	 =					51,		/* �����ת */
	ERR_MOTOR_FLY	 =					52,		/* ����ɳ� */
	ERR_MOTOR_LACKPHASE_U =				53,		/* ����Լ�ȱ��U */
	ERR_MOTOR_LACKPHASE_V =				54,		/* ����Լ�ȱ��V */
	ERR_MOTOR_LACKPHASE_W =				55,		/* ����Լ�ȱ��W */
	ERR_MOTOR_TMP_OV	  =				57,		/* ����¶ȹ��� */
	ERR_MOTOR_TMP_DISCONT =				58,		/* ����¶ȴ��������� */
	ERR_MOTOR_TMP_SHORT	  =				59,		/* ����¶ȴ�������· */
	ERR_MOTOR_LOAD_OV	  =				60,		/* ������� */
	ERR_MOTOR_BRK_FAULT	  =			    61,		/* �����բ�쳣 */


	ERR_ENC_CRC_6813	  =				70,		/* ������У��δͨ�� */
	ERR_ENC_Z_SHIFT		  =				81,		/* �ϴ�Z���������ƫ�ƽϴ� */
	ERR_ENC_Z_DISTURB	  =				82,		/* Z���ܸ��� */
	ERR_ENC_Z_LOSS		  =				83,		/* Z�źŶ�ʧ */

	/* ������ ��ת��ѹ�� */
	ERR_ENC_RES_OFFSET_SIN =            100,    /* ����Sinƫ�ô��� */
	ERR_ENC_RES_OFFSET_COS =            101,    /* ����Cosƫ�ô��� */
	ERR_ENC_RES_DISC_SIN_0 =			102,	/* ����Sin���� */
	ERR_ENC_RES_DISC_COS_0 =			103,	/* ����Sin���� */
	ERR_ENC_RES_DISC_SIN_1 =			104,	/* ����Sin���� */
	ERR_ENC_RES_DISC_COS_1 =			105,	/* ����Sin���� */
	/* CAN������ش��� */
	ERR_CAN_COMM_LOSE	   =			131, 	/* ������ʧ */
	ERR_CAN_RX_FIFO_BUSY   =			132,  	/* ���չ��� */
	ERR_CAN_TX_FIFO_BUSY   =			133,  	/* ���͹��� */
	ERR_CAN_BUS_STOP	   =			134,  	/* ����֡�ﵽ�趨ֵ   ��������ֹͣ�         		*/

    ERR_HOME_TIME_OUT      =            140,    /* ���㸴�鳬ʱ */
	ERR_TIME_AD_ISR_OV     =			180,	/* AD�ж�������� */
	ERR_TIME_MAIN05MS_OV   =			181,	/* ��ѭ��0.5MsA������� */

	ERR_E2_WRITE		   =			201,	/* E2д���� */
	ERR_E2_READ			   =			202,	/* E2������ */
	ERR_E2_RW_TIME_OV	   =			203,	/* E2��д��ʱ */
	ERR_E2_WRITE_NUM_OV	   =			204,	/* E2дԽ�� */
	
	
	
  ERR_OUT_BOUNDARY	   =			211,	  /* ��ݻ�Խλ */
  ERR_STOP	           =			212,		/* ��ݻ�ͣ�� */	
  ERR_LIFT	           =			213,		/* ��̧�� */	
  ERR_ROLL	           =			214,		/* ������ */		
  ERROR_WALK_OVERLOAD  =			215,		/* ���ߵ������ */		
  ERROR_CUT_OVERLOAD   =			216,		/* ���ߵ������ */		
  ERROR_HARDWARE       =			217,		/* Ӳ������ */
  ERROR_OVERCHARGECURRENT       =			218,		/* ������ */	
	
};

#endif

