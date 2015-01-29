#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <termios.h>
#include "video_api.h"
#include <linux/videodev2.h>
typedef struct termios termios;

typedef struct VIDEO_BUFF 
{
	void *start;
	unsigned int  length;
} VIDEO_BUFF ;

typedef struct VIDEO_INFO
{
	int fd;
	int buffIndex;
	VIDEO_BUFF  videoBuffer[BUFFNUM];
	VIDEO_PARAM videoParam;
}VIDEO_INFO, *PVIDEO_INFO;

typedef struct COM_INST
{
	int fd;
	COM_INFO comInfo;
}COM_INST, *PCOM_INST;

#endif
