#ifndef FUNC_H
#define FUNC_H

#include "video_api.h"
#include "typedef.h"

int getCmd(void *Inst, PCOMMAND_INFO cmdInfo);
int picFmtTrans(int wd, int ht, char *srcbuff, char * dstbuff);
int picGrad(int wd, int ht, char *picbuff);


#endif
