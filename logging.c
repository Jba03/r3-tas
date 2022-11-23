//
//  logging.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "logging.h"
#include "common.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"

void llog(int mode, const char* fmt, ...)
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
    for (int i = 0; i < log_indent * 4; i++) printf(" ");
    vprintf(buf, args);
    va_end(args);
}
