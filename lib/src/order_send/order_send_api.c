#include <fcntl.h>     
#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"
#include <math.h>

void * Com_Init()
{
	i32 fd;
	i32 ret = 0;
	PCOM_INST comInst = NULL;
	
	function_in();
	comInst = (PCOM_INST)malloc(sizeof(COM_INST));
	fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);   
	if(fd <= 0)
	{
		video_err("open ttyusb0 failed!!!\n");
		goto exit;
	}
	comInst->fd = fd;
	ret = fcntl(fd, F_SETFL, FNDELAY);
	if(ret  <0)
	{
		video_err("fcntl failed!\n");
	}
	video_dbg("fcntl = %d\n",fcntl(fd, F_SETFL,FNDELAY));
	if(isatty(STDIN_FILENO) == 0)
	{
		video_err("standard input is not a terminal device\n");
	}
	video_err("isatty success!\n");
	video_dbg("fd-open = %d\n",fd);
	
	function_out();
	return comInst;
	exit:
	return NULL;

}

i32 Com_SetConfig(void * Inst, PCOM_INFO comInfo)
{
	i32 ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;
	PCOM_INFO comParam = &(comInst->comInfo);
	termios termIos;
	function_in();

	memcpy(comParam,comInfo,sizeof(COM_INFO));
	tcgetattr(comInst->fd,&termIos);
	termIos.c_cflag |= CLOCAL | CREAD;

	switch(comParam->baundRate)
	{
		case BR9600:
		{
       		 cfsetispeed(&termIos,B9600);
  			 cfsetospeed(&termIos,B9600); 
			 break;
		}
		case BR19200:
		{
       		 cfsetispeed(&termIos,B19200);
  			 cfsetospeed(&termIos,B19200); 
			 break;

		}
		case BR38400:
		{
       		 cfsetispeed(&termIos,B38400);
  			 cfsetospeed(&termIos,B38400); 
			 break;
		}
		case BR115200:
		{
       		 cfsetispeed(&termIos,B115200);
  			 cfsetospeed(&termIos,B115200); 
			 break;
		}
		default:
			video_err("unsuported baund!!!\n");
			ret = -1;
			goto exit;
			
	}
	switch(comParam->dataNum)
	{
		case DN7:
		{
			termIos.c_cflag &= ~CSIZE;
			termIos.c_cflag |= CS7;
			break;
		}
		case DN8:
		{
			termIos.c_cflag &= ~CSIZE;
			termIos.c_cflag |= CS8;
			break;
		}
		default:
			video_err("unsuported data num!!!\n");
			ret = -2;
			goto exit;

	}
	switch(comParam->parity)
	{
		case NOP:
		{
			termIos.c_cflag &= ~PARENB;
			break;
		}
		case ODDP:
		{
			termIos.c_cflag |= PARENB;
			termIos.c_cflag |= PARODD;
			termIos.c_iflag |= (INPCK | ISTRIP);
			break;
		}		
		case EVENP:
		{
			termIos.c_cflag |= PARENB;
			termIos.c_cflag &= ~PARODD;
			termIos.c_iflag |= (INPCK | ISTRIP);
			break;
		}	
		default:
			video_err("unsuported parity!!!\n");
			ret = -3;
			goto exit;


	}
	switch(comParam->stop)
	{
		case S1:
		{
			termIos.c_cflag &= ~CSTOPB;
			break;
		}
		case S2:
		{
			termIos.c_cflag |= CSTOPB;
			break;
		}	
		default:
			video_err("unsuported stop num!!!\n");
			ret = -3;
			goto exit;

	}
 	termIos.c_cc[VTIME] = 0;
  	termIos.c_cc[VMIN]  = 10;
	tcflush(comInst->fd,TCIOFLUSH);
	tcsetattr(comInst->fd,TCSANOW,&termIos);	
	function_out();
 	exit:
	return ret;
}
	
i32 Com_SendData(void *Inst, void *buff, i32 dataNum)
{
	i32 ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;
	
	function_in();
	ret = write(comInst->fd, buff,dataNum);
	if(ret < 0)
	{
		video_err("write data to com err!!!\n");
	}

	function_out();
	return ret;
}

i32 Com_RecieveData(void *Inst, void *buff, i32 dataNum )
{
	i32 ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;
	
	function_in();
	ret = read(comInst->fd, buff,dataNum);
	if(ret < 0)
	{
		video_err("write data to com err!!!\n");
	}

	function_out();
	return ret;
}

i32 Com_GetFd(void * Inst)
{
	i32 ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;
	
	function_in();
	ret = comInst->fd;
	
	function_out();
	return ret;

}

i32 Com_Release(void *Inst)
{
	i32 ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;

	function_in();
	
	if(comInst != NULL)
	{
		if(comInst->fd > 0)
		close(comInst->fd);// ¹Ø±ÕÉè±¸     
		else
		{
			video_err("com not open!!!\n");
			ret = -1;
		}
		free(comInst);
	}
	else
	{
		video_err("comInst not malloc!!!\n");
		ret = -2;
	}
	function_out();
	return ret;

}
i32 getCmd(void *Inst, PCOMMAND_INFO cmdInfo)
{
	i32 ret = 0;
	u8 index = 0;

	u16 serA,serV;
	i32 lenpos;
	PMOTOR_CMD_INFO motorCmd 	= &(cmdInfo->motorCmd);
	PSERVO_CMD_INFO servoCmd 	= &(cmdInfo->servoCmd);
	PLIGHT_CMD_INFO lgtCmd 		= &(cmdInfo->lgtCmd);
	PHORN_CMD_INFO hornCmd 	= &(cmdInfo->hornCmd);
	u8 *cmdbuff 				= cmdInfo->commadData;
	void * comInst = Inst;
	function_in();
	cmdbuff[index] = 0xff;
	index ++;
	lenpos = index;
	index ++;
	if(motorCmd->bMoterCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(motorCmd->cmdtype)
		{
			case 1 :
				cmdbuff[index] = motorCmd->cmdtype;
				index ++;
				cmdbuff[index] = motorCmd->motorV;
				index ++;
				break;
			default :
				cmdbuff[index] = motorCmd->cmdtype;
				index ++;
				video_err("unsupported motor cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	if(servoCmd->bservoCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(servoCmd->cmdtype)
		{
			case 1 :
				cmdbuff[index] = servoCmd->cmdtype;
				index ++;
				serA = servoCmd->servoA*512/100+512;
				memcpy(cmdbuff + index, &(serA), 2);
				index += 2;
				serV= servoCmd->servoV*512/100+512;
				memcpy(cmdbuff + index, &(serV), 2);
				index += 2;
				break;
			default :
				cmdbuff[index] = servoCmd->cmdtype;
				index ++;
				video_err("unsupported servo cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	if(lgtCmd->blightCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(lgtCmd->light)
		{
			case ximie :
				cmdbuff[index] = 0;
				index ++;
				break;
			case changliang :
				cmdbuff[index] = 1;
				index ++;
				break;
			default :
				cmdbuff[index] = (u8)lgtCmd->light;
				index ++;
				video_err("unsupported light cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	
	if(hornCmd->bhornCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(hornCmd->horn)
		{
			case quite:
				cmdbuff[index] = 0;
				index ++;
				break;
			case chang :
				cmdbuff[index] = 1;
				index ++;
				break;
			case duan :
				cmdbuff[index] = 2;
				index ++;
				break;
			case dduan :
				cmdbuff[index] = 3;
				index ++;
				break;
			default :
				cmdbuff[index] = (u8)hornCmd->horn;
				index ++;
				video_err("unsupported light cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}	
	cmdbuff[index] = 0x80;
	cmdbuff[lenpos] = index - lenpos -1;
	cmdInfo->dataLength = index + 1;
	ret = Com_SendData(comInst, cmdbuff, cmdInfo->dataLength);
	if(ret != cmdInfo->dataLength)
	{
		video_err("com send cmd err!!!\n");
	}
	function_out();
	return ret;
}

