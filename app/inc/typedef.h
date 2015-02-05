#ifndef TYPEDEF_H
#define TYPEDEF_H

#define VIMICRO
#define picWd 640
#define picHt 480
#define picSize picWd * picHt
typedef struct MOTOR_CMD_INFO
{
	char bMoterCmd;
	char cmdtype;
	signed char motorV;
}MOTOR_CMD_INFO, *PMOTOR_CMD_INFO;
typedef struct SERVO_CMD_INFO
{
	char bservoCmd;
	char cmdtype;
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
	char blightCmd;
	LIGHT_CMD light;
}LIGHT_CMD_INFO, *PLIGHT_CMD_INFO;

typedef struct HORN_CMD_INFO
{
	char bhornCmd;
	HORN_CMD horn;
}HORN_CMD_INFO, *PHORN_CMD_INFO;

typedef struct COMMAND_INFO
{
	MOTOR_CMD_INFO motorCmd;
	SERVO_CMD_INFO servoCmd;
	LIGHT_CMD_INFO lgtCmd;
	HORN_CMD_INFO hornCmd;
	void *comInst;
	char commadData[50];
	char dataLength;
}COMMAND_INFO,*PCOMMAND_INFO;


#endif

