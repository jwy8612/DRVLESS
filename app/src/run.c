/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-08

    Descriptions : 无人驾驶车app
  
    Version             Description             Date                Author
    0.1                 Created                 2015-01-08       姜万勇

---------------------------------------------------------------------------------------------*/
#include"debug.h"
#include<stdio.h>
#include "video_api.h"

void * comInst = NULL;
void * videoInst = NULL;

#define	TEST 1 
#if TEST
char combuff[20]={1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1,0};
#endif

int devinit()
{
	int ret = 0;
	COM_INFO comInfo;
	v4l2_capability videoCap;
	v4l2_fmtdesc videoFmt;
	v4l2_std_id videoStd;
	v4l2_format fmt;

	function_in();

	comInst = Com_Init();
	comInfo.baundRate = BR115200;
	comInfo.dataNum = DN8;
	comInfo.parity = NOP;
	comInfo.stop = S1;
	ret = Com_SetConfig(comInst, &comInfo);
	if(ret < 0)
	{
		run_err("com set failed,ret = %d\n",ret);
		goto exit1;
	}
	#if 1
	videoInst = Video_Init();
	ret = Video_GetConfig_CAP(videoInst,&videoCap);
	if(ret < 0)
	{
		run_err("video get cap failed,ret = %d\n",ret);
		goto exit2;
	}
	ret = Video_GetConfig_FMT(videoInst,&videoFmt);
	if(ret < 0)
	{
		run_err("video get fmt failed,ret = %d\n",ret);
		goto exit2;
	}
	ret = Video_GetConfig_STD(videoInst,&videoStd);
	if(ret < 0)
	{
		run_err("video get std failed,ret = %d\n",ret);
		goto exit2;
	}
	#endif
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = 720;
	fmt.fmt.pix.height = 576;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	Video_SetConfig_FMT(videoInst,&videoFmt);

	return ret;
	exit1: return -1;
	exit2: return -2;
	
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
	for(i = 0; i < 10; i ++)
	{
		ret = Com_SendData(comInst,combuff, 20);
		if(ret < 20)
		{
			run_err("send data failed,ret = %d\n",ret);
		}
	}
	ret = devRelease();
	if(ret < 0)
	{
		run_err("dev release failed,ret = %d\n",ret);
	}
	
	function_out();
	return ret;
}	
