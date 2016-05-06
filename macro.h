#ifndef __MACRO_H_
#define __MACRO_H_ 

#define __IF_DO(exp, ...) \
    if ((exp)) { \
        __VA_ARGS__;\
    }

#define __CHECK(exp) __IF_DO(!(exp),return;);

#define __LOG(format,...) printf(format, __VA_ARGS__);
#define __LOGS(...) __LOG("%s\n",__VA_ARGS__);

#endif