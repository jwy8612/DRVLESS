#ifndef VIDEO_GET_API_H
#define VIDEO_GET_API_H

#define BUFFNUM 4
typedef struct VIDEO_CROP 
{
	unsigned char bCROP;
	signed int top;
	signed int left;
	signed int width;
	signed int height;
} VIDEO_CROP;

typedef enum PIX_FMT
{
	YUYV8bit = 0,
	YUVBUT = 1,
}PIX_FMT;

typedef struct VIDEO_FMT 
{
	PIX_FMT pixFmt;
	signed int width;
	signed int height;
} VIDEO_FMT;
typedef struct VIDEO_PARAM 
{
	VIDEO_CROP vCrop;
	VIDEO_FMT vFmt;
} VIDEO_PARAM, *PVIDEO_PARAM;

typedef struct VIDEO_BUFF 
{
	void *start;
	unsigned int  length;
} VIDEO_BUFF ;




void *Video_Init();
signed int Video_Show_CAP(void * vInst);
signed int Video_Show_STD(void * vInst);
signed int Video_show_FMTDESC(void * vInst);
signed int Video_SetConfig(void * vInst, VIDEO_PARAM *videoParam);
signed int Video_Showt_CurrentFMT(void * vInst);
signed int Video_BuffersInit(void * vInst);
signed int Video_StartCapture(void * vInst);
signed int Video_GetFd(void * vInst);
signed int Video_GetFrame(void * vInst, VIDEO_BUFF *vBuff);
signed int Video_Release(void * vInst);


#endif
