#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"
#include <math.h>


void Hough(u8 *srcbuff,i32 wd,i32 ht, i32 *pR, i32 *pTh, i32 iThreshold)
{
	i32 iRMax = (i32)sqrt(wd * wd + ht * ht) + 1;
	i32 iThMax = 361;
	i32 iTh, iR;
	i32 iMax = -1;
    	i32 iThMaxIndex = -1;
    	i32 iRMaxIndex = -1;
    	i32 pArray[iRMax * iThMax];
    	float fRate = (float)(PI/180);
	i32 i,j;	
	 i32 iCount ;

	memset(pArray, 0, sizeof(i32) * iRMax * iThMax);

	for (i = 0; i < ht; i++)
	{
		for (j = 0; j < wd; j++)
		{
			 if(srcbuff[i * wd + j] == 255)
			{
				for(iTh = 0; iTh < iThMax; iTh ++)
	                	{
					iR = (i32)(j * cos(iTh * fRate) + i * sin(iTh * fRate));
				//	video_err("IR = %d\n",iR);
					if(iR > 0)
					{
	                       		 pArray[iR * iThMax + iTh]++;
						//		 	

					}
	                	}
	            	}

	        } 
			 video_err("mask\n");

	} 
	 video_err("mask\n");
	for(iR = 0; iR < iRMax; iR++)
	{
		for(iTh = 0; iTh < iThMax; iTh++)
	        {
	            	iCount = pArray[iR * iThMax + iTh];
			if(iCount > iMax)
			{
		                iMax = iCount;
		                iRMaxIndex = iR;
		                iThMaxIndex = iTh;
	          	  }
	        }
	}
	video_err("iRMaxIndex = %d\n",iRMaxIndex);
	video_err("iThMaxIndex = %d\n",iThMaxIndex);
	video_err(" iMax = %d\n", iMax);
	if(iMax >= iThreshold)
	{
	        *pR = iRMaxIndex;
	        *pTh = iThMaxIndex;
	}

	#if 0	//test
	for (i = 0; i < ht; i++)
	{
		for (j = 0; j < wd; j++)
		{
			 if(srcbuff[i * wd + j]  == 255)
			{
				if((i32)(j * cos( iThMaxIndex * fRate) + i * sin( iThMaxIndex * fRate)) != iRMaxIndex)
					srcbuff[i * wd + j]  = 0;
			}
		}
	}
	#endif
	    return;
} 

void picGetLine(u8 *picbuff, i32 wd, i32 ht,  i32 *direc, i32 * pos)
{
	i32 i, j, k, l, pointx, flag, pointnum;
	u8 buff[wd * ht];

	memset(buff, 0, wd * ht);
	for(j = 0; j< ht; j ++)
	{
		for(i = 2; i < wd - 2; i++)
		{
			if(picbuff[j * wd + i] == 255)
			{
				k = j;
				l = i;
				flag = 0;
				while((picbuff[k * wd + l - 2]) ||(picbuff[k * wd + l - 1]) ||(picbuff[k * wd + l]) ||(picbuff[k * wd + l + 1]) ||(picbuff[k * wd + l +2]))
				{
					flag ++;
					pointx = 0;
					pointnum = 0;
					if(picbuff[k * wd + l - 2])
					{
						pointx += l - 2; 
						pointnum ++;
					}
					if(picbuff[k * wd + l - 1])
					{
						pointx += l - 2; 
						pointnum ++;
					}
					if(picbuff[k * wd + l ])
					{
						pointx += l - 2; 
						pointnum ++;
					}
					if(picbuff[k * wd + l + 1])
					{
						pointx += l - 2; 
						pointnum ++;
					}
					if(picbuff[k * wd + l + 2])
					{
						pointx += l - 2; 
						pointnum ++;
					}
					l = pointx / pointnum;
					k ++;
				}
				if(flag > 30)
				{
					printf("start: x= %d	y = %d\n",i, j);
					printf("end: x= %d	y = %d\n",l, k);
					
				}
			}
		}
	}
}

