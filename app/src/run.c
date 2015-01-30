/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-08

    Descriptions : 无人驾驶车app
  
    Version             Description             Date                Author
    0.1                 Created                 2015-01-08       姜万勇

---------------------------------------------------------------------------------------------*/
#include "debug.h"
#include <stdio.h>
#include "video_api.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"

#define FRAMENUM 500

void * comInst = NULL;
void * videoInst = NULL;

#define	VIDEOTEST 1 
#define	COMTEST 0

char combuff[20]={1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1,0};

int devinit()
{
	int ret = 0;
	COM_INFO comInfo;
	VIDEO_PARAM videoParam;
	
	function_in();
	memset(&comInfo, 0, sizeof(comInfo));
	memset(&videoParam, 0, sizeof(videoParam));
	
#if COMTEST
	comInst = Com_Init();
	comInfo.baundRate = BR115200;
	comInfo.dataNum = DN8;
	comInfo.parity = NOP;
	comInfo.stop = S1;
	ret = Com_SetConfig(comInst, &comInfo);
	if(ret < 0)
	{
		run_err("com set failed,ret = %d\n",ret);
	}
#endif	
#if VIDEOTEST
	videoInst = Video_Init();
	Video_Show_CAP(videoInst);
	Video_show_FMTDESC(videoInst);

	videoParam.vCrop.bCROP = 0;
	videoParam.vFmt.pixFmt = YUYV8bit;
	videoParam.vFmt.width = 640;
	videoParam.vFmt.height = 480;
	
	ret = Video_SetConfig(videoInst, &videoParam);
	if(ret < 0)
	{
		run_err("Video_SetConfig failed,ret = %d\n",ret);
	}
	Video_Showt_CurrentFMT(videoInst);
	ret = Video_BuffersInit(videoInst);
	if(ret < 0)
	{
		run_err("Video_Buffers Init failed,ret = %d\n",ret);
	}
#endif
	function_out();

	return ret;
	
}

int devRelease()
{
	int ret = 0;

	function_in();
	ret = Com_Release(comInst);
	if(ret < 0)
	{
		run_err("com release failed,ret = %d\n",ret);
		goto exit1;
	}
	ret = Video_Release(videoInst);
	if(ret < 0)
	{
		run_err("video release failed,ret = %d\n",ret);
		goto exit2;
	}

	function_out();
	exit1: return -1;
	exit2: return -2;

}

int getCmd(PCOMMAND_INFO cmdInfo)
{
	int ret = 0;
	int index = 0;
	int lenpos;
	PMOTOR_CMD_INFO motorCmd 	= &(cmdInfo->motorCmd);
	PSERVO_CMD_INFO servoCmd 	= &(cmdInfo->servoCmd);
	PLIGHT_CMD_INFO lgtCmd 		= &(cmdInfo->lgtCmd);
	PHORN_CMD_INFO hornCmd 		= &(cmdInfo->hornCmd);
	char *cmdbuff 				= cmdInfo->commadData;
	
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
	function_out();
	return ret;
}
int main(void)
{
	int ret = 0;
	int framenum = 0;
	int fd;
	fd_set fds;
	struct timeval time;
	VIDEO_BUFF vBuff;
	FILE * file;
	COMMAND_INFO cmdInfo;
	
	function_in();
	
	file = fopen("/mnt/sdcard/test.yuv","wb+");
	if(file < 0)
	{
		video_err("open test.yuv failed!!!\n");
	}
	ret = devinit();
	if(ret < 0)
	{
		run_err("dev init failed,ret = %d\n",ret);
	}
	Video_StartCapture(videoInst);
	fd = Video_GetFd(videoInst);
	while(1)
	{
		FD_ZERO(&fds);  
		FD_SET(fd, &fds); 
		time.tv_sec = 3;
		time.tv_usec = 0;

		ret = select(fd + 1, &fds, NULL, NULL, &time);
		if(ret < 0)
		{
			run_err("select failed!!!\n");
		}
		if(ret == 0)
		{
			run_err("select time out!!!\n");
		}
		if(ret > 0)
		{
			Video_GetFrame(videoInst, &vBuff);
		}	
		fwrite(vBuff.start,1,vBuff.length,file);
		framenum ++;
		if(framenum == FRAMENUM)
		{
			goto exit;
		}
	}

exit:
	
	ret = devRelease();
	if(ret < 0)
	{
		run_err("dev release failed,ret = %d\n",ret);
	}
	fclose(file);
	function_out();
	return ret;
}	
