/* Basic system-level utilities
   August 2017
*/

#ifndef SYS_UTIL_H
#define SYS_UTIL_H

#define ASPRINTF_CHECK(...) \
{\
    if(-1 == asprintf(__VA_ARGS__))\
    {\
        fprintf(stderr, "Critical error: failed to allocate asprintf memory.\n");\
    }\
}\

int device_reboot(void);
void delay_minutes( int i );

#endif // SYS_UTIL
