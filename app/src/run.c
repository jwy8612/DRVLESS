/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-08

    Descriptions : 无人驾驶车app
  
    Version             Description             Date                Author
    0.1                 Created                 2015-01-08       姜万勇

---------------------------------------------------------------------------------------------*/
#include"debug.h"
#include<stdio.h>
#include "video_api.h"

int main(void)
{
	int ret = 0;
	void * videoInst = NULL;
	
	function_in();
	videoInst = Video_Init();
	ret = Video_GetConfig(videoInst);
		
	function_out();
	return ret;
}	