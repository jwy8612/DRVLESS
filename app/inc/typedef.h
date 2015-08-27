#ifndef TYPEDEF_H
#define TYPEDEF_H

#define VIMICRO
#define picWd 640
#define picHt 480
#define dstWd 320
#define dstHt 240
#define srcSize picWd * picHt*2
#define dstSize 320*240
//#define gradThrea 30 

#define directX 0
#define directY 1

#define PI 3.14

#define YUV_Y 0
#define YUV_U 1
#define YUV_V 2


typedef  unsigned char u8;
typedef  signed char i8;
typedef  unsigned int u32;
typedef  signed int i32;


typedef struct MOTOR_CMD_INFO
{
	u8 bMoterCmd;
	u8 cmdtype;
	i8 motorV;
}MOTOR_CMD_INFO, *PMOTOR_CMD_INFO;
typedef struct SERVO_CMD_INFO
{
	u8 bservoCmd;
	u8 cmdtype;
	unsigned short servoA;
	unsigned short servoV;
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
	u8 blightCmd;
	LIGHT_CMD light;
}LIGHT_CMD_INFO, *PLIGHT_CMD_INFO;

typedef struct HORN_CMD_INFO
{
	u8 bhornCmd;
	HORN_CMD horn;
}HORN_CMD_INFO, *PHORN_CMD_INFO;

typedef struct COMMAND_INFO
{
	MOTOR_CMD_INFO motorCmd;
	SERVO_CMD_INFO servoCmd;
	LIGHT_CMD_INFO lgtCmd;
	HORN_CMD_INFO hornCmd;
	void *comInst;
	u8 commadData[50];
	u8 dataLength;
}COMMAND_INFO,*PCOMMAND_INFO;


#endif

