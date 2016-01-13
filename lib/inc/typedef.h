#ifndef APPTYPEDEF_H
#define APPTYPEDEF_H
#include <termios.h>
#include "video_get_api.h"
#include <linux/videodev2.h>
#include "order_send_api.h"


#define PI 3.14
typedef struct termios termios;

typedef  unsigned char u8;
typedef  signed char i8;
typedef  unsigned int u32;
typedef  signed int i32;
typedef  signed short i16;
typedef  unsigned short u16;
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
