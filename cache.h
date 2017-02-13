#ifndef __CACHE_H_
#define __CACHE_H_

#include "types.h"

namespace rirc {
	class BaseMessage;
	class PrivateMessage;

	void initQueue(void);
	BaseMessage* getMessage(void);

	int printf(char* format,...);
	void queueAdd(rirc::BaseMessage* msg);
}

#endif
