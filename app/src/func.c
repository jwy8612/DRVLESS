/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-02-03

    Descriptions : 功能函数
  
    Version             Description             Date                Author
    0.1                 Created                 2015-02-03       姜万勇

---------------------------------------------------------------------------------------------*/
#include "debug.h"
#include <stdio.h>
#include "video_api.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"

int getCmd(void *Inst, PCOMMAND_INFO cmdInfo)
{
	int ret = 0;
	char index = 0;
	int lenpos;
	PMOTOR_CMD_INFO motorCmd 	= &(cmdInfo->motorCmd);
	PSERVO_CMD_INFO servoCmd 	= &(cmdInfo->servoCmd);
	PLIGHT_CMD_INFO lgtCmd 		= &(cmdInfo->lgtCmd);
	PHORN_CMD_INFO hornCmd 	= &(cmdInfo->hornCmd);
	char *cmdbuff 				= cmdInfo->commadData;
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
				run_err("unsupported motor cmd type!!!\n");
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
				memcpy(cmdbuff + index, &(servoCmd->servoA), 2);
				index += 2;
				memcpy(cmdbuff + index, &(servoCmd->servoV), 2);
				index += 2;
				break;
			default :
				cmdbuff[index] = servoCmd->cmdtype;
				index ++;
				run_err("unsupported servo cmd type!!!\n");
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
				cmdbuff[index] = (char)lgtCmd->light;
				index ++;
				run_err("unsupported light cmd type!!!\n");
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
				cmdbuff[index] = (char)hornCmd->horn;
				index ++;
				run_err("unsupported light cmd type!!!\n");
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
	if(ret != index)
	{
		run_err("com send cmd err!!!\n");
	}
	function_out();
	return ret;
}

/////////YUYV to 4:0:0////////////
int picFmtTrans(int wd, int ht, char *srcbuff, char * dstbuff)
{
	int ret = 0;
	int i, j;
	
	function_in();
	for(i = 0; i < ht; i ++)
	{
		for(j = 0; j < wd; j ++)
		{
			dstbuff[i * wd + j] = srcbuff[i * wd + j * 2];
		}
	}
	function_out();
	return ret;
}

int picGrad(int wd, int ht, char *picbuff)
{
	int ret = 0;
	int i, j;
	signed char gx, gy;
	function_in();
	for(i = 0; i < ht - 1; i ++)
	{
		for(j = 0; j < wd - 1; j ++)
		{
			gx = picbuff[i * wd + j] - picbuff[(i +1)* wd + j +1];
			gy = picbuff[i * wd + j +1] - picbuff[(i +1)* wd + j];
			picbuff[i * wd + j] = abs(gx) + abs(gy);
		}
		picbuff[i * wd + wd] = picbuff[i * wd + wd - 1];
	}
	for(j = 0; j < wd; j ++)
	{
		picbuff[(ht - 1) * wd + j] = picbuff[(ht - 2) * wd + j];
	}

	function_out();
	return ret;
}
