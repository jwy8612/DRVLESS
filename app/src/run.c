/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-08

    Descriptions : 无人驾驶车app
  
    Version             Description             Date                Author
    0.1                 Created                 2015-01-08       姜万勇

---------------------------------------------------------------------------------------------*/
#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"
#include "video_get_api.h"
#include "video_preprocess_api.h"
#include "video_process_api.h"
#include "order_send_api.h"
#define FRAMENUM 10

void * comInst = NULL;
void * videoInst = NULL;

#define	VIDEOTEST 0 
#define	COMTEST 0

u8 combuff[20]={1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1,0};

i32 devinit()
{
	i32 ret = 0;
	COM_INFO comInfo;
	VIDEO_PARAM videoParam;
	
	function_in();
	memset(&comInfo, 0, sizeof(comInfo));
	memset(&videoParam, 0, sizeof(videoParam));
	
#ifndef VIMICRO
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

	videoInst = Video_Init();
	Video_Show_CAP(videoInst);
	Video_show_FMTDESC(videoInst);

	videoParam.vCrop.bCROP = 0;
	videoParam.vFmt.pixFmt = YUYV8bit;
	videoParam.vFmt.width = picWd;
	videoParam.vFmt.height = picHt;
	
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

i32 devRelease()
{
	i32 ret = 0;

	function_in();
	ret = Com_Release(comInst);
	if(ret < 0)
	{
		run_err("com release failed,ret = %d\n",ret);
		
	}
	ret = Video_Release(videoInst);
	if(ret < 0)
	{
		run_err("video release failed,ret = %d\n",ret);
		
	}

	function_out();
	return ret;

}


i32 main(void)
{
	i32 ret = 0;
	i32 framenum = 0;
	i32 fd;
	fd_set fds;
	struct timeval time;
	VIDEO_BUFF vBuff;
	FILE *file;
	FILE *filein, *fileout;
	COMMAND_INFO cmdInfo;
	u8 srcBuff[srcSize];
	u8 dstBuff[dstSize];
	u8 UBuff[dstSize];
	u8 VBuff[dstSize];
	u8 processBuff[dstSize];
	i32 pTh,pR;
	i32 direc, pos;
	u8 threshold;
	
	
	function_in();
	
	file = fopen("/mnt/sdcard/test.yuv","wb+");
	if(file < 0)
	{
		run_err("open test.yuv failed!!!\n");
	}
#ifdef VIMICRO
	filein = fopen("/mnt/sdcard/bishe.yuv","rb+");
	if(file < 0)
	{
		run_err("open 400.yuv failed!!!\n");
	}
	
	fileout = fopen("/mnt/sdcard/out.yuv","wb+");
	if(file < 0)
	{
		run_err("open out.yuv failed!!!\n");
	}
#endif	
	ret = devinit();
	if(ret < 0)
	{
		run_err("dev init failed,ret = %d\n",ret);
	}

//	comGetInit(comInst);
#ifndef VIMICRO

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
			//picFmtTrans(picWd, picHt, vBuff.start, srcBuff);
			//fwrite(vBuff.start,1,vBuff.length,file);
			fwrite(srcBuff, 1, srcSize, file);
			framenum ++;
			if(framenum == FRAMENUM)
			{
				goto exit;
			}
		}
	}
#else

	while(framenum < 200)
	{
	
	fread(srcBuff, 1, srcSize, filein);
#if 1
	picFmtTrans(picWd, picHt, srcBuff, dstBuff, YUV_Y);
	picFilterAverage(dstWd, dstHt, dstBuff);
	threshold = picGetThreshold(dstBuff,dstWd,dstHt,100,50);
	picBinary(dstBuff, dstWd, dstHt, threshold);
	//picGrad(dstWd, dstHt, dstBuff, 20);
	fwrite(dstBuff, 1,dstSize, fileout);

#endif
#if 0
	picFmtTrans(picWd, picHt, srcBuff, UBuff, YUV_U);
	//picFilterMiddle(dstWd, dstHt, UBuff);
	//picGrad(dstWd, dstHt, UBuff, 10);
	fwrite(UBuff, 1,dstSize, fileout);

#endif
#if 0
	picFmtTrans(picWd, picHt, srcBuff, VBuff, YUV_V);
	//picFilterMiddle(dstWd, dstHt, UBuff);
	//picGrad(dstWd, dstHt, UBuff, 10);
	fwrite(VBuff, 1,dstSize, fileout);
#endif
//	
//	picFmtTrans(picWd, picHt, srcBuff, VBuff, YUV_V);
	//picDif(picWd, picHt, UBuff, VBuff, processBuff);
	//picFilter(dstWd, dstHt, dstBuff, directX);
	//picFilter(dstWd, dstHt, srcBuff, directY);
	//picGrad(dstWd, dstHt, processBuff, 20);
	//Hough(srcBuff, picWd, picHt, &pR, &pTh, 0);
//	
	//picFilter(picWd, picHt, srcBuff, directY);
	//picGrad(picWd, picHt, srcBuff, 100);
//	fwrite(UBuff, 1,dstSize, fileout);
//	fwrite(VBuff, 1,dstSize, fileout);
//	fwrite(dstBuff, 1,dstSize, fileout);
//	picGetLine(srcBuff, picWd, picHt, &direc, &pos);
	framenum ++;
	}
#if 0
	while(1)
	{
		run_err("test!!!\n");
		sleep(2);
	}
#endif
#endif

exit:
	
	ret = devRelease();
	if(ret < 0)
	{
		run_err("dev release failed,ret = %d\n",ret);
	}
	fclose(file);
	fclose(filein);
	fclose(fileout);

	function_out();
	return ret;
}	

