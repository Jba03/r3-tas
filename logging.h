//
//  logging.h
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef logging_h
#define logging_h

#include <stdio.h>

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

void llog(int mode, const char* fmt, ...);

#define info(...)      llog(LOG_INFO, __VA_ARGS__)
#define warning(...)   llog(LOG_WARNING, __VA_ARGS__)
#define error(...)     llog(LOG_ERROR, __VA_ARGS__)


#endif /* logging_h */
