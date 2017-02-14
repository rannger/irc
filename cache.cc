#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "cache.h"
#include "types.h"
#include "PrivateMsg.h"
#include "macro.h"
#include "llqueue.h"

namespace rirc {

	static llqueue *__queue = NULL;

	void initQueue(void) 
	{ 
		initQueue(&__queue); 
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
		queueAppend(__queue, message);
		return res;
	}

	rirc::BaseMessage* getMessage(void)
	{
		rirc::BaseMessage* retVal = NULL;
		retVal = static_cast<rirc::BaseMessage*>(queueGet(__queue));
		return retVal;
	}

	void queueAdd(rirc::BaseMessage *msg)
	{
		queueAppend(__queue, msg);
	}
}
