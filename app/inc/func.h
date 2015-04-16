#ifndef FUNC_H
#define FUNC_H

#include "video_api.h"
#include "typedef.h"

int getCmd(void *Inst, PCOMMAND_INFO cmdInfo);
int picFmtTrans(int wd, int ht, char *srcbuff, char * dstbuff);
int picGrad(int wd, int ht, char *picbuff);
int picFilter(int wd, int ht, char *picbuff, char flag);
void Hough(char *srcbuff,int wd,int ht, int *pR, int *pTh, int iThreshold);

#endif
