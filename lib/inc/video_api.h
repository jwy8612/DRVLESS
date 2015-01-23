#ifndef VIDEOAPI_H
#define VIDEOAPI_H

typedef enum BAUND_RATE
{
	BR9600 = 0,
	BR19200,
	BR38400,
	BR115200,
	BRBUTT = 4,	
}BAUND_RATE;

typedef enum DATA_NUM
{
	DN7 = 0,
	DN8,
	DNBUTT = 2,
}DATA_NUM;

typedef enum RARITY
{
	NOP = 0,
	ODDP,
	EVENP,
	BUTTP = 3,
}RARITY;

typedef enum STOP
{
	S1 = 0,
	S2,
	SBUTT = 2,
}STOP;

typedef struct COM_INFO
{
	BAUND_RATE baundRate;
	DATA_NUM dataNum;
	RARITY parity;
	STOP stop;
}COM_INFO, *PCOM_INFO;



void *Video_Init();

int Video_GetConfig(void * vInst);
int Video_Release(void * vInst);

void * Com_Init();
int Com_SetConfig(void * Inst, PCOM_INFO comInfo);
int Com_SendData(void *Inst, void *buff, int dataNum);
int Com_RecieveData(void *Inst, void *buff, int dataNum );
int Com_Release(void *Inst);
	





#endif
