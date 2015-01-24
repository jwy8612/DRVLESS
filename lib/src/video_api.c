/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-07

    Descriptions : 视频采集串口控制api
  
    Version             Description             Date                Author
    0.1                 Created                 2015-01-07        姜万勇

---------------------------------------------------------------------------------------------*/
#include <fcntl.h>     
#include "typedef.h"
#include "debug.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "video_api.h"
#include <string.h>



#define test 1

void *Video_Init()
{
	PVIDEO_INFO videoInfo = NULL;
	int fd;

	function_in();
	videoInfo = (PVIDEO_INFO)malloc(sizeof(VIDEO_INFO));
	fd = open("/dev/video0",O_RDWR);   
	if(fd < 0)
	{
		video_err("open video0 failed!!!\n");
		goto exit;
	}
	videoInfo->fd = fd;
	
	function_out();
	exit:
	return videoInfo;

}
int Video_SetConfig()
{
	int ret = 0;

	function_in();


	function_out();
	return ret;

}
int Video_GetConfig(void * vInst)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	v4l2_capability *cap = &(videoInfo->videoCap);
	v4l2_fmtdesc  *fmtdesc = &(videoInfo->videoFmt);     

	function_in();
 	ret = ioctl(videoInfo->fd, VIDIOC_QUERYCAP, cap);   //查询设备属性
	if(ret < 0)
	{
		video_err("get videoCap failed![err %d]\n",ret);
		goto exit;
	}
  	video_dbg("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",  
            cap->driver,cap->card,cap->bus_info,(cap->version>>16)&0XFF, (cap->version>>8)&0XFF,cap->version&0XFF);    
   	fmtdesc->index=0;     
	fmtdesc->type=V4L2_BUF_TYPE_VIDEO_CAPTURE;     
#if test
	video_dbg("Support format:\n");     
	while(ioctl(videoInfo->fd,VIDIOC_ENUM_FMT,fmtdesc)!=-1)     //获取当前驱动支持的视频格式
	{     
		video_dbg("\t%d.%s\n",fmtdesc->index+1,fmtdesc->description);     
		fmtdesc->index++;  
   	 }  

#endif	
	function_out();
exit: return ret;

}
int Video_GetFrame()
{
	int ret = 0;

	function_in();


	function_out();
	return ret;
}
int Video_Release(void * vInst)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;

	function_in();
	
	if(videoInfo != NULL)
	{
		if(videoInfo->fd > 0)
		close(videoInfo->fd);// 关闭设备
		else
		{
			video_err("video not open!!!\n");
			ret = -1;
		}
		free(videoInfo);
	}
	
	else
	{
		video_err("videoInst not malloc!!!\n");
		ret = -2;
	}
	function_out();
	return ret;

}

void * Com_Init()
{
	int fd;
	int ret = 0;
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
	exit:
	return comInst;

}

int Com_SetConfig(void * Inst, PCOM_INFO comInfo)
{
	int ret = 0;
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
  	termIos.c_cc[VMIN]  = 0;
	tcflush(comInst->fd,TCIOFLUSH);
	tcsetattr(comInst->fd,TCSANOW,&termIos);	
	function_out();
 	exit:
	return ret;
}
	
int Com_SendData(void *Inst, void *buff, int dataNum)
{
	int ret = 0;
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

int Com_RecieveData(void *Inst, void *buff, int dataNum )
{
	int ret = 0;
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
int Com_Release(void *Inst)
{
	int ret = 0;
	PCOM_INST comInst = (PCOM_INST)Inst;

	function_in();
	
	if(comInst != NULL)
	{
		if(comInst->fd > 0)
		close(comInst->fd);// 关闭设备     
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
