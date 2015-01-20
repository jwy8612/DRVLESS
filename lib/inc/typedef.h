#ifndef TYPEDEF_H
#define TYPEDEF_H
#include "videodev2.h"

#define NULL 0
typedef struct v4l2_capability v4l2_capability;
typedef struct v4l2_fmtdesc v4l2_fmtdesc;

typedef struct VIDEO_INFO
{
	int fd;
	v4l2_capability videoCap;
	v4l2_fmtdesc videoFmt;
}VIDEO_INFO, *PVIDEO_INFO;

#endif
