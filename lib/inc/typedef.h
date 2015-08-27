#ifndef APPTYPEDEF_H
#define APPTYPEDEF_H
#include <termios.h>
#include "video_api.h"
#include <linux/videodev2.h>
typedef struct termios termios;

typedef  unsigned char u8;
typedef  signed char i8;
typedef  unsigned int u32;
typedef  signed int i32;

typedef struct VIDEO_INFO
{
	i32 fd;
	i32 buffIndex;
	VIDEO_BUFF  videoBuffer[BUFFNUM];
	VIDEO_PARAM videoParam;
}VIDEO_INFO, *PVIDEO_INFO;

typedef struct COM_INST
{
	i32 fd;
	COM_INFO comInfo;
}COM_INST, *PCOM_INST;

#endif
