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

int getCmd(void *Inst, PCOMMAND_INFO cmdInfo)
{
	int ret = 0;
	char index = 0;
	int lenpos;
	PMOTOR_CMD_INFO motorCmd 	= &(cmdInfo->motorCmd);
	PSERVO_CMD_INFO servoCmd 	= &(cmdInfo->servoCmd);
	PLIGHT_CMD_INFO lgtCmd 		= &(cmdInfo->lgtCmd);
	PHORN_CMD_INFO hornCmd 	= &(cmdInfo->hornCmd);
	char *cmdbuff 				= cmdInfo->commadData;
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
				cmdbuff[index] = (char)lgtCmd->light;
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
				cmdbuff[index] = (char)hornCmd->horn;
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
int picFmtTrans(int wd, int ht, char *srcbuff, char * dstbuff)
{
	int ret = 0;
	int i, j;
	
	function_in();
	for(i = 0; i < ht; i ++)
	{
		for(j = 0; j < wd; j ++)
		{
			dstbuff[i * wd + j] = srcbuff[i * wd + j * 2];
		}
	}
	function_out();
	return ret;
}

int picGrad(int wd, int ht, char *picbuff)
{
	int ret = 0;
	int i, j;
	signed char gx, gy;
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

int picFilter(int wd, int ht, char *picbuff, char flag)
{
	int ret = 0;
	int i, j;
	char w,s;
	char buff[picSize];
	function_in();
	w = 1;
	s = 2;
	if(flag == directX)
	{
		for(j = 0; j < ht; j ++)
		{
			buff[j * wd] = (picbuff[j * wd] * s + picbuff[j * wd + 1] * w) / (w + s);
		}
		for(i = 1; i < wd - 1; i ++)
		{
			for(j = 0; j < ht; j ++)
			{
				buff[j * wd + i] = (picbuff[j * wd + i] * s + picbuff[j * wd + i - 1] * w + picbuff[j * wd + i + 1] * w) / (w + w + s);
			}

		}
		for(j = 0; j < ht; j ++)
		{
			buff[(j + 1) * wd - 1] = (picbuff[(j + 1) * wd - 1] * s + picbuff[(j + 1) * wd - 2] * w) / (w + s);
		}
		memcpy(picbuff,buff,picSize);
	}
	if(flag == directY)
	{

	}
	

}
void Hough(char *srcbuff,int wd,int ht, int *pR, int *pTh, int iThreshold)
{
	int iRMax = (int)sqrt(wd * wd + ht * ht) + 1;
	int iThMax = 361;
	int iTh, iR;
	int iMax = -1;
    	int iThMaxIndex = -1;
    	int iRMaxIndex = -1;
    	int pArray[iRMax * iThMax];
    	float fRate = (float)(PI/180);
	int i,j;	
	 int iCount ;

	memset(pArray, 0, sizeof(int) * iRMax * iThMax);

	for (i = 0; i < ht; i++)
	{
		for (j = 0; j < wd; j++)
		{
			 if(srcbuff[i * wd + j] == 255)
			{
				for(iTh = 0; iTh < iThMax; iTh ++)
	                	{
					iR = (int)(j * cos(iTh * fRate) + i * sin(iTh * fRate));
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
				if((int)(j * cos( iThMaxIndex * fRate) + i * sin( iThMaxIndex * fRate)) != iRMaxIndex)
					srcbuff[i * wd + j]  = 0;
			}
		}
	}
	#endif
	    return;
} 
