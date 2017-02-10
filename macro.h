#ifndef __MACRO_H_
#define __MACRO_H_ 
#include "cache.h"

#define __IF_DO(exp, ...) \
    if ((exp)) { \
        __VA_ARGS__;\
    }

#define __CHECK(exp) __IF_DO(!(exp),return;);

#if 0
	#define KNRM  "\x1B[0m"
	#define KRED  "\x1B[31m"
	#define KGRN  "\x1B[32m"
	#define KYEL  "\x1B[33m"
	#define KBLU  "\x1B[34m"
	#define KMAG  "\x1B[35m"
	#define KCYN  "\x1B[36m"
	#define KWHT  "\x1B[37m"
	#define RESET "\x1B[0m"
#else 
	#define KNRM  ""
	#define KRED  ""
	#define KGRN  ""
	#define KYEL  ""
	#define KBLU  ""
	#define KMAG  ""
	#define KCYN  ""
	#define KWHT  ""
	#define RESET ""
#endif

#define __LOG(format,...) rirc::printf(format, __VA_ARGS__);

#define __EXIT \
	do { \
		printf("%s,%d,%d",__FILE__,__LINE__,errno); \
		exit(-1); \
	} while(0); 

#endif
