#ifndef __F_FUNC_CODE_H_
#define __F_FUNC_CODE_H_
#include "stdint.h"
#include "UserConfig.h"
// ��ù�����code��ȫ���������е�index����������������funcCode.all[]�е��±ꡣ
// ���� FUNCCODE_ALL �Ķ��壬һ��������ı�ʾ��4�ְ취����Ӧ�Ļ��index�İ취��
// 1. funcCode.all[i]     ---- i
// 2. funcCode.f5[7]      ---- GetCodeIndex(funcCode.group.f5[7])
// 3. group, grade        ---- GetGradeIndex(group, grade)
// 4. funcCode.code.maxFrq---- GetCodeIndex(funcCode.code.maxFrq)
#define GetCodeIndex(code)    ((uint16_t)((&(code)) - (&(unFuncCode.All[0]))))
#define GetGradeIndex(group, grade)  (funcCodeGradeSum[group] + (grade))

//=====================================================================
// ����������λ����
//
// С��λ
#define ATTRIBUTE_POINT_NO				0
#define ATTRIBUTE_POINT_ONE				1
#define ATTRIBUTE_POINT_TWO				2
#define ATTRIBUTE_POINT_THREE			3
#define ATTRIBUTE_POINT_FOUR			4



// ��д����
#define ATTRIBUTE_READ_AND_WRITE        0   // (�κ�ʱ��)��д
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // ����ʱֻ��
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // ֻ��

// �๦�����������
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // �����Ĺ�����
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // ��������룬ʮ����
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // ��������룬ʮ������

struct  FUNC_ATTRIBUTE_BITS
{                           // bits   description
    uint16_t point:3;         // 2:0    radix point,С���㡣
                            //        0-��С���㣬1-1λС����...��4-4λС��
                            //        (0.0000-100,00.000-011,000.00-010,0000.0-001,00000-000)
    uint16_t displayBits:3;   // 8:6    5�������Ҫ��ʾ��λ����0-��ʾ0λ��1-��ʾ1λ��...��5-��ʾ5λ
    uint16_t upperLimit:1;    // 9      1-������������ع��������ƣ�0-ֱ������������
    uint16_t lowerLimit:1;    // 10     1-������������ع��������ƣ�0-ֱ������������
    uint16_t writable:2;      // 12:11  ������д���ԣ�00-���Զ�д, 01-������ֻ����10-����ֻ��
    uint16_t signal:1;        // 13     ���ţ�unsignal-0; signal-1
    uint16_t multiLimit:2;    // 15:14  �ù�����Ϊ�������������.
                            //        00-����������(�����);
                            //        01-ʮ����,  �������������;
                            //        10-ʮ������,�������������;
};

union FUNC_ATTRIBUTE			//��������
{
   uint16_t                      all;
   struct FUNC_ATTRIBUTE_BITS  bit;
};

/* ���������Ա����ޡ����ޡ����� */
/* ������ĳ���ֵ������EEPROM_CHECK��������䣬����������ʾ�� */
typedef struct FUNCCODE_ATTRIBUTE_STRUCT
{
    uint16_t                  lower;          // ����
    uint16_t                  upper;          // ����
    uint16_t                  init;           // ����ֵ
    union FUNC_ATTRIBUTE    attribute;      // ����

    uint16_t                  eepromIndex;    // ��ӦEEPROM�洢��index
    uint16_t				  *pu16FuncCodeAdd;
} FUNCCODE_ATTRIBUTE;
extern const FUNCCODE_ATTRIBUTE funcCodeAttribute[];

#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(unFuncCode.Code.CheckWord.u16Rsvd4All)
//=====================================================================

struct DI_FUNCCODE
{
	uint16_t u16Function;				/* DI���� */
	uint16_t u16Polariy;				/* DI���� */	
	uint16_t u16FilterTime;				/* DI�˲�ʱ�� */
};
struct AI_FUNCCODE
{
	uint16_t u16Offset;					/* AIƫ�� */
	uint16_t u16FilterTime;				/* AI�˲�ʱ�䳣�� */
	uint16_t u16DeadZone;				/* AI���� */
	uint16_t u16ZeroDrift;				/* AI��Ư */
};

struct DO_FUNCCODE
{
	uint16_t u16Function;				/* DO���� */
	uint16_t u16Polariy;				/* DO���� */	
};



typedef struct GROUP0
{	
    /* ����������� */
	uint16_t u16ServMotorNo;			/* ������� */
    uint16_t u16MotorPower;             /* �������� *0.01KW */
    uint16_t u16MotorVoltage;           /* ������ѹ *1V */
    uint16_t u16MotorCurrent;           /* �������� 0.01A*/
    uint16_t u16MotorSpeed;             /* ����ת�� *1RPM */
	uint16_t u16MotorTrq;				/* ����ת�� *0.01NM */
	uint16_t u16MotorJn;				/* ת������ *0.01kgcm2 */
	uint16_t u16MotorPoles;				/* ������ */
    uint16_t u16MotorRs;                /* ������ӵ��� *0.001O */
    uint16_t u16MotorLd;                /* ���d���� *0.01mH */
    uint16_t u16MotorLq;                /* ���q���� *0.01mH */
    uint16_t u16MotorBemfCoeff;         /* ����߷��綯��ϵ�� *0.01mv/rpm */
	uint16_t u16MotorKt;				/* ת��ϵ�� *0.01Nm/Arms */
	uint16_t u16MotorTe;				/* ����������� *0.01ms */
	uint16_t u16MotorTm;				/* �����е���� *0.01ms */
	uint16_t u16EncType;				/* ���������� */
	uint16_t u16AbsEncOffSetH;			/* ���Ա�����λ��ƫ�ø�16 */
	uint16_t u16AbsEncOffSetL;			/* ���Ա�����λ��ƫ�õ�16 */
	uint16_t u16EncLineNumH;			/* ������������16 */
	uint16_t u16EncLineNumL;			/* ������������16 */
	uint16_t u16ThetaZ;					/* Z�źŶ�Ӧ�Ƕ� *0.1�� */
	uint16_t u16ThetaU;					/* U�������ؽǶ� *0.1�� */
    uint16_t u16Rsd[10];                /* Ԥ�� */
}GROUP0_STR;

typedef struct GROUP1
{
	/* ���������� */
	uint16_t u16FuncSoftVersion;		/* Ӧ�������汾 */
	uint16_t u16FuncSoftRevision;		/* Ӧ��������޶��汾 */
	uint16_t u16CoreSoftVersion;		/* �������������汾 */	
	uint16_t u16CoreSoftRevision;		/* ����������޶��汾 */
	uint16_t u16CpldVersion;			/* CPLD �汾 */
	uint16_t u16ServoType;				/* �ŷ���� */
	uint16_t u16CanOpenVersion;			/* CanOpen �汾 */
	uint16_t u16CustomSoftVersion;			/* �˿Ͷ��ư汾 */
	uint16_t u16CustomSoftRevision;		/* �˿Ͷ����޶��汾 */
}GROUP1_STR;

typedef struct GROUP2
{
	/* �������Ʋ��� */
	uint16_t u16CtrlMode;				/* ����ģʽ */
	uint16_t u16RatoDir;				/* ��ת���� */
	uint16_t u16PlsOutDir;				/* ������巽�� */
	uint16_t u16SpdDispLpfTime;			/* �ٶ���ʾ��ͨ�˲�ʱ�� */
	uint16_t u16E2PowerOffSave;			/* ���索�� */
	uint16_t u16AbzFilterTime;			/* ��������ʱ�䳣�� */
    uint16_t u16Rsd[25];
}GROUP2_STR;

typedef struct GROUP3
{
	uint16_t u16FuncPowerOnDI[4];		/* DI�����ϵ���Чλ */
	struct DI_FUNCCODE DI[8];				/* DI���ܣ����ԣ��˲�ʱ�� */
	struct AI_FUNCCODE AI[2];		/* AI ������ */
	uint16_t u16SpdGainAI;				/* ģ����10V��Ӧ�ٶ�ֵ */
	uint16_t u16CurGainAI;				/* ģ����10V��Ӧת��ֵ */
	
}GROUP3_STR;


typedef struct GROUP4
{
	struct DO_FUNCCODE DO[4];
}GROUP4_STR;

typedef struct GROUP5
{
	/* λ�ÿ��Ʋ��� */
	uint16_t u16PosRefSrc;				/* λ��ָ����Դ */
	uint16_t u16PosRefLpfTime;			/* λ��ָ���ͨ�˲�ʱ�䳣�� */
	uint16_t u16PosRefFirTime;			/* λ��ָ��ƽ���˲�ʱ�䳣�� */
	uint16_t u16GearNumerator1High;		/* ���ӳ��ַ���1��16λ */
	uint16_t u16GearNumerator1Low;		/* ���ӳ��ַ���1��16λ */
	uint16_t u16GearDenominator1High;	/* ���ӳ��ַ�ĸ1��16λ */
	uint16_t u16GearDenominator1Low;	/* ���ӳ��ַ�ĸ1��16λ */
	uint16_t u16GearNumerator2High;		/* ���ӳ��ַ���2��16λ */
	uint16_t u16GearNumerator2Low;		/* ���ӳ��ַ���2��16λ */
	uint16_t u16GearDenominator2High;	/* ���ӳ��ַ�ĸ2��16λ */
	uint16_t u16GearDenominator2Low;	/* ���ӳ��ַ�ĸ2��16λ */
	uint16_t u16GearChgCond;			/* ���ӳ����л����� */
	uint16_t u16PlsInMode;				/* ��������ģʽ */
	uint16_t u16PosErrClrMode;			/* λ��������ģʽ */
	uint16_t u16EncPlsDivFre;			/* ��������Ƶ�� */
	uint16_t u16PosArriveCond;			/* λ�ôﵽ���� */
	uint16_t u16PosArriveThreshold;		/* λ�õ�����ֵ */
	uint16_t u16PosNearThreshold;		/* λ�ýӽ���ֵ */
	
	uint16_t u16HomingEnable;			/* ԭ�㸴��ʹ�ܿ��� */
	uint16_t u16HomingMode;				/* ԭ�㸴��ģʽ */
	uint16_t u16HomingHighSpeed;		/* ��������ԭ�㿪�ص��ٶ� */
	uint16_t u16HomingLowSpeed;			/* ��������ԭ�㿪�ص��ٶ� */
	uint16_t u16HomingAccDecTime;		/* ����ԭ�㿪���źŵ��ٶ� */
	uint16_t u16HomingTime;				/* ����ԭ�㿪��ʱ�� */
	uint16_t u16HomePosOffsetH;			/* ��еԭ��ƫ���� */
	uint16_t u16HomePosOffsetL;			/* ��еԭ��ƫ���� */
    
    uint16_t u16Rsd[20];
}GROUP5_STR;

typedef struct GROUP6
{
	uint16_t u16SpdXSrc;				/* ���ٶ�ָ����Դ */
	uint16_t u16SpdYSrc;				/* ���ٶ�ָ����Դ */
	uint16_t u16SpdRefSrc;				/* �ٶ�ָ��ѡ�� */
	uint16_t u16SpdPreset;				/* �ٶ�ָ������趨 */
	uint16_t u16SpdJogSet;				/* �㶯�ٶ��趨 */
	uint16_t u16SpdAccTime;				/* ����ʱ�䳣�� */
	uint16_t u16SpdDecTime;				/* ����ʱ�䳣�� */
	uint16_t u16SpdMaxLmt;				/* ����ٶ����� */
	uint16_t u16SpdFwdLmt;				/* �����ٶ����� */
	uint16_t u16SpdRevLmt;				/* �����ٶ����� */
	//uint16_t u16TorFfdSrc;			//����ǰ��Դ
	uint16_t u16PosZerSpdThreshold;		/* ��λ�̶�ת����ֵ */
	uint16_t u16SpdRatoThreshold;		/* �����תת����ֵ */
	uint16_t u16SpdSameThreshold;		/* �ٶ�һ����ֵ */
	uint16_t u16SpdArriveThreshold;		/* �ٶȵ�����ֵ */
	uint16_t u16SpdZeroThreshold;		/* ������ֵ */
	uint16_t u16SpdTrajType;			/* �ٶȹ켣���� */	
    uint16_t u16Rsd[19];
}GROUP6_STR;

typedef struct GROUP7
{
	uint16_t u16TrqXSrc;				/* ������ָ����Դ */
	uint16_t u16TrqYSrc;				/* ������ָ����Դ */
	uint16_t u16TrqRefSrc;				/* ����ָ��Դ */
	uint16_t u16TrqPreset;				/* ����ָ������趨 */
	uint16_t u16TrqRefLpfTime;			/* ����ָ���˲�ʱ�䳣�� */
	uint16_t u16TrqLmtSrc;				/* ��������ָ��Դ */
	uint16_t u16TrqAiLmtSrc;			/* ����ģ������Դѡ�� */
	uint16_t u16TrqLmtInsideP;			/* �����ڲ������� */
	uint16_t u16TrqLmtInsideN;			/* �����ڲ����Ƹ� */
	uint16_t u16TrqLmtOutsideP;			/* �����ⲿ������ */
	uint16_t u16TrqLmtOutsideN;			/* �����ⲿ���Ƹ� */
	uint16_t u16TrqSpdLmtSrc;			/* ����ģʽ�ٶ�����Դ */
	uint16_t u16TrqSpdAiLmtSrc;			/* V-LMTԴ */
	uint16_t u16TrqSpdLmtFwd;			/* ����ģʽ�����ٶ���ֵ */
	uint16_t u16TrqSpdLmtRev;			/* ����ģʽ�����ٶ���ֵ */
	//uint16_t u16TrqArrBase;				/* ���شﵽ��׼ֵ */
	uint16_t u16TrqArrThreshold;		/* ���ص�����ֵ */
	uint16_t u16TrqNotArrThreshold;		/* ����δ������ֵ */
	uint16_t u16TrqSpdLmtTimeWindow;		/* ����ģʽת������ʱ�䴰�� */
    uint16_t u16Rsd[9];		

}GROUP7_STR;

typedef struct GROUP8
{
	uint16_t u16AsrKp1;					/* �ٶȻ����� */
	uint16_t u16AsrTi1;					/* �ٶȻ�����ʱ�䳣�� */
	uint16_t u16AprKp1;					/* λ�û����� */
	uint16_t u16AsrKp2;					/* �ٶȻ����� */
	uint16_t u16AsrTi2;					/* �ٶȻ�����ʱ�䳣�� */
	uint16_t u16AprKp2;					/* λ�û����� */	
	uint16_t u16GainChgCond;			/* �����л����� */
	uint16_t u16GainChgDelay;			/* �����л��ӳ� */
	uint16_t u16GainChgRank;			/* �����л��ȼ� */	
	uint16_t u16GainChgHystLoop;		/* �����л��ȼ� */
	uint16_t u16AprKpSwitchTime;		/* λ�������л�ʱ�� */
	uint16_t u16Jx;						/* ����ת�������� */
	uint16_t u16SpdFfdFilterTime;		/* �ٶ�ǰ���˲�ʱ�� */
	uint16_t u16SpdFfdGain;				/* �ٶ�ǰ������ */
	uint16_t u16TrqFfdFilterTime;		/* ת��ǰ���˲�ʱ�� */
	uint16_t u16TrqFfdGain;				/* ת��ǰ������ */
	uint16_t u16SpdFbkFirFltCnt;		/* �ٶȷ���ƽ���˲����� */
	uint16_t u16SpdFbkLpfFc;			/* �ٶȷ�����ͨ�˲�����Ƶ�� */
	uint16_t u16SpdKpdffCoeff;			/* PDFFϵ�� */
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
	/* ͨѶ����� */
	uint16_t u16CommSlaveAddr;			/* �ŷ���� */
	uint16_t u16CommBaudRate;			/* ���ڲ����� */
	uint16_t u16CommParity;				/* ���ݸ�ʽ */
	uint16_t u16CommOverTime;			/* ͨѶ��ʱʱ�� */
	uint16_t u16CommDelay;				/* Ӧ���ӳ�ʱ�� */
	uint16_t u16CommCmdSaveEeprom;		/* MODBUS�Ƿ�E2�洢 */
	uint16_t u16CanBaudRate;			/* CAN������ */
	uint16_t u16CanHearTime;			/* CAN����ʱ�� */
	uint16_t u16CanOverTime;			/* CAN��ʱʱ�� */
    uint16_t u16Rsd[8];
}GROUP10_STR;

typedef struct GROUP11
{
	/* b ���ϱ�������� */
	uint16_t u16PhaseInputLoseChk;	/* ����ȱ��ʹ�� */
	uint16_t u16OverLoadMotorChk;	/* ������ؼ���Ƿ�ʧ�� */
	uint16_t u16OverLoadGainM;		/* ����������� *1% */	
	uint16_t u16OverLoadTimeGainM;  /* �������ʱ������ *1% */
	uint16_t u16PerOverLoadGainM;	/* �����������Ԥ���� *1% */
	uint16_t u16OverSpdThreshold;	/* ������ֵ */
	uint16_t u16OverSpdTime;		/* �����ж�ʱ�� */
	uint16_t u16PulseFreMax;		/* �������Ƶ�� */
	uint16_t u16PosEerrOverH;		/* λ���������16λ */
	uint16_t u16PosEerrOverL;		/* λ���������16λ */
	uint16_t u16MotorFlyChk;		/* ����ɳ����� */	
	uint16_t u16MotorBlockChk;		/* ��ת����ʹ�� */
	uint16_t u16MotorBlockTime;		/* ��ת���±���ʱ�� */
	
	//ERROR_SCENE_STR ErrScene[10];	/* �����ֳ� ����ʾ */   
//	uint16_t ovPointSet;			//��ѹ������
//	uint16_t uvPointSet;			//Ƿѹ������
//	uint16_t brakeStartVoltage;	//ɲ�����迪����


}GROUP11_STR;

typedef struct GROUP12
{
	uint16_t u16ParaInitMode;		//������λ
	uint16_t u16SoftInitMode;		//�����λ
	uint16_t u16ErrReset;			//���ϸ�λ
	uint16_t u16JogRun;				/* Jog���� */
	uint16_t u16DiDoForceGiveEn;		//DI DO ǿ���������ʹ��
	uint16_t u16DiForceGive;			//DI ǿ������
	uint16_t u16DoForceGive;			//DO ǿ�����
	uint16_t u16CheckDigitalTube;	//�����ȱ�����
	uint16_t u16CheckKeyBord;		//�������
	uint16_t u16MemoryAddr;			//�ڴ��ַ
	uint16_t u16TuneCmd;				// ��г
	
	uint16_t u16ServOn;
	uint16_t u16AprInc;
	uint16_t u16AsrRef;
	uint16_t u16AcrRef;
}GOURP12_STR;


typedef struct GROUP_MONDISP
{
	uint16_t u16MonDisp[30];				//������������

}GROUP_MONDISP_STR;

typedef struct CHECK_WORD
{
	uint16_t u16Rsvd4All;			// ������������ǰ��
	uint16_t u16EepromCheckWord1;	// epromCheckWord1
	uint16_t u16EepromCheckWord2;	// eepromCheckWord2
}CHECK_WORD_STR;

typedef struct POWER_OFF_REMEMBER
{
	uint16_t u16PowerUpTimeAddupSec;   /* �ϵ�ʱ��s��ʱ */
	uint16_t u16PowerUpTimeAddupHour;  /* �ۼ��ϵ�ʱ��h */
    uint16_t u16Rsd[14];
    
}POWER_OFF_REMEMBER_STR;

typedef struct ERROR_SCENE
{
	uint16_t u16ErrCode;				/* �����ֳ�������� */
	uint16_t u16TimeH;					/* �����ֳ�ʱ���H */
	uint16_t u16TimeL;					/* �����ֳ�ʱ���L */
    
    int16_t s16SpdRefH;
    int16_t s16SpdRefL;
    int16_t s16SpdFbkH;           		/* �����ֳ������ٶ� */
    int16_t s16SpdFbkL;           		/* �����ֳ������ٶ� */
    
    int16_t s16TrqRefH;
    int16_t s16TrqRefL;
    int16_t s16TrqFbkH;           		/* �����ֳ������ٶ� */
    int16_t s16TrqFbkL;           		/* �����ֳ������ٶ� */
    
    int16_t s16DriverTemp;              /* �������¶� */
    uint16_t u16DcBusVol;       		/* �����ֳ�ĸ�ߵ�ѹ */
    uint16_t u16DiStatus;       		/* �����ֳ�DI״̬ */
    uint16_t u16DoStatus;       		/* �����ֳ�DO״̬ */
    
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
		uint16_t u16CmdHomeAd;	/* EPS ����ָ����ģ���� ���Խ��������жϻ��㷽�� */
	}HomingMode;

}GROUP_REM_STR;

#define ERROR_SCENE_DEPTH               4  /* �����ֳ���� 2�ı��� �������� */
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
#define GU0NUM				(sizeof(struct GROUP_MONDISP) / sizeof(uint16_t)) /* ��ʾ����� */

#define FUNCCODE_GROUP_NUM  14

#define CHK_NUM  			(sizeof(struct CHECK_WORD) / sizeof(uint16_t))  	/* У���� ����E2 Ԥ���� */
#define REM_NUM   			(sizeof(struct POWER_OFF_REMEMBER) / sizeof(uint16_t))    /* ������� */
#define GROUP_REM_NUM		(sizeof(struct GROUP_REM) / sizeof(uint16_t))	/* ���䴢���� */
#define ERR_NUM				(sizeof(struct ERROR_SCENE) / sizeof(uint16_t) * ERROR_SCENE_DEPTH + 1)


#define FNUM_PARA				(G00NUM + G01NUM + G02NUM + G03NUM + G04NUM	\
							   + G05NUM + G06NUM + G07NUM + G08NUM + G09NUM	\
							   + G10NUM + G11NUM + G12NUM 				    \
							   + GU0NUM 									\
							   )// ���й����룬��������ʾ

#define FNUM_EEPROM			(FNUM_PARA + CHK_NUM + REM_NUM \
                            +GROUP_REM_NUM + ERR_NUM)      	                    /* ��Ҫ�洢��EEPROM�е����в��� */
#define FNUM_ALL			(FNUM_EEPROM )							/* ���й����롢������������ʾ */

// ÿ�鹦����ĸ���
// ��FF��֮�⣬ÿ��Ԥ��2�������롣Ϊ�������ӹ�����ʱ���������ûָ�����������
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

	uint16_t gU0[GU0NUM];				//��ʾ�鲻�÷�E2	
	

	uint16_t u16CheckWord[CHK_NUM];
	uint16_t u16PowerOffRem[REM_NUM];
	uint16_t u16ErrScene[ERR_NUM];
    uint16_t u16GroupRem[GROUP_REM_NUM];


};
typedef struct FUNCCODE_CODE
{
	/* G0 �ŷ�������� */
	GROUP0_STR Group0;
	
	/* G1 ���������� */
	GROUP1_STR Group1;
	
	/* G2 �������Ʋ��� */
	GROUP2_STR Group2;
	
	/* G3 ����������� */
	GROUP3_STR Group3;
	
	/* G4 ����������� */
	GROUP4_STR Group4;
	
	/* G5 λ�ÿ��Ʋ��� */
	GROUP5_STR Group5;
	
	/* G6 �ٶȿ��Ʋ��� */
	GROUP6_STR Group6;
	
	/* G7 ת�ؿ��Ʋ��� */
	GROUP7_STR Group7;
	
	/* G8 ��������� */
	GROUP8_STR Group8;

	/* G9 �Ե������� */
	GROUP9_STR Group9;

	/* G10 ͨѶ���� */
	GROUP10_STR Group10;
	/* G11 ���ϱ�������� */
	GROUP11_STR Group11;

	/* G12 �������ܲ���	*/
	GOURP12_STR Group12;

	/* ������ */
	GROUP_MONDISP_STR GroupMon;
	
	/**************************���²���ʾ***************************/
	/* ������ */
	CHECK_WORD_STR	CheckWord;
	
	/* REMEMBER ���籣�� */
	POWER_OFF_REMEMBER_STR PowerOffRem;

	/* �����ֳ�   */
	uint16_t u16ErrRecordIndex;
	ERROR_SCENE_STR ErrScene[ERROR_SCENE_DEPTH];	

    /* ���䴢���� */
	GROUP_REM_STR GroupRem;
}FUNCCODE_CODE_STR;

//=====================================================================
//
// ������Ķ��塣
// �����壬��Ա�ֱ�Ϊ���飬�ṹ�壬�ṹ��
// ���ǣ�һ��������ķ��ʣ������ַ�ʽ:
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
// ��Щ�������������������ĳ�������룬��������funcCode�е�index

// ������ʹ�õ�һЩ�������index
//= �������/ɾ���˹����룬������Ҫ�޸�!

#define EEPROM_CHECK_INDEX1     (GetCodeIndex( unFuncCode.Code.CheckWord.u16EepromCheckWord1))  // eepromCheckWord1
#define EEPROM_CHECK_INDEX2     (GetCodeIndex( unFuncCode.Code.CheckWord.u16EepromCheckWord2))  // eepromCheckWord2

extern 	FUNCCODE_ALL_UN unFuncCode;
extern const uint16_t funcCodeGradeSum[FUNCCODE_GROUP_NUM];
extern const uint16_t funcCodeGradeAll[FUNCCODE_GROUP_NUM];
#endif

