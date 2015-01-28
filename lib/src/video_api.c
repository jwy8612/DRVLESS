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
#include <errno.h>
#include<sys/mman.h>

#define test 1

void *Video_Init()
{
	PVIDEO_INFO videoInfo = NULL;
	int fd;

	function_in();
	videoInfo = (PVIDEO_INFO)malloc(sizeof(VIDEO_INFO));
	fd = open("/dev/video0",O_RDWR | O_NONBLOCK, 0);   
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

int Video_GetConfig_CAP(void * vInst,struct v4l2_capability *cap)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;

	function_in();
 	ret = ioctl(videoInfo->fd, VIDIOC_QUERYCAP, cap);   //查询设备属性
	if(ret < 0)
	{
		video_err("get videoCap failed![err %d]\n",ret);
		goto exit;
	}
  	video_dbg("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",  
            cap->driver,cap->card,cap->bus_info,(cap->version>>16)&0XFF, (cap->version>>8)&0XFF,cap->version&0XFF);    
  	video_dbg("capbility = %d \n",cap->capabilities);

	function_out();
exit: return ret;

}

int Video_GetConfig_FMT(void * vInst, struct v4l2_fmtdesc  *fmtdesc)
{
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	int ret = 0;
	struct v4l2_format fmt;
	function_in();
	fmtdesc->index=0;     
	fmtdesc->type=V4L2_BUF_TYPE_VIDEO_CAPTURE;     
	video_dbg("Support format:\n");     
	while(ioctl(videoInfo->fd,VIDIOC_ENUM_FMT,fmtdesc)!=-1)     //获取当前驱动支持的视频格式
	{     
		video_dbg("\t%d.%s\n",fmtdesc->index+1,fmtdesc->description);     
		fmtdesc->index++;  
	}  

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32; 
	if(ioctl(videoInfo->fd,VIDIOC_TRY_FMT,&fmt) ==-1)
		if(errno == EINVAL)
		video_dbg("not support format V4L2_PIX_FMT_RGB32\n");
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; 
	if(ioctl(videoInfo->fd,VIDIOC_TRY_FMT,&fmt) == -1)
		if(errno == EINVAL)
		video_dbg("not support format V4L2_PIX_FMT_YUV420P\n");
		
	function_out();
	return ret;
}

int Video_SetConfig_CROP(void * vInst, struct v4l2_crop *cropcap)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;

	function_in();

	ret = ioctl(videoInfo->fd, VIDIOC_S_CROP, cropcap);
	if(ret < 0)
	{
		video_err("set video crop failed!!!errno = %d\n",errno);
	}
	function_out();
	return ret;
}

int Video_SetConfig_FMT(void * vInst,struct v4l2_format *videoFmt)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;

	function_in();
	
	ret = ioctl(videoInfo->fd, VIDIOC_S_FMT, videoFmt);
	if(ret == -1)
	{
		video_err("set video fmt failed!!!errno = %d\n",errno);
	}

	function_out();
	return ret;
}
int Video_BuffersInit(void * vInst,struct v4l2_requestbuffers* req)
{
	int ret = 0;
	int i;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_buffer    buf;
	VIDEO_BUFF *buffers = videoInfo->videoBuffer;

	function_in();

	ret = ioctl(videoInfo->fd,VIDIOC_REQBUFS,req);
	if(ret < 0)
	{
		video_err("request buffers failed!!!errno = %d\n",errno);
	}

	for (i = 0; i < BUFFNUM; i ++) 
	{
		memset( &buf, 0, sizeof(buf) );
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		ret = ioctl(videoInfo->fd, VIDIOC_QUERYBUF, &buf);
		if(ret < 0)
		{
			video_err("get buffers addr failed!!!errno = %d\n",errno);
		}
		buffers[i].length = buf.length;
		video_err("buf.length = %d\n",buf.length);
		buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, videoInfo->fd, buf.m.offset);
		 
		if (buffers[i].start == MAP_FAILED) 
		{
			video_err("mmap failed!!!\n");
		}
#if 0
		ret = ioctl(videoInfo->fd, VIDIOC_QBUF, &buf);
		if(ret == -1) 
		{
			video_err("QBUF failed!!!errno = %d\n",errno);
		}
#endif
	}
	function_out();
	
	return ret;
}


int Video_GetCrrent_FMT(void * vInst)
{	
	struct v4l2_format fmt;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_crop cropcap; 
	int ret = 0;

	function_in();
	memset(&cropcap, 0, sizeof(cropcap));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(videoInfo->fd, VIDIOC_G_CROP, &cropcap);
	if (!ret) {
		video_dbg("dev w = %d,h = %d \n",  cropcap.c.width, cropcap.c.height); 
	}

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(videoInfo->fd, VIDIOC_G_FMT, &fmt);
	video_dbg("Current data format information:\n\twidth:%d\n\theight:%d\n",
				fmt.fmt.pix.width,fmt.fmt.pix.height);
	function_out();
	return ret;
}
int Video_GetConfig_STD(void * vInst,v4l2_std_id *std)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	function_in();
	do {
		ret = ioctl(videoInfo->fd, VIDIOC_QUERYSTD, std);
		} while (ret == -1 && errno == EAGAIN);
	if(ret < 0)
	{
		video_err("Get Video STD Failed!!!errno = %d\n",errno);     
	}
	switch (*std) 
	{
		case V4L2_STD_NTSC:
		video_dbg("Support STD: V4L2_STD_NTSC\n");     
		break;
		case V4L2_STD_PAL:
		video_dbg("Support STD: V4L2_STD_PAL\n");     
		break;
	}

	function_out();
	return ret;
}

int Video_GetFrame(void * vInst)
{
	int ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_buffer buf;
	enum v4l2_buf_type type;
	int i;
	FILE * file;
	function_in();
	
	memset(&buf,0,sizeof(buf));
	for(i = 0;i < 4; i++)
	{			
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;	
		buf.memory = V4L2_MEMORY_MMAP;	
		buf.index = i;			
		ret = ioctl(videoInfo->fd, VIDIOC_QBUF, &buf);
		if(ret == -1) 
		{
			video_err("QBUF failed!!!errno = %d\n",errno);
		}
	}

	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	ret = ioctl(videoInfo->fd, VIDIOC_STREAMON, &type);
	if(ret < 0)
	{					 
		video_err("VIDIOC_STREAMON failed!!!errno = %d\n",errno);     
	}

	memset(&buf,0,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = videoInfo->buffIndex;
	//读取缓存
	while(ioctl(videoInfo->fd,VIDIOC_DQBUF,&buf) < 0);
	//ret = ioctl(videoInfo->fd,VIDIOC_DQBUF,&buf);
//	if(ret < 0)
//	{
	//	video_err("VIDIOC_DQBUF failed!!!\n");
	//}
	file = fopen("/mnt/sdcard/test.yuv","wb+");
	if(file < 0)
	{
		video_err("open test.yuv failed!!!\n");
	}
	video_err("index = %d \nlength: %d \n",videoInfo->buffIndex,videoInfo->videoBuffer[videoInfo->buffIndex].length);
	fwrite(videoInfo->videoBuffer[videoInfo->buffIndex].start,1,videoInfo->videoBuffer[videoInfo->buffIndex].length,file);
	fclose(file);
	//重新放入缓存队列
	ret = ioctl(videoInfo->fd,VIDIOC_QBUF,&buf);
	if(ret < 0)
	{
		video_err("VIDIOC_QBUF failed!!!\n");
	}


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
