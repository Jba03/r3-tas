//
//  common.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-06.
//

#ifndef common_h
#define common_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define radians(deg) ((deg) * (M_PI / 180.0f))
#define degrees(rad) ((rad) * (180.0f / M_PI))

#define min(a, b) (((a) < (b)) ? (a) : (b)) /* min: Choose smaller of two values */
#define max(a, b) (((a) > (b)) ? (a) : (b)) /* max: Choose bigger of two values */

#endif /* common_h */
