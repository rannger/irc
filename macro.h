#ifndef __MACRO_H_
#define __MACRO_H_ 

#define __IF_DO(exp, ...) \
    if ((exp)) { \
        __VA_ARGS__;\
    }

#define __CHECK(exp) __IF_DO(!(exp),return;);

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\x1B[0m"

#define __LOG(format,...) \
	printf(format, __VA_ARGS__); \
	refresh();
#define __LOGS(msg) \
	printf(msg); \
	refresh();
#endif
