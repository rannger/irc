#ifndef __MACRO_H_
#define __MACRO_H_ 
#include "cache.h"

#define __IF_DO(exp, ...) \
    if ((exp)) { \
        __VA_ARGS__;\
    }

#define __CHECK(exp) __IF_DO(!(exp),return;);

#define __LOG(format,...) rirc::printf(format, __VA_ARGS__);

#define __EXIT \
	do { \
		printf("%s,%d,%d",__FILE__,__LINE__,errno); \
		exit(-1); \
	} while(0); 

#define __ASSERT_MAIN_THREAD \
	do { \
		if(-1==pthread_main_np()) assert(0); \
		if (pthread_main_np()==0) { \
			printf("%s,%d,here isn't main thread",__FILE__,__LINE__); \
			assert(0); \
		} \
	} while(0);
#endif
