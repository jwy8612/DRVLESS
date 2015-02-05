/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-02-03

    Descriptions :串口读取线程
  
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
#include "func.h"
#include <stdlib.h>
#include <pthread.h>

void *comGetThread(void *arg);

char comreadtest[10];
int comGetInit(void *Inst)
{
	int ret = 0;
	pthread_t tid;
	void *comInst = Inst;
	
	function_in();
	ret = pthread_create(&tid,NULL,comGetThread,comInst);
	if(ret != 0)
	{
		run_err("pthread_create failed !!!ret = %d\n",ret);
	}
	function_out();
	return ret;
}

void *comGetThread(void *arg)
{
	void *comInst = arg;
	int sec = 0;
	int fd;
	fd_set fds;
	struct timeval time;
	int ret = 0;

	
	function_in();
	//fd = Com_GetFd(comInst);
	while(1)
	{
		run_err("thread test time = %d\n",sec);
		sec ++;
		sleep(1);
		#if 0
		FD_ZERO(&fds);  
		FD_SET(fd, &fds); 
		time.tv_sec = 10;
		time.tv_usec = 0;

		ret = select(fd + 1, &fds, NULL, NULL, &time);
		if(ret < 0)
		{
			run_err("select failed!!!\n");
		}
		if(ret == 0)
		{
			run_err("select time out!!!\n");
		}
		if(ret > 0)
		{
			run_err("start to read com!!!\n");
			ret = Com_RecieveData(comInst, comreadtest, 10);
			Com_SendData(comInst, comreadtest, 10);
		}
		#endif
	}
	
	function_out();
	return NULL;
}
