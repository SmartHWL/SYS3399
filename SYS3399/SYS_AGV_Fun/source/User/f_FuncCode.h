#ifndef __F_FUNC_CODE_H_
#define __F_FUNC_CODE_H_
#include "stdint.h"
#include "UserConfig.h"
// 获得功能码code在全部功能码中的index，即功能码在数组funcCode.all[]中的下标。
// 根据 FUNCCODE_ALL 的定义，一个功能码的表示有4种办法，对应的获得index的办法：
// 1. funcCode.all[i]     ---- i
// 2. funcCode.f5[7]      ---- GetCodeIndex(funcCode.group.f5[7])
// 3. group, grade        ---- GetGradeIndex(group, grade)
// 4. funcCode.code.maxFrq---- GetCodeIndex(funcCode.code.maxFrq)
#define GetCodeIndex(code)    ((uint16_t)((&(code)) - (&(unFuncCode.All[0]))))
#define GetGradeIndex(group, grade)  (funcCodeGradeSum[group] + (grade))

//=====================================================================
// 功能码属性位定义
//
// 小数位
#define ATTRIBUTE_POINT_NO				0
#define ATTRIBUTE_POINT_ONE				1
#define ATTRIBUTE_POINT_TWO				2
#define ATTRIBUTE_POINT_THREE			3
#define ATTRIBUTE_POINT_FOUR			4



// 读写属性
#define ATTRIBUTE_READ_AND_WRITE        0   // (任何时候)可写
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // 运行时只读
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // 只读

// 多功能码组合属性
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // 单独的功能码
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // 多个功能码，十进制
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // 多个功能码，十六进制

struct  FUNC_ATTRIBUTE_BITS
{                           // bits   description
    uint16_t point:3;         // 2:0    radix point,小数点。
                            //        0-无小数点，1-1位小数，...，4-4位小数
                            //        (0.0000-100,00.000-011,000.00-010,0000.0-001,00000-000)
    uint16_t displayBits:3;   // 8:6    5个数码管要显示的位数。0-显示0位，1-显示1位，...，5-显示5位
    uint16_t upperLimit:1;    // 9      1-参数由上限相关功能码限制，0-直接由上限限制
    uint16_t lowerLimit:1;    // 10     1-参数由下限相关功能码限制，0-直接由下限限制
    uint16_t writable:2;      // 12:11  参数读写特性，00-可以读写, 01-运行中只读，10-参数只读
    uint16_t signal:1;        // 13     符号，unsignal-0; signal-1
    uint16_t multiLimit:2;    // 15:14  该功能码为多个功能码的组合.
                            //        00-单独功能码(非组合);
                            //        01-十进制,  多个功能码的组合;
                            //        10-十六进制,多个功能码的组合;
};

union FUNC_ATTRIBUTE			//功能属性
{
   uint16_t                      all;
   struct FUNC_ATTRIBUTE_BITS  bit;
};

/* 功能码属性表：上限、下限、属性 */
/* 功能码的出厂值，包括EEPROM_CHECK、掉电记忆，但不包括显示组 */
typedef struct FUNCCODE_ATTRIBUTE_STRUCT
{
    uint16_t                  lower;          // 下限
    uint16_t                  upper;          // 上限
    uint16_t                  init;           // 出厂值
    union FUNC_ATTRIBUTE    attribute;      // 属性

    uint16_t                  eepromIndex;    // 对应EEPROM存储的index
    uint16_t				  *pu16FuncCodeAdd;
} FUNCCODE_ATTRIBUTE;
extern const FUNCCODE_ATTRIBUTE funcCodeAttribute[];

#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(unFuncCode.Code.CheckWord.u16Rsvd4All)
//=====================================================================

struct DI_FUNCCODE
{
	uint16_t u16Function;				/* DI功能 */
	uint16_t u16Polariy;				/* DI极性 */	
	uint16_t u16FilterTime;				/* DI滤波时间 */
};
struct AI_FUNCCODE
{
	uint16_t u16Offset;					/* AI偏置 */
	uint16_t u16FilterTime;				/* AI滤波时间常数 */
	uint16_t u16DeadZone;				/* AI死区 */
	uint16_t u16ZeroDrift;				/* AI零漂 */
};

struct DO_FUNCCODE
{
	uint16_t u16Function;				/* DO功能 */
	uint16_t u16Polariy;				/* DO极性 */	
};



typedef struct GROUP0
{	
    /* 电机基本参数 */
	uint16_t u16ServMotorNo;			/* 电机表编号 */
    uint16_t u16MotorPower;             /* 电机额定功率 *0.01KW */
    uint16_t u16MotorVoltage;           /* 电机额定电压 *1V */
    uint16_t u16MotorCurrent;           /* 电机额定电流 0.01A*/
    uint16_t u16MotorSpeed;             /* 电机额定转速 *1RPM */
	uint16_t u16MotorTrq;				/* 电机额定转矩 *0.01NM */
	uint16_t u16MotorJn;				/* 转动惯量 *0.01kgcm2 */
	uint16_t u16MotorPoles;				/* 极对数 */
    uint16_t u16MotorRs;                /* 电机定子电阻 *0.001O */
    uint16_t u16MotorLd;                /* 电机d轴电感 *0.01mH */
    uint16_t u16MotorLq;                /* 电机q轴电感 *0.01mH */
    uint16_t u16MotorBemfCoeff;         /* 电机线反电动势系数 *0.01mv/rpm */
	uint16_t u16MotorKt;				/* 转矩系数 *0.01Nm/Arms */
	uint16_t u16MotorTe;				/* 电机电气常数 *0.01ms */
	uint16_t u16MotorTm;				/* 电机机械常数 *0.01ms */
	uint16_t u16EncType;				/* 编码器类型 */
	uint16_t u16AbsEncOffSetH;			/* 绝对编码器位置偏置高16 */
	uint16_t u16AbsEncOffSetL;			/* 绝对编码器位置偏置低16 */
	uint16_t u16EncLineNumH;			/* 编码器线数高16 */
	uint16_t u16EncLineNumL;			/* 编码器线数低16 */
	uint16_t u16ThetaZ;					/* Z信号对应角度 *0.1° */
	uint16_t u16ThetaU;					/* U相上升沿角度 *0.1° */
    uint16_t u16Rsd[10];                /* 预留 */
}GROUP0_STR;

typedef struct GROUP1
{
	/* 驱动器参数 */
	uint16_t u16FuncSoftVersion;		/* 应用软件层版本 */
	uint16_t u16FuncSoftRevision;		/* 应用软件层修订版本 */
	uint16_t u16CoreSoftVersion;		/* 电机驱动层软件版本 */	
	uint16_t u16CoreSoftRevision;		/* 电机驱动层修订版本 */
	uint16_t u16CpldVersion;			/* CPLD 版本 */
	uint16_t u16ServoType;				/* 伺服编号 */
	uint16_t u16CanOpenVersion;			/* CanOpen 版本 */
	uint16_t u16CustomSoftVersion;			/* 顾客定制版本 */
	uint16_t u16CustomSoftRevision;		/* 顾客定制修订版本 */
}GROUP1_STR;

typedef struct GROUP2
{
	/* 基本控制参数 */
	uint16_t u16CtrlMode;				/* 控制模式 */
	uint16_t u16RatoDir;				/* 旋转方向 */
	uint16_t u16PlsOutDir;				/* 输出脉冲方向 */
	uint16_t u16SpdDispLpfTime;			/* 速度显示低通滤波时间 */
	uint16_t u16E2PowerOffSave;			/* 掉电储存 */
	uint16_t u16AbzFilterTime;			/* 正交编码时间常数 */
    uint16_t u16Rsd[25];
}GROUP2_STR;

typedef struct GROUP3
{
	uint16_t u16FuncPowerOnDI[4];		/* DI功能上电有效位 */
	struct DI_FUNCCODE DI[8];				/* DI功能，极性，滤波时间 */
	struct AI_FUNCCODE AI[2];		/* AI 各参数 */
	uint16_t u16SpdGainAI;				/* 模拟量10V对应速度值 */
	uint16_t u16CurGainAI;				/* 模拟量10V对应转矩值 */
	
}GROUP3_STR;


typedef struct GROUP4
{
	struct DO_FUNCCODE DO[4];
}GROUP4_STR;

typedef struct GROUP5
{
	/* 位置控制参数 */
	uint16_t u16PosRefSrc;				/* 位置指令来源 */
	uint16_t u16PosRefLpfTime;			/* 位置指令低通滤波时间常数 */
	uint16_t u16PosRefFirTime;			/* 位置指令平均滤波时间常数 */
	uint16_t u16GearNumerator1High;		/* 电子齿轮分子1高16位 */
	uint16_t u16GearNumerator1Low;		/* 电子齿轮分子1低16位 */
	uint16_t u16GearDenominator1High;	/* 电子齿轮分母1高16位 */
	uint16_t u16GearDenominator1Low;	/* 电子齿轮分母1低16位 */
	uint16_t u16GearNumerator2High;		/* 电子齿轮分子2高16位 */
	uint16_t u16GearNumerator2Low;		/* 电子齿轮分子2低16位 */
	uint16_t u16GearDenominator2High;	/* 电子齿轮分母2高16位 */
	uint16_t u16GearDenominator2Low;	/* 电子齿轮分母2低16位 */
	uint16_t u16GearChgCond;			/* 电子齿轮切换条件 */
	uint16_t u16PlsInMode;				/* 脉冲输入模式 */
	uint16_t u16PosErrClrMode;			/* 位置误差清除模式 */
	uint16_t u16EncPlsDivFre;			/* 编码器分频数 */
	uint16_t u16PosArriveCond;			/* 位置达到条件 */
	uint16_t u16PosArriveThreshold;		/* 位置到达阈值 */
	uint16_t u16PosNearThreshold;		/* 位置接近阈值 */
	
	uint16_t u16HomingEnable;			/* 原点复归使能控制 */
	uint16_t u16HomingMode;				/* 原点复归模式 */
	uint16_t u16HomingHighSpeed;		/* 高速搜索原点开关的速度 */
	uint16_t u16HomingLowSpeed;			/* 低速搜索原点开关的速度 */
	uint16_t u16HomingAccDecTime;		/* 搜索原点开关信号的速度 */
	uint16_t u16HomingTime;				/* 搜索原点开关时间 */
	uint16_t u16HomePosOffsetH;			/* 机械原点偏移量 */
	uint16_t u16HomePosOffsetL;			/* 机械原点偏移量 */
    
    uint16_t u16Rsd[20];
}GROUP5_STR;

typedef struct GROUP6
{
	uint16_t u16SpdXSrc;				/* 主速度指令来源 */
	uint16_t u16SpdYSrc;				/* 辅速度指令来源 */
	uint16_t u16SpdRefSrc;				/* 速度指令选择 */
	uint16_t u16SpdPreset;				/* 速度指令键盘设定 */
	uint16_t u16SpdJogSet;				/* 点动速度设定 */
	uint16_t u16SpdAccTime;				/* 加速时间常数 */
	uint16_t u16SpdDecTime;				/* 减速时间常数 */
	uint16_t u16SpdMaxLmt;				/* 最大速度限制 */
	uint16_t u16SpdFwdLmt;				/* 正向速度限制 */
	uint16_t u16SpdRevLmt;				/* 反向速度限制 */
	//uint16_t u16TorFfdSrc;			//力矩前馈源
	uint16_t u16PosZerSpdThreshold;		/* 零位固定转速阈值 */
	uint16_t u16SpdRatoThreshold;		/* 电机旋转转速阈值 */
	uint16_t u16SpdSameThreshold;		/* 速度一致阈值 */
	uint16_t u16SpdArriveThreshold;		/* 速度到达阈值 */
	uint16_t u16SpdZeroThreshold;		/* 零速阈值 */
	uint16_t u16SpdTrajType;			/* 速度轨迹类型 */	
    uint16_t u16Rsd[19];
}GROUP6_STR;

typedef struct GROUP7
{
	uint16_t u16TrqXSrc;				/* 主力矩指令来源 */
	uint16_t u16TrqYSrc;				/* 辅力矩指令来源 */
	uint16_t u16TrqRefSrc;				/* 力矩指令源 */
	uint16_t u16TrqPreset;				/* 力矩指令键盘设定 */
	uint16_t u16TrqRefLpfTime;			/* 力矩指令滤波时间常数 */
	uint16_t u16TrqLmtSrc;				/* 力矩限制指令源 */
	uint16_t u16TrqAiLmtSrc;			/* 力矩模拟限制源选择 */
	uint16_t u16TrqLmtInsideP;			/* 力矩内部限制正 */
	uint16_t u16TrqLmtInsideN;			/* 力矩内部限制负 */
	uint16_t u16TrqLmtOutsideP;			/* 力矩外部限制正 */
	uint16_t u16TrqLmtOutsideN;			/* 力矩外部限制负 */
	uint16_t u16TrqSpdLmtSrc;			/* 力矩模式速度限制源 */
	uint16_t u16TrqSpdAiLmtSrc;			/* V-LMT源 */
	uint16_t u16TrqSpdLmtFwd;			/* 力矩模式正向速度限值 */
	uint16_t u16TrqSpdLmtRev;			/* 力矩模式反向速度限值 */
	//uint16_t u16TrqArrBase;				/* 力矩达到基准值 */
	uint16_t u16TrqArrThreshold;		/* 力矩到达阈值 */
	uint16_t u16TrqNotArrThreshold;		/* 力矩未到达阈值 */
	uint16_t u16TrqSpdLmtTimeWindow;		/* 力矩模式转速受限时间窗口 */
    uint16_t u16Rsd[9];		

}GROUP7_STR;

typedef struct GROUP8
{
	uint16_t u16AsrKp1;					/* 速度环增益 */
	uint16_t u16AsrTi1;					/* 速度环积分时间常数 */
	uint16_t u16AprKp1;					/* 位置环增益 */
	uint16_t u16AsrKp2;					/* 速度环增益 */
	uint16_t u16AsrTi2;					/* 速度环积分时间常数 */
	uint16_t u16AprKp2;					/* 位置环增益 */	
	uint16_t u16GainChgCond;			/* 增益切换条件 */
	uint16_t u16GainChgDelay;			/* 增益切换延迟 */
	uint16_t u16GainChgRank;			/* 增益切换等级 */	
	uint16_t u16GainChgHystLoop;		/* 增益切换等级 */
	uint16_t u16AprKpSwitchTime;		/* 位置增益切换时间 */
	uint16_t u16Jx;						/* 负载转动惯量比 */
	uint16_t u16SpdFfdFilterTime;		/* 速度前馈滤波时间 */
	uint16_t u16SpdFfdGain;				/* 速度前馈增益 */
	uint16_t u16TrqFfdFilterTime;		/* 转矩前馈滤波时间 */
	uint16_t u16TrqFfdGain;				/* 转矩前馈增益 */
	uint16_t u16SpdFbkFirFltCnt;		/* 速度反馈平均滤波次数 */
	uint16_t u16SpdFbkLpfFc;			/* 速度反馈低通滤波截至频率 */
	uint16_t u16SpdKpdffCoeff;			/* PDFF系数 */
    uint16_t u16Rsd[10]; 
}GROUP8_STR;

typedef struct GROUP9
{
	struct
	{
		uint16_t u16NotchFre;
		uint16_t u16WidthRank;
		uint16_t u16DepthRank;
	}NotchFilter[4];
    uint16_t u16Rsd[20];
}GROUP9_STR;

typedef struct GROUP10
{
	/* 通讯类参数 */
	uint16_t u16CommSlaveAddr;			/* 伺服轴号 */
	uint16_t u16CommBaudRate;			/* 串口波特率 */
	uint16_t u16CommParity;				/* 数据格式 */
	uint16_t u16CommOverTime;			/* 通讯超时时间 */
	uint16_t u16CommDelay;				/* 应答延迟时间 */
	uint16_t u16CommCmdSaveEeprom;		/* MODBUS是否E2存储 */
	uint16_t u16CanBaudRate;			/* CAN波特率 */
	uint16_t u16CanHearTime;			/* CAN心跳时间 */
	uint16_t u16CanOverTime;			/* CAN超时时间 */
    uint16_t u16Rsd[8];
}GROUP10_STR;

typedef struct GROUP11
{
	/* b 故障保护类参数 */
	uint16_t u16PhaseInputLoseChk;	/* 输入缺相使能 */
	uint16_t u16OverLoadMotorChk;	/* 电机过载检测是否失能 */
	uint16_t u16OverLoadGainM;		/* 电机过载增益 *1% */	
	uint16_t u16OverLoadTimeGainM;  /* 电机过载时间增益 *1% */
	uint16_t u16PerOverLoadGainM;	/* 电机过载增益预报警 *1% */
	uint16_t u16OverSpdThreshold;	/* 超速阈值 */
	uint16_t u16OverSpdTime;		/* 超速判断时间 */
	uint16_t u16PulseFreMax;		/* 最大脉冲频率 */
	uint16_t u16PosEerrOverH;		/* 位置误差过大高16位 */
	uint16_t u16PosEerrOverL;		/* 位置误差过大低16位 */
	uint16_t u16MotorFlyChk;		/* 电机飞车保护 */	
	uint16_t u16MotorBlockChk;		/* 堵转过温使能 */
	uint16_t u16MotorBlockTime;		/* 堵转过温保护时间 */
	
	//ERROR_SCENE_STR ErrScene[10];	/* 故障现场 不显示 */   
//	uint16_t ovPointSet;			//过压保护点
//	uint16_t uvPointSet;			//欠压保护点
//	uint16_t brakeStartVoltage;	//刹车电阻开启点


}GROUP11_STR;

typedef struct GROUP12
{
	uint16_t u16ParaInitMode;		//参数复位
	uint16_t u16SoftInitMode;		//软件复位
	uint16_t u16ErrReset;			//故障复位
	uint16_t u16JogRun;				/* Jog运行 */
	uint16_t u16DiDoForceGiveEn;		//DI DO 强势输入输出使能
	uint16_t u16DiForceGive;			//DI 强制输入
	uint16_t u16DoForceGive;			//DO 强制输出
	uint16_t u16CheckDigitalTube;	//数码管缺画检测
	uint16_t u16CheckKeyBord;		//按键检测
	uint16_t u16MemoryAddr;			//内存地址
	uint16_t u16TuneCmd;				// 调谐
	
	uint16_t u16ServOn;
	uint16_t u16AprInc;
	uint16_t u16AsrRef;
	uint16_t u16AcrRef;
}GOURP12_STR;


typedef struct GROUP_MONDISP
{
	uint16_t u16MonDisp[30];				//参数监视数组

}GROUP_MONDISP_STR;

typedef struct CHECK_WORD
{
	uint16_t u16Rsvd4All;			// 保留，放在最前面
	uint16_t u16EepromCheckWord1;	// epromCheckWord1
	uint16_t u16EepromCheckWord2;	// eepromCheckWord2
}CHECK_WORD_STR;

typedef struct POWER_OFF_REMEMBER
{
	uint16_t u16PowerUpTimeAddupSec;   /* 上电时间s计时 */
	uint16_t u16PowerUpTimeAddupHour;  /* 累计上电时间h */
    uint16_t u16Rsd[14];
    
}POWER_OFF_REMEMBER_STR;

typedef struct ERROR_SCENE
{
	uint16_t u16ErrCode;				/* 故障现场错误代码 */
	uint16_t u16TimeH;					/* 故障现场时间戳H */
	uint16_t u16TimeL;					/* 故障现场时间戳L */
    
    int16_t s16SpdRefH;
    int16_t s16SpdRefL;
    int16_t s16SpdFbkH;           		/* 故障现场错误速度 */
    int16_t s16SpdFbkL;           		/* 故障现场错误速度 */
    
    int16_t s16TrqRefH;
    int16_t s16TrqRefL;
    int16_t s16TrqFbkH;           		/* 故障现场错误速度 */
    int16_t s16TrqFbkL;           		/* 故障现场错误速度 */
    
    int16_t s16DriverTemp;              /* 驱动器温度 */
    uint16_t u16DcBusVol;       		/* 故障现场母线电压 */
    uint16_t u16DiStatus;       		/* 故障现场DI状态 */
    uint16_t u16DoStatus;       		/* 故障现场DO状态 */
    
  //  uint16_t u16Rsd[10];
}ERROR_SCENE_STR;

typedef struct GROUP_REM
{
    struct
    {
        uint16_t u16SinOffset;
        uint16_t u16CosOffset;
        uint16_t u16SinGain;
        uint16_t u16CosGain;
    }Resolve;

	struct
	{
		uint16_t u16SpdH;
		uint16_t u16SpdL;
		uint16_t u16TimeOut;
		int16_t  s16Compensation;
		uint16_t u16CmdHomeAd;	/* EPS 输入指令是模拟量 可以借助他来判断回零方向 */
	}HomingMode;

}GROUP_REM_STR;

#define ERROR_SCENE_DEPTH               4  /* 故障现场深度 2的倍数 方便运算 */
#define G00NUM				(sizeof(struct GROUP0) / sizeof(uint16_t))
#define G01NUM				(sizeof(struct GROUP1) / sizeof(uint16_t))
#define G02NUM				(sizeof(struct GROUP2) / sizeof(uint16_t))
#define G03NUM				(sizeof(struct GROUP3) / sizeof(uint16_t))
#define G04NUM				(sizeof(struct GROUP4) / sizeof(uint16_t))
#define G05NUM				(sizeof(struct GROUP5) / sizeof(uint16_t))
#define G06NUM				(sizeof(struct GROUP6) / sizeof(uint16_t))
#define G07NUM				(sizeof(struct GROUP7) / sizeof(uint16_t))
#define G08NUM				(sizeof(struct GROUP8) / sizeof(uint16_t))
#define G09NUM				(sizeof(struct GROUP9) / sizeof(uint16_t))
#define G10NUM				(sizeof(struct GROUP10) / sizeof(uint16_t))
#define G11NUM				(sizeof(struct GROUP11) / sizeof(uint16_t))
#define G12NUM				(sizeof(struct GROUP12) / sizeof(uint16_t))
#define GU0NUM				(sizeof(struct GROUP_MONDISP) / sizeof(uint16_t)) /* 显示组参数 */

#define FUNCCODE_GROUP_NUM  14

#define CHK_NUM  			(sizeof(struct CHECK_WORD) / sizeof(uint16_t))  	/* 校验字 包含E2 预留字 */
#define REM_NUM   			(sizeof(struct POWER_OFF_REMEMBER) / sizeof(uint16_t))    /* 掉电记忆 */
#define GROUP_REM_NUM		(sizeof(struct GROUP_REM) / sizeof(uint16_t))	/* 记忆储存组 */
#define ERR_NUM				(sizeof(struct ERROR_SCENE) / sizeof(uint16_t) * ERROR_SCENE_DEPTH + 1)


#define FNUM_PARA				(G00NUM + G01NUM + G02NUM + G03NUM + G04NUM	\
							   + G05NUM + G06NUM + G07NUM + G08NUM + G09NUM	\
							   + G10NUM + G11NUM + G12NUM 				    \
							   + GU0NUM 									\
							   )// 所有功能码，不包括显示

#define FNUM_EEPROM			(FNUM_PARA + CHK_NUM + REM_NUM \
                            +GROUP_REM_NUM + ERR_NUM)      	                    /* 需要存储在EEPROM中的所有参数 */
#define FNUM_ALL			(FNUM_EEPROM )							/* 所有功能码、参数，包括显示 */

// 每组功能码的个数
// 除FF组之外，每组预留2个功能码。为了在增加功能码时，尽量不用恢复出厂参数。
struct FUNCCODE_GROUP 
{
	uint16_t G00[G00NUM];
	uint16_t G01[G01NUM];
	uint16_t G02[G02NUM];
	uint16_t G03[G03NUM];
	uint16_t G04[G04NUM];
	uint16_t G05[G05NUM];
	uint16_t G06[G06NUM];
	uint16_t G07[G07NUM];
	uint16_t G08[G08NUM];
	uint16_t G09[G09NUM];
	uint16_t G10[G10NUM];
	uint16_t G11[G11NUM];	
	uint16_t G12[G12NUM];

	uint16_t gU0[GU0NUM];				//显示组不用放E2	
	

	uint16_t u16CheckWord[CHK_NUM];
	uint16_t u16PowerOffRem[REM_NUM];
	uint16_t u16ErrScene[ERR_NUM];
    uint16_t u16GroupRem[GROUP_REM_NUM];


};
typedef struct FUNCCODE_CODE
{
	/* G0 伺服电机参数 */
	GROUP0_STR Group0;
	
	/* G1 驱动器参数 */
	GROUP1_STR Group1;
	
	/* G2 基本控制参数 */
	GROUP2_STR Group2;
	
	/* G3 端子输入参数 */
	GROUP3_STR Group3;
	
	/* G4 端子输出参数 */
	GROUP4_STR Group4;
	
	/* G5 位置控制参数 */
	GROUP5_STR Group5;
	
	/* G6 速度控制参数 */
	GROUP6_STR Group6;
	
	/* G7 转矩控制参数 */
	GROUP7_STR Group7;
	
	/* G8 增益类参数 */
	GROUP8_STR Group8;

	/* G9 自调整参数 */
	GROUP9_STR Group9;

	/* G10 通讯参数 */
	GROUP10_STR Group10;
	/* G11 故障保护类参数 */
	GROUP11_STR Group11;

	/* G12 辅助功能参数	*/
	GOURP12_STR Group12;

	/* 监视组 */
	GROUP_MONDISP_STR GroupMon;
	
	/**************************以下不显示***************************/
	/* 检验字 */
	CHECK_WORD_STR	CheckWord;
	
	/* REMEMBER 掉电保存 */
	POWER_OFF_REMEMBER_STR PowerOffRem;

	/* 故障现场   */
	uint16_t u16ErrRecordIndex;
	ERROR_SCENE_STR ErrScene[ERROR_SCENE_DEPTH];	

    /* 记忆储存组 */
	GROUP_REM_STR GroupRem;
}FUNCCODE_CODE_STR;

//=====================================================================
//
// 功能码的定义。
// 联合体，成员分别为数组，结构体，结构体
// 于是，一个功能码的访问，有三种方式:
// funcCode.all[index]     index = GetCodeIndex(funcCode.code.presetFrq);
// funcCode.group.f0[8]    index = GetCodeIndex(funcCode.group.f0[8]);
// funcCode.code.presetFrq
// 
//=====================================================================
typedef union FUNCCODE_ALL
{
    uint16_t All[FNUM_ALL];	
    struct FUNCCODE_GROUP Group;
    struct FUNCCODE_CODE Code;
} FUNCCODE_ALL_UN;

//=====================================================================
// 有些功能码的上下限是其它某个功能码，这里是在funcCode中的index

// 程序中使用的一些功能码的index
//= 如果增加/删除了功能码，这里需要修改!

#define EEPROM_CHECK_INDEX1     (GetCodeIndex( unFuncCode.Code.CheckWord.u16EepromCheckWord1))  // eepromCheckWord1
#define EEPROM_CHECK_INDEX2     (GetCodeIndex( unFuncCode.Code.CheckWord.u16EepromCheckWord2))  // eepromCheckWord2

extern 	FUNCCODE_ALL_UN unFuncCode;
extern const uint16_t funcCodeGradeSum[FUNCCODE_GROUP_NUM];
extern const uint16_t funcCodeGradeAll[FUNCCODE_GROUP_NUM];
#endif

