#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"
#include <math.h>

void line_display(u8 *srcbuff,i32 wd,i32 ht,i32 iTh,i32 iR)
{
	i32 i;
    float fRate = (float)(PI/180);
	float k;
	i32 b;

	
	k=tan((90+iTh)*fRate);
	b=iR/sin(iTh*fRate);
	if(iTh>90)
	{
		b=0-b;
	}
	
	for (i=0; i<wd; i++)
	{
		if((k*i+b>0)&&(k*i+b<ht))
			srcbuff[(i32)(k*i+b) * wd + i]=255;
	}
	for (i=0; i<ht; i++)
	{
		if(((i-b)/k>0)&&((i-b)/k<wd))
			srcbuff[i * wd + (i32)((i-b)/k)]=255;
	}

}
int pic_Hough(u8 *srcbuff,i32 wd,i32 ht, i32 *pR, i32 *pTh)
{
	i32 iRMax = (i32)sqrt(wd * wd + ht * ht);
	i32 iThMax = 180;
	i32 iTh, iR;
	i32 iMax[2]={0,0};
    i32 iThMaxIndex[2];
    i32 iRMaxIndex[2];
    i32 * pArray=NULL;//[iRMax * iThMax];
    float fRate = (float)(PI/180);
	i32 i,j;	
	i32 iCount ;
	static u8 startFlag = 0;
	static float cosTable[180];
	static float sinTable[180];
	float k;
	i32 b;
	function_in();
#if 0
	memset(srcbuff,0,wd*ht);
	iRMaxIndex[0]=80;
	iThMaxIndex[0]=-60;
	line_display(srcbuff,wd,ht,iThMaxIndex[0],iRMaxIndex[0]);
#endif
	if(startFlag==0)
	{
		for(i=0;i<iThMax;i++)
		{
			cosTable[i]=cos((i-90)*fRate);
			sinTable[i]=sin((i-90)*fRate);
		}
		startFlag=1;
	}
	pArray = (i32*)malloc(iRMax*iThMax*sizeof(i32));
	if(pArray==NULL)
	{
		video_err("malloc failed!!!\n");
		goto err;
	}
	memset(pArray, 0, sizeof(i32) * iRMax * iThMax);

	for (j=0; j<ht; j++)
	{
		for (i=0; i<wd; i++)
		{
			if(srcbuff[j * wd + i] == 255)
			{
				for(iTh = -90; iTh < 90; iTh ++)
	           	{
					iR = (i32)(i * cosTable[iTh+90] + j * sinTable[iTh+90]);
					if(iR > 0)
					{
						pArray[iR * iThMax + iTh+90]++;
					}
	            }
	       	}

		} 

	} 
	for(iR = 0; iR < iRMax; iR++)
	{
		for(iTh = 0; iTh < iThMax; iTh++)
		{
			iCount = pArray[iR * iThMax + iTh];
			if((iCount > iMax[0])||(iCount > iMax[1]))
			{
				if(iCount > iMax[0])
				{
					iMax[0] = iCount;
					if((iThMaxIndex[0]-iTh+90>5)||(iThMaxIndex[0]-iTh+90<-5))
					{
						iRMaxIndex[1]=iRMaxIndex[0];
						iThMaxIndex[1] = iThMaxIndex[0];
						iMax[1]=iMax[0];
					}
					iRMaxIndex[0] = iR;
					iThMaxIndex[0] = iTh-90;
#if 0
					printf("max\n");
					printf("%d\n",iCount);
					printf("iThMaxIndex[0]=%d\n",iThMaxIndex[0]);
					printf("iRMaxIndex[0]=%d\n",iRMaxIndex[0]);
					printf("iThMaxIndex[1]=%d\n",iThMaxIndex[1]);
					printf("iRMaxIndex[1]=%d\n",iRMaxIndex[1]);
#endif
				}
				else
				{
					if((iTh-90-iThMaxIndex[0]>5)||(iTh-90-iThMaxIndex[0]<-5))
					{
						iMax[1] = iCount;
						iRMaxIndex[1] = iR;
						iThMaxIndex[1] = iTh-90;
#if 0
						printf("second\n");
						printf("%d\n",iCount);
						printf("iThMaxIndex[0]=%d\n",iThMaxIndex[0]);
						printf("iRMaxIndex[0]=%d\n",iRMaxIndex[0]);
						printf("iThMaxIndex[1]=%d\n",iThMaxIndex[1]);
						printf("iRMaxIndex[1]=%d\n",iRMaxIndex[1]);
#endif
					}
				}
			}
		}
	}
//  debug

	//line_display(srcbuff,wd,ht,iThMaxIndex[0],iRMaxIndex[0]);
	//line_display(srcbuff,wd,ht,iThMaxIndex[1],iRMaxIndex[1]);

	*pTh = iThMaxIndex[0]+iThMaxIndex[1];
	
	b=iRMaxIndex[0]/sin(iThMaxIndex[0]*fRate);
	k=tan((90+iThMaxIndex[0])*fRate);
	*pR = (240-b)/k;
	*pTh = iThMaxIndex[0]+iThMaxIndex[1];
	
	b=iRMaxIndex[1]/sin(iThMaxIndex[1]*fRate);
	k=tan((90+iThMaxIndex[1])*fRate);
	*pR += (240-b)/k;
	*pR/=2;
	*pR-=160;
	//printf("pTh=%d pR=%d\n",*pTh,*pR);
	function_out();
	err:
	if(pArray!=NULL)
		free(pArray);
	return -1;
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

