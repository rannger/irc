#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "cache.h"
#include "pkt_queue.h"
#include "types.h"
#include "PrivateMsg.h"
#include "macro.h"

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

		for(int i = 0;i<res;++i) {
			const uint8_t val = static_cast<uint8_t>(buf[i]);
			__IF_DO(val < 0x20||val > 0x7E, 
					buf[i] = 0x20;);
		}

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
