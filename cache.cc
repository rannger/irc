#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "cache.h"
#include "pkt_queue.h"
#include "types.h"
#include "PrivateMsg.h"

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
		str_t msg(buf,res);
		rirc::BaseMessage *message = new rirc::BaseMessage(msg);
		queue_add(__queue, message);
		return res;
	}

	rirc::BaseMessage* getMessage(void)
	{
		rirc::BaseMessage* retVal = NULL;
		retVal = static_cast<rirc::BaseMessage*>(queue_get(__queue));
		return retVal;
	}

	void queueAdd(rirc::PrivateMessage *msg)
	{
		queue_add(__queue, msg);
	}
}
