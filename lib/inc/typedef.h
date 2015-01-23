#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <linux/videodev2.h>
#include <termios.h>
#include "video_api.h"

typedef struct v4l2_capability v4l2_capability;
typedef struct v4l2_fmtdesc v4l2_fmtdesc;
typedef struct termios termios;
typedef struct VIDEO_INFO
{
	int fd;
	v4l2_capability videoCap;
	v4l2_fmtdesc videoFmt;
}VIDEO_INFO, *PVIDEO_INFO;

typedef struct COM_INST
{
	int fd;
	COM_INFO comInfo;
}COM_INST, *PCOM_INST;

#endif
