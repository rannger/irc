#ifndef __CACHE_H_
#define __CACHE_H_

#include "types.h"

namespace rirc {
	void initQueue(void);
	str_t* getMessage(void);
	int printf(char* format,...);
}

#endif
