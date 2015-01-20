#ifndef DEBUG_H
#define DEBUG_H

#define DBG 1
#define ERR 1

#if(DBG)
#define video_dbg(fmt, args...)		printf("[VIDEO:%s] " fmt, __FUNCTION__, ## args)
#define function_in()				printf("%s entry!\n", __FUNCTION__)
#define function_out()				printf("%s exit!\n", __FUNCTION__)
#else
#define video_dbg(fmt, args...)	
#define function_in()		
#define function_out()	
#endif

#if(ERR)
#define video_err(fmt, args...)		printf("[VIVSMART:%s] " fmt, __FUNCTION__, ## args)
#else
#define video_err(fmt, args...)	
#endif

#endif

