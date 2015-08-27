#ifndef FUNC_H
#define FUNC_H

#include "video_api.h"
#include "typedef.h"

i32 getCmd(void *Inst, PCOMMAND_INFO cmdInfo);
i32 picFmtTrans(i32 wd, i32 ht, u8 *srcbuff, u8 * dstbuff, u8 type);
i32 picGrad(i32 wd, i32 ht, u8 *picbuff, u8 gradThrea);
i32 picFilterAverage(i32 wd, i32 ht, u8 *picbuff);
void Hough(u8 *srcbuff,i32 wd,i32 ht, i32 *pR, i32 *pTh, i32 iThreshold);
i32 picDif(i32 wd, i32 ht, u8 *picbuff1, u8 *picbuff2, u8 *picbuff3);
i32 picFilterMiddle(i32 wd, i32 ht, u8 *picbuff);
i32 picGetThreshold(u8 *picbuff, i32 wd, i32 ht, i32 startLine, i32 startRow);
void picBinary(u8 *picbuff, i32 wd, i32 ht, u8 Threshold);
#endif
