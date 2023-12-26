//
//  logging.h
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef logging_h
#define logging_h

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define BOLD "\033[1m"
#define FADED "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

#define COLOR_DEFAULT   "\x1B[0m"
#define COLOR_RED       "\x1B[31m"
#define COLOR_GREEN     "\x1B[32m"
#define COLOR_YELLOW    "\x1B[33m"
#define COLOR_BLUE      "\x1B[34m"
#define COLOR_PINK      "\x1B[35m"
#define COLOR_CYAN      "\x1B[36m"
#define COLOR_WHITE     "\x1B[37m"
#define COLOR_GREY      "\x1B[38m"

#define LOG_INFO    0
#define LOG_WARNING 1
#define LOG_ERROR   2

static inline void strlog(int mode, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    char buf[4096];
    memset(buf, 0, 4096);
    
    switch (mode)
    {
        case LOG_INFO: strcat(buf, COLOR_BLUE); break;
        case LOG_WARNING: strcat(buf, COLOR_YELLOW); break;
        case LOG_ERROR: strcat(buf, COLOR_RED); break;
        default: break;
    }
    
    strcat(buf, fmt);
    strcat(buf, COLOR_DEFAULT);
    //for (int i = 0; i < log_indent * 4; i++) printf(" ");
    vprintf(buf, args);
    va_end(args);
}


#define info(...)       strlog(LOG_INFO, __VA_ARGS__)
#define warning(...)    strlog(LOG_WARNING, __VA_ARGS__)
#define error(...)      strlog(LOG_ERROR, __VA_ARGS__)


#endif /* logging_h */
