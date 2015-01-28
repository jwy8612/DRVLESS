#ifndef VIDEOAPI_H
#define VIDEOAPI_H
#include <linux/videodev2.h>
#define BUFFNUM 4

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

int Video_GetConfig_CAP(void * vInst,struct v4l2_capability *cap);
int Video_GetConfig_FMT(void * vInst, struct v4l2_fmtdesc  *fmtdesc);
int Video_SetConfig_CROP(void * vInst, struct v4l2_crop *cropcap);

int Video_GetConfig_STD(void * vInst,v4l2_std_id *std);
int Video_SetConfig_FMT(void * vInst,struct v4l2_format *videoFmt);
int Video_BuffersInit(void * vInst,struct v4l2_requestbuffers* req);
int Video_GetCrrent_FMT(void * vInst);

int Video_Release(void * vInst);

void * Com_Init();
int Com_SetConfig(void * Inst, PCOM_INFO comInfo);
int Com_SendData(void *Inst, void *buff, int dataNum);
int Com_RecieveData(void *Inst, void *buff, int dataNum );
int Com_Release(void *Inst);
	





#endif
