#ifndef VIDEOAPI_H
#define VIDEOAPI_H

#define BUFFNUM 4

typedef struct VIDEO_CROP 
{
	char bCROP;
	int top;
	int left;
	int width;
	int height;
} VIDEO_CROP;

typedef enum PIX_FMT
{
	YUYV8bit = 0,
	YUVBUT = 1,
}PIX_FMT;

typedef struct VIDEO_FMT 
{
	PIX_FMT pixFmt;
	int width;
	int height;
} VIDEO_FMT;
typedef struct VIDEO_PARAM 
{
	VIDEO_CROP vCrop;
	VIDEO_FMT vFmt;
} VIDEO_PARAM, *PVIDEO_PARAM;

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

typedef struct MOTOR_CMD_INFO
{
	char bMoterCmd;
	signed char motorV;
}MOTOR_CMD_INFO, *PMOTOR_CMD_INFO;
typedef struct SERVO_CMD_INFO
{

}SERVO_CMD_INFO, *PSERVO_CMD_INFO;
typedef struct LIGHT_CMD_INFO
{

}LIGHT_CMD_INFO, *PLIGHT_CMD_INFO;
typedef struct HORN_CMD_INFO
{

}HORN_CMD_INFO, *PHORN_CMD_INFO;



typedef struct COMMAND_INFO
{
	struct MOTOR_CMD_INFO motorCmd;
	struct SERVO_CMD_INFO servoCmd;
	void *comInst;
	char commadData[50];
	char dataLength;
}COMMAND_INFO,*PCOMMAND_INFO;


void *Video_Init();
int Video_Show_CAP(void * vInst);
int Video_Show_STD(void * vInst);
int Video_show_FMTDESC(void * vInst);
int Video_SetConfig(void * vInst, VIDEO_PARAM *videoParam);
int Video_Showt_CurrentFMT(void * vInst);
int Video_BuffersInit(void * vInst);
int Video_GetFrame(void * vInst);
int Video_Release(void * vInst);

void * Com_Init();
int Com_SetConfig(void * Inst, PCOM_INFO comInfo);
int Com_SendData(void *Inst, void *buff, int dataNum);
int Com_RecieveData(void *Inst, void *buff, int dataNum );
int Com_Release(void *Inst);
	





#endif
