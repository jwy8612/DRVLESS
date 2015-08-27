#ifndef VIDEOAPI_H
#define VIDEOAPI_H

#define BUFFNUM 4
typedef struct VIDEO_CROP 
{
	unsigned char bCROP;
	signed int top;
	signed int left;
	signed int width;
	signed int height;
} VIDEO_CROP;

typedef enum PIX_FMT
{
	YUYV8bit = 0,
	YUVBUT = 1,
}PIX_FMT;

typedef struct VIDEO_FMT 
{
	PIX_FMT pixFmt;
	signed int width;
	signed int height;
} VIDEO_FMT;
typedef struct VIDEO_PARAM 
{
	VIDEO_CROP vCrop;
	VIDEO_FMT vFmt;
} VIDEO_PARAM, *PVIDEO_PARAM;

typedef struct VIDEO_BUFF 
{
	void *start;
	unsigned int  length;
} VIDEO_BUFF ;

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
signed int Video_Show_CAP(void * vInst);
signed int Video_Show_STD(void * vInst);
signed int Video_show_FMTDESC(void * vInst);
signed int Video_SetConfig(void * vInst, VIDEO_PARAM *videoParam);
signed int Video_Showt_CurrentFMT(void * vInst);
signed int Video_BuffersInit(void * vInst);
signed int Video_StartCapture(void * vInst);
signed int Video_GetFd(void * vInst);
signed int Video_GetFrame(void * vInst, VIDEO_BUFF *vBuff);
signed int Video_Release(void * vInst);

void * Com_Init();
signed int Com_SetConfig(void * Inst, PCOM_INFO comInfo);
signed int Com_SendData(void *Inst, void *buff, signed int dataNum);
signed int Com_RecieveData(void *Inst, void *buff, signed int dataNum );
signed int Com_GetFd(void * Inst);
signed int Com_Release(void *Inst);
	





#endif
