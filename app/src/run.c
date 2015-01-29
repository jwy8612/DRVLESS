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
	Video_Show_STD(videoInst);
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
int main(void)
{
	int ret = 0;
	int i;
	
	function_in();
	ret = devinit();
	if(ret < 0)
	{
		run_err("dev init failed,ret = %d\n",ret);
	}

//	Video_GetFrame(videoInst);
#if COMTEST
	for(i = 0; i < 10; i ++)
	{
		ret = Com_SendData(comInst,combuff, 20);
		if(ret < 20)
		{
			run_err("send data failed,ret = %d\n",ret);
		}
	}
#endif
	ret = devRelease();
	if(ret < 0)
	{
		run_err("dev release failed,ret = %d\n",ret);
	}
	
	function_out();
	return ret;
}	
