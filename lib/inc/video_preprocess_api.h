#ifndef VIDEO_REPROCESS_API_H
#define VIDEO_REPROCESS_API_H

#define PI 3.14

typedef enum
{
	YUV_Y = 0,
	YUV_U,
	YUV_V,
}YUV_SUB;
signed int picFmtTrans(i32 wd, i32 ht, u8 *srcbuff, u8 * dstbuff, YUV_SUB type);
signed int picGrad(i32 wd, i32 ht, u8 *picbuff, u8 gradThrea);
signed int picFilterAverage(i32 wd, i32 ht, u8 *picbuff);
signed int picFilterMiddle(i32 wd, i32 ht, u8 *picbuff);
signed int picGetThreshold(u8 *picbuff, i32 wd, i32 ht, i32 startLine, i32 startRow);
void picBinary(u8 *picbuff, i32 wd, i32 ht, u8 Threshold);

#endif
