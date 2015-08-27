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

void *Video_Init()
{
	PVIDEO_INFO videoInfo = NULL;
	i32 fd;

	function_in();
	videoInfo = (PVIDEO_INFO)malloc(sizeof(VIDEO_INFO));
	if(videoInfo == NULL)
	{
		video_err("PVIDEO_INFO malloc failed!!!");
		goto exit;
	}
	fd = open("/dev/video0",O_RDWR | O_NONBLOCK, 0);   
	if(fd < 0)
	{
		video_err("open video0 failed!!!fd = %d\n",fd);
		goto exit;
	}
	videoInfo->fd = fd;
	
	function_out();
	return videoInfo;
exit:
	return NULL;

}

i32 Video_Show_CAP(void * vInst)
{
	i32 ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_capability cap;

	function_in();
	memset(&cap, 0, sizeof(cap));
 	ret = ioctl(videoInfo->fd, VIDIOC_QUERYCAP, &cap);   
	if(ret < 0)
	{
		video_err("VIDIOC_QUERYCAP failed!!!errno = %d\n",errno);
	}
  	video_err("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",  
            cap.driver, cap.card, cap.bus_info, (cap.version >> 16) & 0XFF, (cap.version >> 8) & 0XFF, cap.version & 0XFF);    
  	video_err("capbility = %d \n",cap.capabilities);

	function_out();
	return ret;

}

i32 Video_Show_STD(void * vInst)
{
	i32 ret = 0;
	v4l2_std_id std = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	
	function_in();
	do {
		ret = ioctl(videoInfo->fd, VIDIOC_QUERYSTD, &std);
		} while (ret == -1 && errno == EAGAIN);
	if(ret < 0)
	{
		video_err("Get Video STD Failed!!!errno = %d\n",errno);     
	}
	switch (std) 
	{
		case V4L2_STD_NTSC:
			video_err("Support STD: V4L2_STD_NTSC~~~\n");     
			break;
		case V4L2_STD_PAL:
			video_err("Support STD: V4L2_STD_PAL~~~\n");     
			break;
		default :
			video_err("other V4L2_STD~~~std = %lx\n",(long unsigned int)std);    
	}

	function_out();
	return ret;
}

i32 Video_show_FMTDESC(void * vInst)
{
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	i32 ret = 0;
	struct v4l2_format fmt;
	struct v4l2_fmtdesc  fmtdesc;
	
	function_in();
	memset(&fmt, 0, sizeof(fmt));
	memset(&fmtdesc, 0, sizeof(fmtdesc));
	fmtdesc.index=0;     
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;     
	video_err("Support format:\n");     
	while(ioctl(videoInfo->fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)  
	{     
		video_err("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);     
		fmtdesc.index++;  
	}  

	function_out();
 	return ret;
}

i32 Video_SetConfig(void * vInst, VIDEO_PARAM *videoParam)
{
	i32 ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	PVIDEO_PARAM vParam = &(videoInfo->videoParam);
	struct v4l2_crop cropcap;
	struct v4l2_format fmt;

	function_in();
	memcpy(vParam, videoParam, sizeof(VIDEO_PARAM));
	memset(&cropcap, 0, sizeof(cropcap));
	memset(&fmt, 0, sizeof(fmt));
	if(vParam->vCrop.bCROP)
	{
		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cropcap.c.left = vParam->vCrop.left;
		cropcap.c.top= vParam->vCrop.top;
		cropcap.c.width = vParam->vCrop.width;
		cropcap.c.height= vParam->vCrop.height;
		ret = ioctl(videoInfo->fd, VIDIOC_S_CROP, cropcap);
		if(ret < 0)
		{
			video_err("set video crop failed!!!errno = %d\n",errno);
			goto exit;
		}
	}

	switch(vParam->vFmt.pixFmt)
	{
		case YUYV8bit :
			
			fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
			ret = ioctl(videoInfo->fd, VIDIOC_TRY_FMT, &fmt);
			if(ret < 0)
			{
				if(errno == EINVAL)
					video_err("not support format V4L2_PIX_FMT__YUYV!!!\n");
				else
					video_err("VIDIOC_TRY_FMT failed!!!errno = %d\n",errno);
				goto exit;
			} 
			else
			{
				video_err("support format V4L2_PIX_FMT__YUYV~~~\n");
			}
			break;
		default :
			video_err("unsported pixfmt");
			goto exit;
	}
	fmt.fmt.pix.width = vParam->vFmt.width;
	fmt.fmt.pix.height = vParam->vFmt.height;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret = ioctl(videoInfo->fd, VIDIOC_S_FMT, &fmt);
	if(ret < 0)
	{
		video_err("set video fmt failed!!!errno = %d\n",errno);
		goto exit;
	}
	
	function_out();
exit:	
	return ret;
}

i32 Video_Showt_CurrentFMT(void * vInst)
{	
	struct v4l2_format fmt;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_crop cropcap; 
	i32 ret = 0;

	function_in();
	memset(&cropcap, 0, sizeof(cropcap));
	memset(&fmt, 0, sizeof(fmt));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(videoInfo->fd, VIDIOC_G_CROP, &cropcap);
	if (ret < 0)
		video_err("not support crop!!!errno = %d\n",errno);
	else
		video_err("dev w = %d,h = %d \n",  cropcap.c.width, cropcap.c.height); 
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(videoInfo->fd, VIDIOC_G_FMT, &fmt);
	if(ret < 0)
		video_err("VIDIOC_G_FMT failed!!!errno = %d\n",errno);
	else
		video_err("Current data format information:\n\twidth:%d\n\theight:%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
	
	function_out();
	return ret;
}


i32 Video_BuffersInit(void * vInst)
{
	i32 ret = 0;
	i32 i;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_buffer    buf;
	VIDEO_BUFF *buffers = videoInfo->videoBuffer;
	struct v4l2_requestbuffers req;
	function_in();

	memset(&req, 0, sizeof(req));
	memset(&buf, 0, sizeof(buf));
	req.count = BUFFNUM;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	req.memory = V4L2_MEMORY_MMAP; 
	ret = ioctl(videoInfo->fd,VIDIOC_REQBUFS,&req);
	if(ret < 0)
	{
		video_err("request buffers failed!!!errno = %d\n",errno);
		goto exit;
	}
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	for (i = 0; i < BUFFNUM; i ++) 
	{
		buf.index = i;
		ret = ioctl(videoInfo->fd, VIDIOC_QUERYBUF, &buf);
		if(ret < 0)
		{
			video_err("get buffers addr failed!!!errno = %d\n",errno);
			goto exit;
		}
		buffers[i].length = buf.length;
		video_dbg("buf.length = %d\n",buf.length);
		buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, videoInfo->fd, buf.m.offset);
		if (buffers[i].start == MAP_FAILED) 
		{
			video_err("mmap failed!!!\n");
			goto exit;
		}
		ret = ioctl(videoInfo->fd, VIDIOC_QBUF, &buf);
		if(ret < 0) 
		{
			video_err("QBUF failed!!!errno = %d\n",errno);
			goto exit;
		}
	}
	function_out();
exit:	
	return ret;
}


i32 Video_StartCapture(void * vInst)
{
	i32 ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	enum v4l2_buf_type type;
	
	function_in();
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(videoInfo->fd, VIDIOC_STREAMON, &type);
	if(ret < 0)
	{					 
		video_err("VIDIOC_STREAMON failed!!!errno = %d\n",errno);     
	}
	
	function_out();
	return ret;
}
i32 Video_GetFd(void * vInst)
{
	i32 ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	
	function_in();
	ret = videoInfo->fd;
	
	function_out();
	return ret;

}

i32 Video_GetFrame(void * vInst, VIDEO_BUFF *vBuff)
{
	i32 ret = 0;
	PVIDEO_INFO videoInfo = (PVIDEO_INFO)vInst;
	struct v4l2_buffer buf;
	
	function_in();
	memset(&buf,0,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = videoInfo->buffIndex;
	ret = ioctl(videoInfo->fd,VIDIOC_DQBUF,&buf);
	if(ret < 0)
	{
		video_err("VIDIOC_DQBUF failed!!!\n");
	}
	video_dbg("index = %d \nlength: %d \n",videoInfo->buffIndex,videoInfo->videoBuffer[videoInfo->buffIndex].length);
	vBuff->start = videoInfo->videoBuffer[videoInfo->buffIndex].start;
	vBuff->length = videoInfo->videoBuffer[videoInfo->buffIndex].length;
	
	ret = ioctl(videoInfo->fd,VIDIOC_QBUF,&buf);
	if(ret < 0)
	{
		video_err("VIDIOC_QBUF failed!!!\n");
	}
	videoInfo->buffIndex ++;
	if(videoInfo->buffIndex == BUFFNUM)
	{
		videoInfo->buffIndex = 0;
	}

	function_out();
	return ret;
}
i32 Video_Release(void * vInst)
{
	i32 ret = 0;
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
	exit:
	return comInst;

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
