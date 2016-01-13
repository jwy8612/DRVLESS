#ifndef ORDER_SEND_API_H
#define ORDER_SEND_API_H

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
	unsigned char bMoterCmd;
	unsigned char cmdtype;
	signed char motorV;
}MOTOR_CMD_INFO, *PMOTOR_CMD_INFO;
typedef struct SERVO_CMD_INFO
{
	unsigned char bservoCmd;
	unsigned char cmdtype;
	signed char servoA;
	signed char servoV;
}SERVO_CMD_INFO, *PSERVO_CMD_INFO;

typedef enum LIGHT_CMD
{
	ximie = 0,
	changliang,
}LIGHT_CMD;

typedef enum HORN_CMD
{
	quite = 0,
	chang,
	duan,
	dduan,
	hornBUT =4,
}HORN_CMD;
typedef struct LIGHT_CMD_INFO
{
	unsigned char blightCmd;
	LIGHT_CMD light;
}LIGHT_CMD_INFO, *PLIGHT_CMD_INFO;

typedef struct HORN_CMD_INFO
{
	unsigned char bhornCmd;
	HORN_CMD horn;
}HORN_CMD_INFO, *PHORN_CMD_INFO;

typedef struct COMMAND_INFO
{
	MOTOR_CMD_INFO motorCmd;
	SERVO_CMD_INFO servoCmd;
	LIGHT_CMD_INFO lgtCmd;
	HORN_CMD_INFO hornCmd;
	void *comInst;
	unsigned char commadData[50];
	unsigned char dataLength;
}COMMAND_INFO,*PCOMMAND_INFO;


void * Com_Init();
signed int Com_SetConfig(void * Inst, PCOM_INFO comInfo);
signed int Com_SendData(void *Inst, void *buff, signed int dataNum);
signed int Com_RecieveData(void *Inst, void *buff, signed int dataNum );
signed int Com_GetFd(void * Inst);
signed int Com_Release(void *Inst);
signed int getCmd(void *Inst, PCOMMAND_INFO cmdInfo);

#endif
