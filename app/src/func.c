/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-02-03

    Descriptions : 功能函数
  
    Version             Description             Date                Author
    0.1                 Created                 2015-02-03       姜万勇

---------------------------------------------------------------------------------------------*/
#include "debug.h"
#include <stdio.h>
#include "video_api.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "typedef.h"
#include <math.h>

#define FILTER 3

u8 getMedianNum(u8 * bArray, int iFilterLen)  
{  
    int i,j;// 循环变量  
    u8 bTemp;  
      
    // 用冒泡法对数组进行排序  
    for (j = 0; j < iFilterLen - 1; j ++)  
    {  
        for (i = 0; i < iFilterLen - j - 1; i ++)  
        {  
            if (bArray[i] > bArray[i + 1])  
            {  
                // 互换  
                bTemp = bArray[i];  
                bArray[i] = bArray[i + 1];  
                bArray[i + 1] = bTemp;  
            }  
        }  
    }  
      
    // 计算中值  
    if ((iFilterLen & 1) > 0)  
    {  
        // 数组有奇数个元素，返回中间一个元素  
        bTemp = bArray[(iFilterLen + 1) / 2];  
    }  
    else  
    {  
        // 数组有偶数个元素，返回中间两个元素平均值  
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;  
    }  
  
    return bTemp;  
}  

u8 getAverageNum(u8 * bArray, int iFilterLen)
{
	u32 sum = 0;
	int i;
	u8 ret;
	for(i=0; i< iFilterLen; i++)
	{
		sum += bArray[i];
	}
	ret = sum / iFilterLen;
	return ret;
}
i32 getCmd(void *Inst, PCOMMAND_INFO cmdInfo)
{
	i32 ret = 0;
	u8 index = 0;
	i32 lenpos;
	PMOTOR_CMD_INFO motorCmd 	= &(cmdInfo->motorCmd);
	PSERVO_CMD_INFO servoCmd 	= &(cmdInfo->servoCmd);
	PLIGHT_CMD_INFO lgtCmd 		= &(cmdInfo->lgtCmd);
	PHORN_CMD_INFO hornCmd 	= &(cmdInfo->hornCmd);
	u8 *cmdbuff 				= cmdInfo->commadData;
	void * comInst = Inst;
	function_in();
	cmdbuff[index] = 0xff;
	index ++;
	lenpos = index;
	index ++;
	if(motorCmd->bMoterCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(motorCmd->cmdtype)
		{
			case 1 :
				cmdbuff[index] = motorCmd->cmdtype;
				index ++;
				cmdbuff[index] = motorCmd->motorV;
				index ++;
				break;
			default :
				cmdbuff[index] = motorCmd->cmdtype;
				index ++;
				run_err("unsupported motor cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	if(servoCmd->bservoCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(servoCmd->cmdtype)
		{
			case 1 :
				cmdbuff[index] = servoCmd->cmdtype;
				index ++;
				memcpy(cmdbuff + index, &(servoCmd->servoA), 2);
				index += 2;
				memcpy(cmdbuff + index, &(servoCmd->servoV), 2);
				index += 2;
				break;
			default :
				cmdbuff[index] = servoCmd->cmdtype;
				index ++;
				run_err("unsupported servo cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	if(lgtCmd->blightCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(lgtCmd->light)
		{
			case ximie :
				cmdbuff[index] = 0;
				index ++;
				break;
			case changliang :
				cmdbuff[index] = 1;
				index ++;
				break;
			default :
				cmdbuff[index] = (u8)lgtCmd->light;
				index ++;
				run_err("unsupported light cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}
	
	if(hornCmd->bhornCmd)
	{
		cmdbuff[index] = 0x01;
		index ++;
		switch(hornCmd->horn)
		{
			case quite:
				cmdbuff[index] = 0;
				index ++;
				break;
			case chang :
				cmdbuff[index] = 1;
				index ++;
				break;
			case duan :
				cmdbuff[index] = 2;
				index ++;
				break;
			case dduan :
				cmdbuff[index] = 3;
				index ++;
				break;
			default :
				cmdbuff[index] = (u8)hornCmd->horn;
				index ++;
				run_err("unsupported light cmd type!!!\n");
		}
	}
	else
	{
		cmdbuff[index] = 0x00;
		index ++;
	}	
	cmdbuff[index] = 0x80;
	cmdbuff[lenpos] = index - lenpos -1;
	cmdInfo->dataLength = index + 1;
	ret = Com_SendData(comInst, cmdbuff, cmdInfo->dataLength);
	if(ret != index)
	{
		run_err("com send cmd err!!!\n");
	}
	function_out();
	return ret;
}

/////////YUYV to 4:0:0////////////
i32 picFmtTrans(i32 wd, i32 ht, u8 *srcbuff, u8 * dstbuff, u8 type)
{
	i32 ret = 0;
	i32 i, j;
	
	function_in();
	switch(type)
	{
		case YUV_Y :
		{
			for(i = 0; i < ht / 2; i ++)
			{
				for(j = 0; j < wd / 2; j ++)
				{
					dstbuff[i * wd / 2 + j] = srcbuff[i * 4 * wd + j * 4];
				}
			}
			break;
		}
		case YUV_U :
		{
			for(i = 0; i < ht / 2; i ++)
			{
				for(j = 0; j < wd / 2; j ++)
				{
					dstbuff[i * wd / 2 + j] = srcbuff[i * 4 * wd + j * 4 + 1];
				}
			}
			break;
		}
		case YUV_V :
		{
			for(i = 0; i < ht / 2; i ++)
			{
				for(j = 0; j < wd / 2; j ++)
				{
					dstbuff[i * wd / 2 + j] = srcbuff[i * 4 * wd + j * 4 + 3];
				}
			}
			break;
		}		
		default:
			break;
	}
	function_out();
	return ret;
}

i32 picGrad(i32 wd, i32 ht, u8 *picbuff, u8 gradThrea)
{
	i32 ret = 0;
	i32 i, j;
	i8 gx, gy;
	function_in();
	for(i = 0; i < ht - 1; i ++)
	{
		for(j = 0; j < wd - 1; j ++)
		{
			gx = picbuff[i * wd + j] - picbuff[(i +1)* wd + j +1];
			gy = picbuff[i * wd + j +1] - picbuff[(i +1)* wd + j];
			picbuff[i * wd + j] = abs(gx) + abs(gy);
			picbuff[i * wd + j] = picbuff[i * wd + j] > gradThrea ? 255 : 0;
		}
		picbuff[i * wd + wd] = picbuff[i * wd + wd - 1];
		picbuff[i * wd + j] = picbuff[i * wd + j] > gradThrea ? 255 : 0;
	}
	for(j = 0; j < wd; j ++)
	{
		picbuff[(ht - 1) * wd + j] = picbuff[(ht - 2) * wd + j];
		picbuff[i * wd + j] = picbuff[i * wd + j] > gradThrea ? 255 : 0;
	}

	function_out();
	return ret;
}
i32 picDif(i32 wd, i32 ht, u8 *picbuff1, u8 *picbuff2, u8 *picbuff3)
{
	i32 ret = 0;
	i32 i, j;

	function_in();
	for(i = 0; i < ht; i ++)
	{
		for(j = 0; j < wd; j ++)
		{
			picbuff3[i * wd + j] = picbuff1[i * wd + j] * 3 / 2 - picbuff2[i * wd + j];
		}
	}

	function_out();
	return ret;
}
i32 picAdd(i32 wd, i32 ht, u8 *picbuff1, u8 *picbuff2, u8 *picbuff3)
{
	i32 ret = 0;
	i32 i, j;

	function_in();
	for(i = 0; i < ht; i ++)
	{
		for(j = 0; j < wd; j ++)
		{
			picbuff3[i * wd + j] = picbuff1[i * wd + j] *  + picbuff2[i * wd + j];
		}
	}

	function_out();
	return ret;
}
i32 picFilterAverage(i32 wd, i32 ht, u8 *picbuff)
{
	i32 ret = 0;
	i32 i, j,k,l;
	u8 buff[wd*ht];
	u8 wind[FILTER * FILTER];
	function_in();
	for(j= FILTER/2; j<ht - FILTER/2; j++)
	{	
		for(i=FILTER/2; i<wd - FILTER/2; i ++)
		{
			for(k=0; k<FILTER;k++)
			{
				for(l=0;l<FILTER;l ++)
				{
					wind[k * FILTER+l] = picbuff[(k + j - 1) * wd + l + i - 1];
				}
			}
			buff[j*wd+i] = getAverageNum(wind,FILTER*FILTER);
		}
	}
	memcpy(picbuff,buff,wd*ht);
	#if 1
	for(j = 0;j < FILTER/2;j++)
	{
		for(i=0;i<wd;i++)
		{
			picbuff[j*wd+i] = buff[FILTER/2*wd+i];
		}
	}
	for(j = ht-FILTER/2;j < ht;j++)
	{
		for(i=0;i<wd;i++)
		{
			picbuff[j*wd+i] = buff[(ht - FILTER/2-1)*wd+i];
		}
	}	
	for(i = 0;i < FILTER/2;i++)
	{
		for(j=0;j<ht;j++)
		{
			picbuff[j*wd+i] = buff[j*wd + FILTER/2];
		}
	}
	for(i = wd-FILTER/2;i < wd;i++)
	{
		for(j=0;j<ht;j++)
		{
			picbuff[j*wd+i] = buff[j*wd+wd - FILTER/2-1];
		}
	}	
	#endif
	function_out();
	return ret;
}
i32 picFilterMiddle(i32 wd, i32 ht, u8 *picbuff)
{
	i32 ret = 0;
	i32 i, j,k,l;
	u8 buff[wd*ht];
	u8 wind[FILTER * FILTER];
	function_in();

	for(j= FILTER/2; j<ht - FILTER/2; j++)
	{	
		for(i=FILTER/2; i<wd - FILTER/2; i ++)
		{
			for(k=0; k<FILTER;k++)
			{
				for(l=0;l<FILTER;l ++)
				{
					wind[k * FILTER+l] = picbuff[(k + j - 1) * wd + l + i - 1];
				}
			}
			buff[j*wd+i] = getMedianNum(wind,FILTER*FILTER);
		}
	}
	memcpy(picbuff,buff,wd*ht);
	#if 1
	for(j = 0;j < FILTER/2;j++)
	{
		for(i=0;i<wd;i++)
		{
			picbuff[j*wd+i] = buff[FILTER/2*wd+i];
		}
	}
	for(j = ht-FILTER/2;j < ht;j++)
	{
		for(i=0;i<wd;i++)
		{
			picbuff[j*wd+i] = buff[(ht - FILTER/2-1)*wd+i];
		}
	}	
	for(i = 0;i < FILTER/2;i++)
	{
		for(j=0;j<ht;j++)
		{
			picbuff[j*wd+i] = buff[j*wd + FILTER/2];
		}
	}
	for(i = wd-FILTER/2;i < wd;i++)
	{
		for(j=0;j<ht;j++)
		{
			picbuff[j*wd+i] = buff[j*wd+wd - FILTER/2-1];
		}
	}	
	#endif
	function_out();

	return ret;

}
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
				//	run_err("IR = %d\n",iR);
					if(iR > 0)
					{
	                       		 pArray[iR * iThMax + iTh]++;
						//		 	

					}
	                	}
	            	}

	        } 
			 run_err("mask\n");

	} 
	 run_err("mask\n");
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
	run_err("iRMaxIndex = %d\n",iRMaxIndex);
	run_err("iThMaxIndex = %d\n",iThMaxIndex);
	run_err(" iMax = %d\n", iMax);
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
i32 IsDimodal(u32 *HistoGram, u8 *top1, u8 *top2)
{
	int Count = 0; 
	i32 i;
	u8 tmp1 = 0, tmp2 = 0;
	for(i=2; i<254; i++)
	{
		if(HistoGram[i-2] < HistoGram[i-1] && HistoGram[i-1] <= HistoGram[i] &&HistoGram[i+1] <= HistoGram[i] && HistoGram[i+2] < HistoGram[i+1])    
		{          
			tmp1 = tmp2;
			tmp2 = i;
			Count++;  
		}     
	}  
	*top1 = tmp1;
	*top2 = tmp2;
	if(Count==2) 
	{
		return 1;  
	}
	else        
		return 0;  
}

i32 picGetThreshold(u8 *picbuff, i32 wd, i32 ht, i32 startLine, i32 startRow)
{
	u32 HistoGram[256];
	u32 HistoGram1[256];
	i32 i,j;
	i32 ret = 0;
	u8 top1,top2;
	function_in();
	memset(HistoGram, 0, 256*4);
	for(j=startLine; j<ht; j++)
	{
		for(i=startRow; i<wd-startRow; i++)
		{
			HistoGram[picbuff[j*wd+i]] ++;
		}
	}
#if 0	
	for(i=0;i<256;i++)
		printf("%u\n",HistoGram[i]);
#endif
	j=0;
	#if 1
	while (!IsDimodal(HistoGram, &top1, &top2))                                        
    {
		HistoGram1[0] = (HistoGram[0] + HistoGram[0] + HistoGram[1]) / 3;                 
		for (i = 1; i < 255; i++)
			HistoGram1[i] = (HistoGram[i - 1] + HistoGram[i] + HistoGram[i + 1]) / 3;     
		HistoGram1[255] = (HistoGram[254] + HistoGram[255] + HistoGram[255]) / 3;        
		memcpy(HistoGram, HistoGram1, 256 *4);
		j++;
		
		if(j>= 5)
		{
			run_err("smooth HistoGram too more!!!\n");
			break;
		}
    }
	#endif
	run_dbg("top1:%d	top2:%d\n",top1,top2);
#if 0
	for(i=0;i<256;i++)
		printf("%u\n",HistoGram[i]);
#endif
	for(i=top1+1; i<top2-1; i++)
	{
		if(HistoGram[i-1] >HistoGram[i] &&HistoGram[i+1] > HistoGram[i])
			ret = i;
	}
	if(top2 - ret > 2)
		ret = top2 - 2;
	run_err("Threshold:%d\n",ret);
	function_out();
	return ret;
}
void picBinary(u8 *picbuff, i32 wd, i32 ht, u8 Threshold)
{
	i32 i,j;
	function_in();
	for(j=0; j<ht; j++)
	{
		for(i=0; i<wd; i++)
		{
			picbuff[j*wd + i] = picbuff[j*wd + i] > Threshold ? 255 : 0;
		}
	}
	function_out();
}
