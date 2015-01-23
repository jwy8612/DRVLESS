#ifndef DEBUG_H
#define DEBUG_H

#define DBG 1
#define ERR 1

#if(DBG)
#define run_dbg(fmt, args...)		printf("[RUN:%s] " fmt, __FUNCTION__, ## args)
#define function_in()				printf("%s entry!\n", __FUNCTION__)
#define function_out()				printf("%s exit!\n", __FUNCTION__)
#else
#define run_dbg(fmt, args...)	
#define function_in()		
#define function_out()	
#endif

#if(ERR)
#define run_err(fmt, args...)		printf("[RUN:%s] " fmt, __FUNCTION__, ## args)
#else
#define run_err(fmt, args...)	
#endif

#endif

