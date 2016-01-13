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
	
	comInst = Com_Init();
	if(comInst!=NULL)
	{
		comInfo.baundRate = BR115200;
		comInfo.dataNum = DN8;
		comInfo.parity = NOP;
		comInfo.stop = S1;
		ret = Com_SetConfig(comInst, &comInfo);
		if(ret < 0)
		{
			run_err("com set failed,ret = %d\n",ret);
		}
	}

	videoInst = Video_Init();

	if(videoInst!=NULL)
	{
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
	}
	function_out();

	return ret;
	
}

i32 devRelease()
{
	i32 ret = 0;

	function_in();
	ret = Com_Release(comInst);
	comInst= NULL;
	if(ret < 0)
	{
		run_err("com release failed,ret = %d\n",ret);
		
	}
	ret = Video_Release(videoInst);
	videoInst=NULL;
	if(ret < 0)
	{
		run_err("video release failed,ret = %d\n",ret);
		
	}

	function_out();
	return ret;

}

void carDrive(i32 pTh, i32 pR, PCOMMAND_INFO cmdInfo)
{
	function_in();
	cmdInfo->motorCmd.bMoterCmd=1;
	cmdInfo->motorCmd.cmdtype=1;
	cmdInfo->motorCmd.motorV=50;

	cmdInfo->servoCmd.bservoCmd = 1;
	cmdInfo->servoCmd.cmdtype = 1;
	cmdInfo->servoCmd.servoA = 0;
	cmdInfo->servoCmd.servoV = 0;
		
	function_out();
}
i32 main(i32 argc, u8 ** argv)
{
	i32 ret = 0;
	i32 framenum = 0;
	i32 fd;
	fd_set fds;
	struct timeval time;
	VIDEO_BUFF vBuff;
	FILE *file=NULL;
	FILE *filein = NULL;
	FILE *fileout=NULL;
	COMMAND_INFO cmdInfo;
	u8 srcBuff[srcSize];
	u8 dstBuff[dstSize];
	u8 UBuff[dstSize];
	u8 VBuff[dstSize];
	u8 processBuff[dstSize];
	i32 pTh,pR;
	i32 direc, pos;
	u8 threshold;
	u8 testName[20];
	i32 testValue;
	i32 testValue1;
		
	function_in();
	memset(&cmdInfo,0,sizeof(COMMAND_INFO));
	
	ret = devinit();
	if(ret < 0)
	{
		run_err("dev init failed,ret = %d\n",ret);
	}

/*************功能测试*******************************/
	if(argc > 1)
	{
		strcpy(testName,argv[1]);
		if(argc>2)
			testValue = atoi(argv[2]);
		if(argc > 3)
			testValue1= atoi(argv[3]);

		/*************电机测试********/
		if(!strcmp(testName,"motortest"))
		{
			cmdInfo.motorCmd.bMoterCmd=1;
			cmdInfo.motorCmd.cmdtype=1;
			cmdInfo.motorCmd.motorV=testValue;

			getCmd(comInst, &cmdInfo);
		}

		/*************舵机测试********/
		if(!strcmp(testName,"servotest"))
		{
			cmdInfo.servoCmd.bservoCmd = 1;
			cmdInfo.servoCmd.cmdtype = 1;
			cmdInfo.servoCmd.servoA = testValue;
			cmdInfo.servoCmd.servoV = testValue1;
		
			getCmd(comInst, &cmdInfo);
		}
		
		/*************图像测试********/
		if(!strcmp(testName,"videotest"))
		{
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
			while(framenum < 350)
			{
			
				fread(srcBuff, 1, srcSize, filein);
				picFmtTrans(picWd, picHt, srcBuff, dstBuff, YUV_Y);
				picFilterAverage(dstWd, dstHt, dstBuff);
				threshold = picGetThreshold(dstBuff,dstWd,dstHt,100,50);
				picBinary(dstBuff, dstWd, dstHt, threshold);
				picLinePre(dstBuff,dstWd,dstHt);
				
				pic_Hough(dstBuff,dstWd,dstWd,&pR,&pTh);
				
				fwrite(dstBuff, 1,dstSize, fileout);
				framenum ++;
			}
			
			if(filein)
				fclose(filein);
			if(fileout)
				fclose(fileout);
		}
		
	}
	
/******************************************/
/*************正常运行*******************************/
	if(argc == 1)
	{
		if(videoInst==NULL)
		{
			run_err("videoinst is null!!!!\n");
			goto exit;
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
				picFmtTrans(picWd, picHt, (u8*)vBuff.start, dstBuff, YUV_Y);
		
		
				fwrite(srcBuff, 1, srcSize, file);
				framenum ++;
				if(framenum == FRAMENUM)
				{
					goto exit;
				}
			}
		}
	}
/******************************************/

//	cmdInfo->comInst = comInst;
//	comGetInit(comInst);

exit:
	
	ret = devRelease();
	if(ret < 0)
	{
		run_err("dev release failed,ret = %d\n",ret);
	}



	function_out();

	
	return ret;
}	

