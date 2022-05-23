#ifndef __BOUNDARY_H_
#define __BOUNDARY_H_
#include "stdint.h"


/* ���軻 */
#define  DETECTORIN       0
#define  DETECTOROUT      1
#define  DETECTORNONE     2 

enum MOWER_DETECTOR_FSM{
	MOWER_NONE = 0,
	MOWER_LIN_RIN,
	MOWER_LIN_ROUT,
	MOWER_LOUT_RIN,	
	MOWER_LOUT_ROUT,	
};

/*����ָ��*/
typedef struct  
{	
	struct
	{
			uint8_t  u8FirstFlag;
			uint32_t u32Count5ms;			
			uint32_t u32Count50ms;			
	}Para;
		
	struct
	{
			uint32_t Postive;
		  uint32_t Negative;
	}In;
	
	struct
	{	
		  uint8_t u8statuesCurrent;
			uint8_t u8statues;
		
	}Out;
	
}BOUNDARY_SUB_STR  ;



typedef struct  
{	
	BOUNDARY_SUB_STR  L;
	BOUNDARY_SUB_STR  R;

	struct
	{					
	  enum MOWER_DETECTOR_FSM eMowerPosition;	      /*����ı߽�ֵ*/			
	}Out;
	
}BOUNDARY_STR;



/*����ָ��*/
typedef struct  
{
  void(*pvBoundary_Init)(BOUNDARY_STR *pstr );
	void(*pvBoundary_Deal)(BOUNDARY_SUB_STR *pstr );
	void(*pvBoundary_ADCIsr)(BOUNDARY_STR *pstr );
	
}BOUNDARY_FUN_STR;


/*����*/
extern BOUNDARY_STR  Boundary;  
extern BOUNDARY_FUN_STR strBoundaryFun; 


#endif


