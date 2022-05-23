/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName: f_FuncCode.c 
 Author:     Version :   Date: 
 Sgk         1.0         2022年1月10日
 Description: 功能码参数表
 Version: // 版本信息
 History: 
 <author>    <time>          <version >   <desc> 
 Sgk         2022年1月10日       1.0          初版
***********************************************************/
#include "f_FuncCode.h"
#include "UserConfig.h"
//#include "SystemPU.h"



#define CTRL_MODE	 1
#define CTRL_RATO_DIR 1
#define CTRL_SPD_ACC_TIME 1
#define CTRL_SPD_DEC_TIME 1
#define BASE_RPM  1
#define CTRL_SPD_TRAJ 1
#define CTRL_ASR_OUT_MAX 1
#define CTRL_ASR_OUT_MIN 1
#define CTRL_ASR_KP 1
#define CTRL_ASR_TI 1
#define CTRL_APR_KP 1


/* 变量声明 */
FUNCCODE_ALL_UN unFuncCode;

//对每一组group，可操作的grade个数
const uint16_t funcCodeGradeAll[FUNCCODE_GROUP_NUM] =
{
	G00NUM - 0, G01NUM - 0, G02NUM - 0, G03NUM - 0, G04NUM - 0,		
	G05NUM - 0,	G06NUM - 0, G07NUM - 0, G08NUM - 0, G09NUM - 0,		
	G10NUM - 0, G11NUM - 0,	G12NUM - 0,											
};

// 每一组group的开始功能码的index 
// 对每一功能码 currentIndex = GetGradeIndex(group, grade) = funcCodeGradeSum[group]+grade;
const uint16_t funcCodeGradeSum[FUNCCODE_GROUP_NUM] =
{
//============================================
    GetCodeIndex(unFuncCode.Group.G00[0]),         // F0
    GetCodeIndex(unFuncCode.Group.G01[0]),         // F1
    GetCodeIndex(unFuncCode.Group.G02[0]),         // F2
    GetCodeIndex(unFuncCode.Group.G03[0]),         // F3

    GetCodeIndex(unFuncCode.Group.G04[0]),         // F4
    GetCodeIndex(unFuncCode.Group.G05[0]),         // F5
    GetCodeIndex(unFuncCode.Group.G06[0]),         // F6
    GetCodeIndex(unFuncCode.Group.G07[0]),         // F7

    GetCodeIndex(unFuncCode.Group.G08[0]),         // F8
    GetCodeIndex(unFuncCode.Group.G09[0]),         // F9
    GetCodeIndex(unFuncCode.Group.G10[0]),         // FA
    GetCodeIndex(unFuncCode.Group.G11[0]),         // FB
	GetCodeIndex(unFuncCode.Group.G12[0]),		   // FB

//============================================
	GetCodeIndex(unFuncCode.Group.gU0[0]),		 // FB

  
//============================================
};

// 功能码的属性表，上限、下限、属性。
// 不包括 (EEPROM_CHK、掉电记忆、)显示
// E2地址位0xffff 不储存 跳过
#if 1//FNUM_PARA
const FUNCCODE_ATTRIBUTE funcCodeAttribute[FNUM_PARA] =
{
	/* 下限			  上限 			 出厂值	    	属性 			EEPROM地址 	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 0 电机参数类 */ 
	{  0,			  65535,			0,			 0x0128,		100,		&unFuncCode.Code.Group0.u16ServMotorNo	}, 
	{  1,			  65535, 			0,			 0x012a,		101,		&unFuncCode.Code.Group0.u16MotorPower	},  
	{  1,			  65535,			0,			 0x0128,		102,		&unFuncCode.Code.Group0.u16MotorVoltage	}, 
	{  1,			  65535, 			0,			 0x012a,		103,		&unFuncCode.Code.Group0.u16MotorCurrent	}, 
	{  100,			  6000 ,			0,			 0x0120, 		104,		&unFuncCode.Code.Group0.u16MotorSpeed	}, 
	{  1,			  65535,			0,			 0x012a, 		105,		&unFuncCode.Code.Group0.u16MotorTrq		}, 
	{  1,			  65535,			0,			 0x012a, 		106,		&unFuncCode.Code.Group0.u16MotorJn		}, 
	{  1,			  100  ,			0,			 0x0118, 		107,		&unFuncCode.Code.Group0.u16MotorPoles	}, 
	{  1,			  65535,			0,			 0x012b, 		108,		&unFuncCode.Code.Group0.u16MotorRs		}, 
	{  1,			  65535,			0,			 0x012a, 		109,		&unFuncCode.Code.Group0.u16MotorLd		}, 
	{  1,			  65535,			0,			 0x012a, 		110,		&unFuncCode.Code.Group0.u16MotorLq		}, 
	{  1,			  65535,			0,			 0x012a, 		111,		&unFuncCode.Code.Group0.u16MotorBemfCoeff}, 
	{  1,			  65535,			0,			 0x012a, 		112,		&unFuncCode.Code.Group0.u16MotorKt		}, 
	{  1,			  65535,			0,			 0x012a, 		113,		&unFuncCode.Code.Group0.u16MotorTe		}, 
	{  1,			  65535,			0,			 0, 			114,		&unFuncCode.Code.Group0.u16MotorTm		}, 
	{  0,			  65535,			0,			 0, 			115,		&unFuncCode.Code.Group0.u16EncType		}, 
	{  0,			  65535,			0,			 0, 			116,		&unFuncCode.Code.Group0.u16AbsEncOffSetH}, 
	{  0,			  65535,			0,			 0, 			117,		&unFuncCode.Code.Group0.u16AbsEncOffSetL}, 
	{  0,			  65535,			0,			 0, 			118,		&unFuncCode.Code.Group0.u16EncLineNumH	}, 
	{  0,			  65535,			0,			 0, 			119,		&unFuncCode.Code.Group0.u16EncLineNumL	}, 		
	{  0,			  3600 ,			0,			 0, 			120,		&unFuncCode.Code.Group0.u16ThetaZ		}, 
	{  0,			  3600 ,			0,			 0, 			121,		&unFuncCode.Code.Group0.u16ThetaU   	}, 
    {  0,			     0 ,			0,			 0, 			122,		&unFuncCode.Code.Group0.u16Rsd[0]   	}, 
    {  0,			     0 ,			0,			 0, 			123,		&unFuncCode.Code.Group0.u16Rsd[1]   	}, 
    {  0,			     0 ,			0,			 0, 			124,		&unFuncCode.Code.Group0.u16Rsd[2]   	}, 
    {  0,			     0 ,			0,			 0, 			125,		&unFuncCode.Code.Group0.u16Rsd[3]   	}, 
    {  0,			     0 ,			0,			 0, 			126,		&unFuncCode.Code.Group0.u16Rsd[4]   	}, 
    {  0,			     0 ,			0,			 0, 			127,		&unFuncCode.Code.Group0.u16Rsd[5]   	}, 
    {  0,			     0 ,			0,			 0, 			128,		&unFuncCode.Code.Group0.u16Rsd[6]   	}, 
    {  0,			     0 ,			0,			 0, 			129,		&unFuncCode.Code.Group0.u16Rsd[7]   	}, 
    {  0,			     0 ,			0,			 0, 			130,		&unFuncCode.Code.Group0.u16Rsd[8]   	}, 
    {  0,			     0 ,			0,			 0, 			131,		&unFuncCode.Code.Group0.u16Rsd[9]   	}, 

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 1 伺服控制器类 */ 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16FuncSoftVersion}, 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16FuncSoftRevision}, 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16CoreSoftVersion}, 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16CoreSoftRevision}, 
	{  0,			  65535,			0,			 0, 		0xffff,		&unFuncCode.Code.Group1.u16CpldVersion	  }, 
	{  0,			  65535,			0,			 0, 		0xffff,		&unFuncCode.Code.Group1.u16ServoType	  }, 
	{  0,			  65535,			0,			 0, 		0xffff,		&unFuncCode.Code.Group1.u16CanOpenVersion }, 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16CustomSoftVersion  }, 
	{  0,			  65535,			0,			 0x022A, 	0xffff,		&unFuncCode.Code.Group1.u16CustomSoftRevision }, 
	/* Group 2 基本控制器类 */ 
	{  0,                 6,	CTRL_MODE,			 0, 			132,	&unFuncCode.Code.Group2.u16CtrlMode       }, 
	{  0,                 2,	CTRL_RATO_DIR,		 0, 			133,	&unFuncCode.Code.Group2.u16RatoDir	 	  }, 
	{  0,			  1    ,			0,			 0, 			134,	&unFuncCode.Code.Group2.u16PlsOutDir	  }, 
	{  0,			  5000 ,		   50,			 0, 			135, 	&unFuncCode.Code.Group2.u16SpdDispLpfTime }, 
	{  0,			  1    ,		    1,			 0, 			136, 	&unFuncCode.Code.Group2.u16E2PowerOffSave }, 	
	{  0,			  255  ,		   25,			 0, 			137, 	&unFuncCode.Code.Group2.u16AbzFilterTime  }, 
    {  0,			    0  ,		   0,			 0, 			138, 	&unFuncCode.Code.Group2.u16Rsd[0]  }, 
    {  0,			    0  ,		   0,			 0, 			140, 	&unFuncCode.Code.Group2.u16Rsd[1]  }, 
    {  0,			    0  ,		   0,			 0, 			141, 	&unFuncCode.Code.Group2.u16Rsd[2]  }, 
    {  0,			    0  ,		   0,			 0, 			142, 	&unFuncCode.Code.Group2.u16Rsd[3]  }, 
    {  0,			    0  ,		   0,			 0, 			143, 	&unFuncCode.Code.Group2.u16Rsd[4]  }, 
    {  0,			    0  ,		   0,			 0, 			144, 	&unFuncCode.Code.Group2.u16Rsd[5]  }, 
    {  0,			    0  ,		   0,			 0, 			145, 	&unFuncCode.Code.Group2.u16Rsd[6]  }, 
    {  0,			    0  ,		   0,			 0, 			146, 	&unFuncCode.Code.Group2.u16Rsd[7]  }, 
    {  0,			    0  ,		   0,			 0, 			147, 	&unFuncCode.Code.Group2.u16Rsd[8]  }, 
    {  0,			    0  ,		   0,			 0, 			148, 	&unFuncCode.Code.Group2.u16Rsd[9]  }, 
    {  0,			    0  ,		   0,			 0, 			149, 	&unFuncCode.Code.Group2.u16Rsd[10]  }, 
    {  0,			    0  ,		   0,			 0, 			150, 	&unFuncCode.Code.Group2.u16Rsd[11]  }, 
    {  0,			    0  ,		   0,			 0, 			151, 	&unFuncCode.Code.Group2.u16Rsd[12]  }, 
    {  0,			    0  ,		   0,			 0, 			152, 	&unFuncCode.Code.Group2.u16Rsd[13]  }, 
    {  0,			    0  ,		   0,			 0, 			153, 	&unFuncCode.Code.Group2.u16Rsd[14]  }, 
    {  0,			    0  ,		   0,			 0, 			154, 	&unFuncCode.Code.Group2.u16Rsd[15]  }, 
    {  0,			    0  ,		   0,			 0, 			155, 	&unFuncCode.Code.Group2.u16Rsd[16]  }, 
    {  0,			    0  ,		   0,			 0, 			156, 	&unFuncCode.Code.Group2.u16Rsd[17]  }, 
    {  0,			    0  ,		   0,			 0, 			157, 	&unFuncCode.Code.Group2.u16Rsd[18]  }, 
    {  0,			    0  ,		   0,			 0, 			158, 	&unFuncCode.Code.Group2.u16Rsd[19]  }, 
    {  0,			    0  ,		   0,			 0, 			159, 	&unFuncCode.Code.Group2.u16Rsd[20]  }, 
    {  0,			    0  ,		   0,			 0, 			160, 	&unFuncCode.Code.Group2.u16Rsd[21]  }, 
    {  0,			    0  ,		   0,			 0, 			161, 	&unFuncCode.Code.Group2.u16Rsd[22]  }, 
    {  0,			    0  ,		   0,			 0, 			162, 	&unFuncCode.Code.Group2.u16Rsd[23]  }, 
    {  0,			    0  ,		   0,			 0, 			163, 	&unFuncCode.Code.Group2.u16Rsd[24]  }, 

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 3 端子输入参数 */ 
	{  0,			  65535,		    0,		 0x0028, 			164, 	&unFuncCode.Code.Group3.u16FuncPowerOnDI[0] }, 
	{  0,			  65535,			0,		 0x0028,		    165, 	&unFuncCode.Code.Group3.u16FuncPowerOnDI[1] }, 
	{  0,			  65535,			0,		 0x0028,		    166, 	&unFuncCode.Code.Group3.u16FuncPowerOnDI[2] }, 
	{  0,			  65535,			0,		 0x0028,		    166, 	&unFuncCode.Code.Group3.u16FuncPowerOnDI[3] }, 
	{  0,	            30,			    0,		 0x0128,		    167, 	&unFuncCode.Code.Group3.DI[0].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    168, 	&unFuncCode.Code.Group3.DI[0].u16Polariy  	}, 
	{  0,	 		  65535,			0,		 0x0128,		    169, 	&unFuncCode.Code.Group3.DI[0].u16FilterTime }, 
	{  0,	            30,			    0,		 0x0128,		    170, 	&unFuncCode.Code.Group3.DI[1].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    171, 	&unFuncCode.Code.Group3.DI[1].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    172, 	&unFuncCode.Code.Group3.DI[1].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    173, 	&unFuncCode.Code.Group3.DI[2].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    174, 	&unFuncCode.Code.Group3.DI[2].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    175, 	&unFuncCode.Code.Group3.DI[2].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    176, 	&unFuncCode.Code.Group3.DI[3].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    177, 	&unFuncCode.Code.Group3.DI[3].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    178, 	&unFuncCode.Code.Group3.DI[3].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    179, 	&unFuncCode.Code.Group3.DI[4].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    180, 	&unFuncCode.Code.Group3.DI[4].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    181, 	&unFuncCode.Code.Group3.DI[4].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    182, 	&unFuncCode.Code.Group3.DI[5].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    183, 	&unFuncCode.Code.Group3.DI[5].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    184, 	&unFuncCode.Code.Group3.DI[5].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    185, 	&unFuncCode.Code.Group3.DI[6].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    186, 	&unFuncCode.Code.Group3.DI[6].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    187, 	&unFuncCode.Code.Group3.DI[6].u16FilterTime },
	{  0,	            30,			    0,		 0x0128,		    188, 	&unFuncCode.Code.Group3.DI[7].u16Function	}, 
	{  0,               4,	            0,		 0x0128,		    189, 	&unFuncCode.Code.Group3.DI[7].u16Polariy	}, 
	{  0,			  65535,			0,		 0x0128,		    190, 	&unFuncCode.Code.Group3.DI[7].u16FilterTime },
	{(uint16_t)-5000,  5000,			0,		 0x0428,		    191, 	&unFuncCode.Code.Group3.AI[0].u16Offset		}, 
	{  0,			  65535,			0,		 0x0028,		    192, 	&unFuncCode.Code.Group3.AI[0].u16FilterTime	}, 
	{  0,			  10000,			0,		 0x0029,		    193, 	&unFuncCode.Code.Group3.AI[0].u16DeadZone 	},
	{(uint16_t)-5000,  5000,			0,		 0x0429,		    194, 	&unFuncCode.Code.Group3.AI[0].u16ZeroDrift	},
	{(uint16_t)-5000,  5000,			0,		 0x0428,		    195, 	&unFuncCode.Code.Group3.AI[1].u16Offset		}, 
	{  0,			  65535,			0,		 0x0028,		    196, 	&unFuncCode.Code.Group3.AI[1].u16FilterTime	}, 
	{  0,			  10000,			0,		 0x0029,		    197, 	&unFuncCode.Code.Group3.AI[1].u16DeadZone 	},
	{(uint16_t)-5000,  5000,			0,		 0x0429,		    198, 	&unFuncCode.Code.Group3.AI[1].u16ZeroDrift	},
	{  0,  			   9000,		 3000,		 0x0028,		    199, 	&unFuncCode.Code.Group3.u16SpdGainAI		},
	{100,  			    800,		 100,		 0x002A,		    200, 	&unFuncCode.Code.Group3.u16CurGainAI	},
	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 4 端子输出参数 */ 
	{ 0,	            17, 		    0,		 0x0128,		    201, 	&unFuncCode.Code.Group4.DO[0].u16Function	},
	{ 0,                1,              0,		 0x0128,		    202, 	&unFuncCode.Code.Group4.DO[0].u16Polariy	},
	{ 0,	            17, 		    0,		 0x0128,		    203, 	&unFuncCode.Code.Group4.DO[1].u16Function	},
	{ 0,                1,              0,		 0x0128,		    204, 	&unFuncCode.Code.Group4.DO[1].u16Polariy	},
	{ 0,	            17, 		    0,		 0x0128,		    205, 	&unFuncCode.Code.Group4.DO[2].u16Function	},
	{ 0,                1,              0,		 0x0128,		    206, 	&unFuncCode.Code.Group4.DO[2].u16Polariy	},
	{ 0,	            17, 		    0,		 0x0128,		    207, 	&unFuncCode.Code.Group4.DO[3].u16Function	},
	{ 0,                1,              0,		 0x0128,		    208, 	&unFuncCode.Code.Group4.DO[3].u16Polariy	},
				
	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 5 位置控制参数 */ 
	{  0,			  1    ,			0,			 0, 			209,		&unFuncCode.Code.Group5.u16PosRefSrc	  	}, 
	{  0,			  65535,			0,			 0, 			210,		&unFuncCode.Code.Group5.u16PosRefLpfTime  	}, 
	{  0,			  1280,			    0,			 0, 			211,		&unFuncCode.Code.Group5.u16PosRefFirTime  }, 
	{  1,			  16384,			0,			 0, 			212,		&unFuncCode.Code.Group5.u16GearNumerator1High}, 
	{  1,			  65535,			1,			 0, 			213,		&unFuncCode.Code.Group5.u16GearNumerator1Low}, 
	{  1,			  16384,			0,			 0, 			214,		&unFuncCode.Code.Group5.u16GearDenominator1High}, 
	{  1,			  65535,			1,			 0, 			215,		&unFuncCode.Code.Group5.u16GearDenominator1Low}, 
	{  1,			  16384,			0,			 0, 			216,		&unFuncCode.Code.Group5.u16GearNumerator2High}, 
	{  1,			  65535,			1,			 0, 			217,		&unFuncCode.Code.Group5.u16GearNumerator2Low}, 
	{  1,			  16384,			0,			 0, 			218,		&unFuncCode.Code.Group5.u16GearDenominator2High}, 
	{  1,			  65535,			1,			 0, 			219,		&unFuncCode.Code.Group5.u16GearDenominator2Low}, 
	{  0,			  1    ,			0,			 0, 			220,		&unFuncCode.Code.Group5.u16GearChgCond	  }, 
	{  0,			  1    ,			0,			 0, 			221,		&unFuncCode.Code.Group5.u16PlsInMode	  }, 
	{  0,			  1    ,			0,			 0, 			222,		&unFuncCode.Code.Group5.u16PosErrClrMode  }, 
	{  0,			  1    ,			0,			 0, 			223,		&unFuncCode.Code.Group5.u16EncPlsDivFre   }, 
	{  0,			  1    ,			0,			 0, 			224,		&unFuncCode.Code.Group5.u16PosArriveCond  }, 
	{  0,			  1    ,			0,			 0, 			225,		&unFuncCode.Code.Group5.u16PosArriveThreshold}, 
	{  0,			  1    ,			0,			 0, 			226,		&unFuncCode.Code.Group5.u16PosNearThreshold},

	
	{  0,			  6    ,			0,			 0, 		    227,		&unFuncCode.Code.Group5.u16HomingEnable	 	},
	{  0,			  9    ,			0,			 0, 		    228,		&unFuncCode.Code.Group5.u16HomingMode		},
	{  0,			  3000 ,		  100,			 0, 		    229,		&unFuncCode.Code.Group5.u16HomingHighSpeed	},
	{  0,			  1000 ,		   10,			 0, 		    230,		&unFuncCode.Code.Group5.u16HomingLowSpeed	},
	{  0,			  1000 ,		 1000,			 0, 		    231,		&unFuncCode.Code.Group5.u16HomingAccDecTime	},
	{  0,			  65535,		10000,			 0, 		    232,		&unFuncCode.Code.Group5.u16HomingTime		},
	{(uint16_t)-16384,16384,			0,			 0, 		    233,		&unFuncCode.Code.Group5.u16HomePosOffsetH	},
	{  0,			  65535,			0,			 0, 		    234,		&unFuncCode.Code.Group5.u16HomePosOffsetL	},
    {  0,			      0,			0,			 0, 		    235,		&unFuncCode.Code.Group5.u16Rsd[0]	},
    {  0,			      0,			0,			 0, 		    236,		&unFuncCode.Code.Group5.u16Rsd[1]	},
    {  0,			      0,			0,			 0, 		    237,		&unFuncCode.Code.Group5.u16Rsd[2]	},
    {  0,			      0,			0,			 0, 		    238,		&unFuncCode.Code.Group5.u16Rsd[3]	},
    {  0,			      0,			0,			 0, 		    239,		&unFuncCode.Code.Group5.u16Rsd[4]	},
    {  0,			      0,			0,			 0, 		    240,		&unFuncCode.Code.Group5.u16Rsd[5]	},
    {  0,			      0,			0,			 0, 		    241,		&unFuncCode.Code.Group5.u16Rsd[6]	},
    {  0,			      0,			0,			 0, 		    242,		&unFuncCode.Code.Group5.u16Rsd[7]	},
    {  0,			      0,			0,			 0, 		    243,		&unFuncCode.Code.Group5.u16Rsd[8]	},
    {  0,			      0,			0,			 0, 		    244,		&unFuncCode.Code.Group5.u16Rsd[9]	},
    {  0,			      0,			0,			 0, 		    245,		&unFuncCode.Code.Group5.u16Rsd[10]	},
    {  0,			      0,			0,			 0, 		    246,		&unFuncCode.Code.Group5.u16Rsd[11]	},
    {  0,			      0,			0,			 0, 		    247,		&unFuncCode.Code.Group5.u16Rsd[12]	},
    {  0,			      0,			0,			 0, 		    248,		&unFuncCode.Code.Group5.u16Rsd[13]	},
    {  0,			      0,			0,			 0, 		    249,		&unFuncCode.Code.Group5.u16Rsd[14]	},
    {  0,			      0,			0,			 0, 		    250,		&unFuncCode.Code.Group5.u16Rsd[15]	},
    {  0,			      0,			0,			 0, 		    251,		&unFuncCode.Code.Group5.u16Rsd[16]	},
    {  0,			      0,			0,			 0, 		    252,		&unFuncCode.Code.Group5.u16Rsd[17]	},
    {  0,			      0,			0,			 0, 		    253,		&unFuncCode.Code.Group5.u16Rsd[18]	},
    {  0,			      0,			0,			 0, 		    254,		&unFuncCode.Code.Group5.u16Rsd[19]	},
 

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 6 速度控制参数 */ 	
	{  0,             2    ,            0,      0x0128,            255,     &unFuncCode.Code.Group6.u16SpdXSrc        },	
	{  0,             2    ,            0,      0x0128,            256,     &unFuncCode.Code.Group6.u16SpdYSrc        },
	{  0, 			  2	   ,			0,		0x0128,			   257,	    &unFuncCode.Code.Group6.u16SpdRefSrc	  },	
	{(uint16_t)-3000, 3000 ,		    0,		0x0428,		       258,	    &unFuncCode.Code.Group6.u16SpdPreset	  },	
	{  0, 			  3000 ,		  100,		0x0028,			   259,	    &unFuncCode.Code.Group6.u16SpdJogSet	  },	
	{  0, 			  65535,CTRL_SPD_ACC_TIME,	0x0028,			   260,	    &unFuncCode.Code.Group6.u16SpdAccTime	  },
	{  0, 			  65535,CTRL_SPD_DEC_TIME,	0x0028,			   261,	    &unFuncCode.Code.Group6.u16SpdDecTime	  },
	{  0, 			  6000 ,	 BASE_RPM,		0x0028, 		   262,		&unFuncCode.Code.Group6.u16SpdMaxLmt	  },	
	{  0, 			  6000 ,     BASE_RPM,		0x0028, 		   263,		&unFuncCode.Code.Group6.u16SpdFwdLmt	  },	
	{  0, 			  6000 ,	 BASE_RPM,		0x0028, 		   264,		&unFuncCode.Code.Group6.u16SpdRevLmt	  },	
	{  0, 			  3000 ,		   10,		0x0028, 		   265,		&unFuncCode.Code.Group6.u16PosZerSpdThreshold},	
	{  0, 			  1000 ,		   20,		0x0028, 		   266,		&unFuncCode.Code.Group6.u16SpdRatoThreshold}, 	
	{  0, 			  100  ,		   10,		0x0028, 		   267,		&unFuncCode.Code.Group6.u16SpdSameThreshold}, 	
	{ 10, 			  3000 ,		   10,		0x0028, 		   268,		&unFuncCode.Code.Group6.u16SpdArriveThreshold}, 	
	{  1, 			  3000 ,		   10,		0x0028, 		   269,		&unFuncCode.Code.Group6.u16SpdZeroThreshold}, 
  	{  0, 			     5 ,CTRL_SPD_TRAJ,		0x0028, 		   270,		&unFuncCode.Code.Group6.u16SpdTrajType}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   271,		&unFuncCode.Code.Group6.u16Rsd[0]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   272,		&unFuncCode.Code.Group6.u16Rsd[1]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   273,		&unFuncCode.Code.Group6.u16Rsd[2]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   274,		&unFuncCode.Code.Group6.u16Rsd[3]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   275,		&unFuncCode.Code.Group6.u16Rsd[4]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   276,		&unFuncCode.Code.Group6.u16Rsd[5]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   277,		&unFuncCode.Code.Group6.u16Rsd[6]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   278,		&unFuncCode.Code.Group6.u16Rsd[7]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   279,		&unFuncCode.Code.Group6.u16Rsd[8]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   280,		&unFuncCode.Code.Group6.u16Rsd[9]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   281,		&unFuncCode.Code.Group6.u16Rsd[10]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   282,		&unFuncCode.Code.Group6.u16Rsd[11]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   283,		&unFuncCode.Code.Group6.u16Rsd[12]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   284,		&unFuncCode.Code.Group6.u16Rsd[13]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   285,		&unFuncCode.Code.Group6.u16Rsd[14]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   286,		&unFuncCode.Code.Group6.u16Rsd[15]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   287,		&unFuncCode.Code.Group6.u16Rsd[16]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   288,		&unFuncCode.Code.Group6.u16Rsd[17]}, 
  	{  0, 			     0 ,		    0,		0x0028, 		   289,		&unFuncCode.Code.Group6.u16Rsd[18]}, 


	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 7 转矩控制参数 */		
	{  0, 			  2    ,		   0,			 0, 		   290,		&unFuncCode.Code.Group7.u16TrqXSrc         }, 	
	{  0, 			  2    ,		   1,			 0, 		   291,		&unFuncCode.Code.Group7.u16TrqYSrc         }, 	
	{  0, 			  4    ,		   0,			 0, 		   292,		&unFuncCode.Code.Group7.u16TrqRefSrc       }, 	
	{(uint16_t)-3000, 3000 ,		   0,			 0, 		   293,		&unFuncCode.Code.Group7.u16TrqPreset       }, 	
	{  0,             3000 ,		  79,			 0, 		   294,		&unFuncCode.Code.Group7.u16TrqRefLpfTime   }, 	
	{  0,             4    ,		   0,			 0, 		   295,		&unFuncCode.Code.Group7.u16TrqLmtSrc       }, 	
	{  0,             2    ,		   1,			 0, 		   296,		&unFuncCode.Code.Group7.u16TrqAiLmtSrc     }, 	
	{  0,             3000 ,		CTRL_ASR_OUT_MAX,0, 	       297,		&unFuncCode.Code.Group7.u16TrqLmtInsideP   }, 
	{  0,			  3000 ,		CTRL_ASR_OUT_MIN,0, 	       298,		&unFuncCode.Code.Group7.u16TrqLmtInsideN   }, 
	{  0,			  3000 ,		3000,			 0, 		   299,		&unFuncCode.Code.Group7.u16TrqLmtOutsideP  }, 
	{  0,			  3000 ,		3000,			 0, 		   300,		&unFuncCode.Code.Group7.u16TrqLmtOutsideN  }, 	
	{  0,			  2    ,		   0,			 0, 		   301,		&unFuncCode.Code.Group7.u16TrqSpdLmtSrc    }, 	
	{  0,			  2    ,		   1,			 0, 		   302,		&unFuncCode.Code.Group7.u16TrqSpdAiLmtSrc  }, 
	{  0,			  3000 ,		3000,			 0, 		   303,		&unFuncCode.Code.Group7.u16TrqSpdLmtFwd    }, 	
	{  0,			  3000 ,	    3000,			 0, 		   304,		&unFuncCode.Code.Group7.u16TrqSpdLmtRev    }, 
	{  0,			  3000 , 	     200,			 0, 		   305,		&unFuncCode.Code.Group7.u16TrqArrThreshold }, 	
	{  0,			  3000 , 	     100,			 0, 		   306,		&unFuncCode.Code.Group7.u16TrqNotArrThreshold}, 
	{  5,			  300  , 	      10,			 0, 		   307,		&unFuncCode.Code.Group7.u16TrqSpdLmtTimeWindow}, 
    {  0,			    0  , 	      0,			 0, 		   308,		&unFuncCode.Code.Group7.u16Rsd[0]}, 
    {  0,			    0  , 	      0,			 0, 		   309,		&unFuncCode.Code.Group7.u16Rsd[1]}, 
    {  0,			    0  , 	      0,			 0, 		   310,		&unFuncCode.Code.Group7.u16Rsd[2]}, 
    {  0,			    0  , 	      0,			 0, 		   311,		&unFuncCode.Code.Group7.u16Rsd[3]}, 
    {  0,			    0  , 	      0,			 0, 		   312,		&unFuncCode.Code.Group7.u16Rsd[4]}, 
    {  0,			    0  , 	      0,			 0, 		   313,		&unFuncCode.Code.Group7.u16Rsd[5]}, 
    {  0,			    0  , 	      0,			 0, 		   314,		&unFuncCode.Code.Group7.u16Rsd[6]}, 
    {  0,			    0  , 	      0,			 0, 		   315,		&unFuncCode.Code.Group7.u16Rsd[7]}, 
    {  0,			    0  , 	      0,			 0, 		   316,		&unFuncCode.Code.Group7.u16Rsd[8]}, 

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 8 增益类参数 */		
	#if defined(Tn) && defined(Te) && defined(Jn) && defined(Jx)
	{  1,			  20000, 	    1000,			 0, 		   317,		&unFuncCode.Code.Group8.u16AsrKp1          }, 
	{ 15,			  51200,		 200,			 0, 		   318,		&unFuncCode.Code.Group8.u16AsrTi1		   }, 
	#else
	{  1,			  20000, (CTRL_ASR_KP*100),		 0, 		   317,		&unFuncCode.Code.Group8.u16AsrKp1          }, 
	{ 15,			  51200, (CTRL_ASR_TI*10000),	 0, 		   318,		&unFuncCode.Code.Group8.u16AsrTi1		   }, 
	#endif
	{  0,			  20000,	CTRL_APR_KP,		 0, 		   319,		&unFuncCode.Code.Group8.u16AprKp1		   }, 
	{  1,			  20000, 	    1000,			 0, 		   320,		&unFuncCode.Code.Group8.u16AsrKp2          }, 
	{ 15,			  51200,		 200,			 0, 		   321,		&unFuncCode.Code.Group8.u16AsrTi2		   }, 
	{  0,			  20000,		 400,			 0, 		   322,		&unFuncCode.Code.Group8.u16AprKp2		   }, 	
	{  0,			     10,		   0,			 0, 		   323,		&unFuncCode.Code.Group8.u16GainChgCond	   },	
	{  0,			  10000,		  50,			 0, 		   324,		&unFuncCode.Code.Group8.u16GainChgDelay	   },	
	{  0,			  20000,		  50,			 0, 		   325,		&unFuncCode.Code.Group8.u16GainChgRank	   },	
	{  0,			  20000,		  30,			 0, 		   326,		&unFuncCode.Code.Group8.u16GainChgHystLoop },
	{  0,			  10000,		  30,			 0, 		   327,		&unFuncCode.Code.Group8.u16AprKpSwitchTime },
	{  0,			  12000,		  0,			 0, 		   328,		&unFuncCode.Code.Group8.u16Jx              },	
	{  0,			   6400,		 50,			 0, 		   329,		&unFuncCode.Code.Group8.u16SpdFfdFilterTime},
	{  0,			   1000,		  0,			 0, 		   330,		&unFuncCode.Code.Group8.u16SpdFfdGain	   },		
	{  0,			   6400,		 50,			 0, 		   331,		&unFuncCode.Code.Group8.u16TrqFfdFilterTime},		
	{  0,			   2000,		  0,			 0, 		   332,		&unFuncCode.Code.Group8.u16TrqFfdGain      },	
	{  0,			      5,		  0,			 0, 		   333,		&unFuncCode.Code.Group8.u16SpdFbkFirFltCnt },	
	{100,			   4000,	   4000,			 0, 		   334,		&unFuncCode.Code.Group8.u16SpdFbkLpfFc     },	
	{  0,			   1000,	   1000,			 0, 		   335,		&unFuncCode.Code.Group8.u16SpdKpdffCoeff   },
    {  0,			      0,	      0,			 0, 		   336,		&unFuncCode.Code.Group8.u16Rsd[0]   },
    {  0,			      0,	      0,			 0, 		   337,		&unFuncCode.Code.Group8.u16Rsd[1]   },
    {  0,			      0,	      0,			 0, 		   338,		&unFuncCode.Code.Group8.u16Rsd[2]   },
    {  0,			      0,	      0,			 0, 		   339,		&unFuncCode.Code.Group8.u16Rsd[3]   },
    {  0,			      0,	      0,			 0, 		   340,		&unFuncCode.Code.Group8.u16Rsd[4]   },
    {  0,			      0,	      0,			 0, 		   341,		&unFuncCode.Code.Group8.u16Rsd[5]   },
    {  0,			      0,	      0,			 0, 		   342,		&unFuncCode.Code.Group8.u16Rsd[6]   },
    {  0,			      0,	      0,			 0, 		   343,		&unFuncCode.Code.Group8.u16Rsd[7]   },
    {  0,			      0,	      0,			 0, 		   344,		&unFuncCode.Code.Group8.u16Rsd[8]   },
    {  0,			      0,	      0,			 0, 		   345,		&unFuncCode.Code.Group8.u16Rsd[9]   },

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 9  */
	{  0,			   4000,	   	  0,			 0, 		   346,		&unFuncCode.Code.Group9.NotchFilter[0].u16NotchFre},
	{  0,			     20,		  2,			 0, 		   347, 	&unFuncCode.Code.Group9.NotchFilter[0].u16WidthRank},
	{  0,				 99,		  0,			 0, 		   348, 	&unFuncCode.Code.Group9.NotchFilter[0].u16DepthRank},
	{  0,			   4000,		  0,			 0, 		   349, 	&unFuncCode.Code.Group9.NotchFilter[1].u16NotchFre},
	{  0,				 20,		  2,			 0, 		   350, 	&unFuncCode.Code.Group9.NotchFilter[1].u16WidthRank},
	{  0,				 99,		  0,			 0, 		   351, 	&unFuncCode.Code.Group9.NotchFilter[1].u16DepthRank},
	{  0,			   4000,		  0,			 0, 		   352, 	&unFuncCode.Code.Group9.NotchFilter[2].u16NotchFre},
	{  0,				 20,		  2,			 0, 		   353, 	&unFuncCode.Code.Group9.NotchFilter[2].u16WidthRank},
	{  0,				 99,		  0,			 0, 		   354, 	&unFuncCode.Code.Group9.NotchFilter[2].u16DepthRank},
	{  0,			   4000,		  0,			 0, 		   355, 	&unFuncCode.Code.Group9.NotchFilter[3].u16NotchFre},
	{  0,				 20,		  2,			 0, 		   356, 	&unFuncCode.Code.Group9.NotchFilter[3].u16WidthRank},
	{  0,				 99,		  0,			 0, 		   357, 	&unFuncCode.Code.Group9.NotchFilter[4].u16DepthRank},
    {  0,				 0,		      0,			 0, 		   358, 	&unFuncCode.Code.Group9.u16Rsd[0]},
    {  0,				 0,		 	  0,			 0, 		   359, 	&unFuncCode.Code.Group9.u16Rsd[1]},
    {  0,				 0,		  	  0,			 0, 		   360, 	&unFuncCode.Code.Group9.u16Rsd[2]},
    {  0,				 0,		  	  0,			 0, 		   361, 	&unFuncCode.Code.Group9.u16Rsd[3]},
    {  0,				 0,		  	  0,			 0, 		   362, 	&unFuncCode.Code.Group9.u16Rsd[4]},
    {  0,				 0,		      0,			 0, 		   363, 	&unFuncCode.Code.Group9.u16Rsd[5]},
    {  0,				 0,		  	  0,			 0, 		   364, 	&unFuncCode.Code.Group9.u16Rsd[6]},
    {  0,				 0,		  	  0,			 0, 		   365, 	&unFuncCode.Code.Group9.u16Rsd[7]},
    {  0,				 0,		  	  0,			 0, 		   366, 	&unFuncCode.Code.Group9.u16Rsd[8]},
    {  0,				 0,		  	  0,			 0, 		   367, 	&unFuncCode.Code.Group9.u16Rsd[9]},
    {  0,				 0,		      0,			 0, 		   368, 	&unFuncCode.Code.Group9.u16Rsd[10]},
    {  0,				 0,		  	  0,			 0, 		   369, 	&unFuncCode.Code.Group9.u16Rsd[11]},
    {  0,				 0,		  	  0,			 0, 		   370, 	&unFuncCode.Code.Group9.u16Rsd[12]},
    {  0,				 0,		  	  0,			 0, 		   371, 	&unFuncCode.Code.Group9.u16Rsd[13]},
    {  0,				 0,		  	  0,			 0, 		   372, 	&unFuncCode.Code.Group9.u16Rsd[14]},
    {  0,				 0,		  	  0,			 0, 		   373, 	&unFuncCode.Code.Group9.u16Rsd[15]},
    {  0,				 0,		  	  0,			 0, 		   374, 	&unFuncCode.Code.Group9.u16Rsd[16]},
    {  0,				 0,		  	  0,			 0, 		   375, 	&unFuncCode.Code.Group9.u16Rsd[17]},
    {  0,				 0,		  	  0,			 0, 		   376, 	&unFuncCode.Code.Group9.u16Rsd[18]},
    {  0,				 0,		  	  0,			 0, 		   377, 	&unFuncCode.Code.Group9.u16Rsd[19]},
 

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 10	*/
	{  0,				200,		  1,			 0, 		   378, 	&unFuncCode.Code.Group10.u16CommSlaveAddr	},
	{  0,				  5,		  5,			 0, 		   379, 	&unFuncCode.Code.Group10.u16CommBaudRate	},
	{  0,				  3,		  0,			 0, 		   380, 	&unFuncCode.Code.Group10.u16CommParity		},
	{  0,			  65535,		  0,			 0, 		   381, 	&unFuncCode.Code.Group10.u16CommOverTime	},
	{  0,			  65535,		  0,			 0, 		   382, 	&unFuncCode.Code.Group10.u16CommDelay		},	
	{  0,			      1,		  0,			 0, 		   383, 	&unFuncCode.Code.Group10.u16CommCmdSaveEeprom},
	{  0,			      7,		  0,			 0, 		   384, 	&unFuncCode.Code.Group10.u16CanBaudRate		},
    {  0,			  65535,		 10,			 0, 		   385, 	&unFuncCode.Code.Group10.u16CanHearTime		},
    {  0,			  65535,		 200,			 0, 		   386, 	&unFuncCode.Code.Group10.u16CanOverTime		},
    {  0,			      0,		  0,			 0, 		   387, 	&unFuncCode.Code.Group10.u16Rsd[1]		    },
    {  0,			      0,		  0,			 0, 		   388, 	&unFuncCode.Code.Group10.u16Rsd[2]		    },
    {  0,			      0,		  0,			 0, 		   389, 	&unFuncCode.Code.Group10.u16Rsd[3]		    },
    {  0,			      0,		  0,			 0, 		   390, 	&unFuncCode.Code.Group10.u16Rsd[4]		    },
    {  0,			      0,		  0,			 0, 		   391, 	&unFuncCode.Code.Group10.u16Rsd[5]		    },
    {  0,			      0,		  0,			 0, 		   392, 	&unFuncCode.Code.Group10.u16Rsd[6]		    },
    {  0,			      0,		  0,			 0, 		   393, 	&unFuncCode.Code.Group10.u16Rsd[7]		    },
    {  0,			      0,		  0,			 0, 		   394, 	&unFuncCode.Code.Group10.u16Rsd[8]		    },

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 11 */
	{  0,			      1,		  0,			 0, 		   394, 	&unFuncCode.Code.Group11.u16PhaseInputLoseChk},
	{  0,			      1,		  1,			 0, 		   395, 	&unFuncCode.Code.Group11.u16OverLoadMotorChk},
	{ 50,			    300,		100,			 0, 		   396, 	&unFuncCode.Code.Group11.u16OverLoadGainM	},
	{  0,			    300,		100,			 0, 		   397, 	&unFuncCode.Code.Group11.u16OverLoadTimeGainM},
	{  0,				300,		100,			 0, 		   398, 	&unFuncCode.Code.Group11.u16PerOverLoadGainM},
	{  0,			  10000,	  10000,			 0, 		   399, 	&unFuncCode.Code.Group11.u16OverSpdThreshold},
	{  0,			  65535,	  10000,			 0, 		   400, 	&unFuncCode.Code.Group11.u16OverSpdTime		},	
	{  0,			   4000,	   4000,			 0, 		   401, 	&unFuncCode.Code.Group11.u16PulseFreMax		},
	{  0,			  65535,	      0,			 0, 		   402, 	&unFuncCode.Code.Group11.u16PosEerrOverH	},	
	{  1,			  65535,	  32767,			 0, 		   403, 	&unFuncCode.Code.Group11.u16PosEerrOverL	},
	{  0,			      1,	      1,			 0, 		   404, 	&unFuncCode.Code.Group11.u16MotorFlyChk		},
	{  0,				  1,		  1,			 0, 		   405, 	&unFuncCode.Code.Group11.u16MotorBlockChk 	},
	{  0,			  65535,		  1,			 0, 		   406, 	&unFuncCode.Code.Group11.u16MotorBlockTime	},
	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group 12 */
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16ParaInitMode	},
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16SoftInitMode	},
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16ErrReset		},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16JogRun			},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16DiDoForceGiveEn	},
	{  0,			  65535,		  0,			 0,         0xffff, 	&unFuncCode.Code.Group12.u16DiForceGive		},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16DoForceGive		},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16CheckDigitalTube},	
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16CheckKeyBord	},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16MemoryAddr		},
	{  0,			  65535,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16TuneCmd			},
	{  0,			  	  1,		  0,			 0, 	    0xffff, 	&unFuncCode.Code.Group12.u16ServOn			},
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16AprInc	},
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16AsrRef		},
	{  0,			  65535,		  0,			 0, 		0xffff, 	&unFuncCode.Code.Group12.u16AcrRef 		},

	/* 下限			  上限			 出厂值			属性			EEPROM地址	功能码地址（实际暂时没用只是为了方便看信息）*/
	/* Group MonDisp */
	{  0,			  65535,		  0,		 0x629, 		0xffff,     &unFuncCode.Code.GroupMon.u16MonDisp[0]		},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[1] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[2] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[3] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[4] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[5] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[6] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[7] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[8] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[9] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[10] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[11] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[12] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[13] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[14] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[15] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[16] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[17] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[18] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[19] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[20] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[21] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[22] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[23] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[24] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[25] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[26] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[27] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[28] 	},
	{  0,			  65535,		  0,		 0x629, 		0xffff, 	&unFuncCode.Code.GroupMon.u16MonDisp[29] 	},
};
#endif

/* 非功能码存储地址 E2的检验码 掉电记忆参数 故障现场 */
const uint16_t fcNoAttri2Eeprom[CHK_NUM + REM_NUM + GROUP_REM_NUM + ERR_NUM] =  
{
	/* E2 校验码地址 */
	1,
	2,
    /* 掉电记忆参数地址 */
    3,
    4,
    5,
	6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    /* 故障现场地址 */
	19,		/* 错误索引 */
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    68,
    69,
    70,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    
    /* 记忆储存组 */
	81,
	82,
    83,
    84,
    85,
    86,
    87,
    88,
    89,
    #if 0
    90,
    91,
    92,
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    117,
    118,
    119,
    120,
    #endif
};
