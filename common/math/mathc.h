/*******************************************************
 * math.h: Common mathematics functions
 *******************************************************
 * Created by Jba03 on 2022-12-20
 * Copyright (c) 2022 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef math_h
#define math_h

#include <math.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

static inline const double radians(const double degrees)
{
    return degrees * (M_PI / 180.0);
}

static inline const double degrees(const double radians)
{
    return radians * (180.0 / M_PI);
}

#endif /* math_h */
