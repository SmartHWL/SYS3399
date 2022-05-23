#ifndef __ERR_CODE_H_
#define __ERR_CODE_H_

/* 
错误代码管理规范：
1.创建错误代码应在已有基础上创建新错误代码，不能删除现有代码。
2.创建错误代码要保证唯一性，即不能在程序中同一错误代码有多个入口。
3.创建完错误代码应及时上传服务器，保证错误代码及时刷新，防止多人同时创建同一错误代码。
*/
enum ERR_CODE
{
	ERR_NO_CODE		 =   				0,
	ERR_OPA_OFFSET_A =                  10,		/* 运放采样偏置A错误 */
	ERR_OPA_OFFSET_B =                  11,		/* 运放采样偏置B错误 */
	ERR_OPA_OFFSET_C =                  12,		/* 运放采样偏置C错误 */

	ERR_OC_FAULT     =                  20,		/* 硬件过流 */

	ERR_VOL_OV       =                  30,		/* 驱动过压 */
	ERR_VOL_UD       =                  31,		/* 驱动欠压 */


	ERR_TMP_OV		 =					40,		/* 驱动器过温 */
	ERR_TMP_DISCONT	 =					41,		/* 温度传感器断线 */
	ERR_TMP_SHORT	 =					42,		/* 温度传感器短路 */


	ERR_MOTOR_SPD_OV =					50,		/* 电机超速 */
	ERR_MOTOR_BLOCK	 =					51,		/* 电机堵转 */
	ERR_MOTOR_FLY	 =					52,		/* 电机飞车 */
	ERR_MOTOR_LACKPHASE_U =				53,		/* 电机自检缺相U */
	ERR_MOTOR_LACKPHASE_V =				54,		/* 电机自检缺相V */
	ERR_MOTOR_LACKPHASE_W =				55,		/* 电机自检缺相W */
	ERR_MOTOR_TMP_OV	  =				57,		/* 电机温度过高 */
	ERR_MOTOR_TMP_DISCONT =				58,		/* 电机温度传感器断线 */
	ERR_MOTOR_TMP_SHORT	  =				59,		/* 电机温度传感器短路 */
	ERR_MOTOR_LOAD_OV	  =				60,		/* 电机过载 */
	ERR_MOTOR_BRK_FAULT	  =			    61,		/* 电机抱闸异常 */


	ERR_ENC_CRC_6813	  =				70,		/* 编码器校验未通过 */
	ERR_ENC_Z_SHIFT		  =				81,		/* 上次Z脉冲与这次偏移较大 */
	ERR_ENC_Z_DISTURB	  =				82,		/* Z相受干扰 */
	ERR_ENC_Z_LOSS		  =				83,		/* Z信号丢失 */

	/* 编码器 旋转变压器 */
	ERR_ENC_RES_OFFSET_SIN =            100,    /* 旋变Sin偏置错误 */
	ERR_ENC_RES_OFFSET_COS =            101,    /* 旋变Cos偏置错误 */
	ERR_ENC_RES_DISC_SIN_0 =			102,	/* 旋变Sin断线 */
	ERR_ENC_RES_DISC_COS_0 =			103,	/* 旋变Sin断线 */
	ERR_ENC_RES_DISC_SIN_1 =			104,	/* 旋变Sin断线 */
	ERR_ENC_RES_DISC_COS_1 =			105,	/* 旋变Sin断线 */
	/* CAN总线相关错误 */
	ERR_CAN_COMM_LOSE	   =			131, 	/* 主机丢失 */
	ERR_CAN_RX_FIFO_BUSY   =			132,  	/* 接收过载 */
	ERR_CAN_TX_FIFO_BUSY   =			133,  	/* 发送过载 */
	ERR_CAN_BUS_STOP	   =			134,  	/* 错误帧达到设定值   引起总线停止活动         		*/

    ERR_HOME_TIME_OUT      =            140,    /* 回零复归超时 */
	ERR_TIME_AD_ISR_OV     =			180,	/* AD中断任务溢出 */
	ERR_TIME_MAIN05MS_OV   =			181,	/* 主循环0.5MsA任务溢出 */

	ERR_E2_WRITE		   =			201,	/* E2写错误 */
	ERR_E2_READ			   =			202,	/* E2读错误 */
	ERR_E2_RW_TIME_OV	   =			203,	/* E2读写超时 */
	ERR_E2_WRITE_NUM_OV	   =			204,	/* E2写越界 */
	
	
	
  ERR_OUT_BOUNDARY	   =			211,	  /* 割草机越位 */
  ERR_STOP	           =			212,		/* 割草机停机 */	
  ERR_LIFT	           =			213,		/* 被抬升 */	
  ERR_ROLL	           =			214,		/* 被侧倾 */		
  ERROR_WALK_OVERLOAD  =			215,		/* 行走电机过载 */		
  ERROR_CUT_OVERLOAD   =			216,		/* 行走电机过载 */		
  ERROR_HARDWARE       =			217,		/* 硬件故障 */
  ERROR_OVERCHARGECURRENT       =			218,		/* 充电过流 */	
	
};

#endif

