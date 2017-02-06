#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "cache.h"
#include "pkt_queue.h"

namespace rirc {
	static struct queue_root *__queue = NULL;

	void initQueue(void) 
	{ 
		init_queue(&__queue); 
	}
	
	int printf(char* format,...) 
	{
		char buf[1024];
		bzero(buf,sizeof(char)*1024);
		va_list argptr;
		va_start(argptr, format);
		int res = vsnprintf(buf,1023,format, argptr);
		va_end(argptr);
		str_t *msg = new str_t(buf,res);
		queue_add(__queue, msg);
		return res;
	}

	str_t* getMessage(void)
	{
		str_t* retVal = NULL;
		retVal = static_cast<str_t*>(queue_get(__queue));
		return retVal;
	}
}
