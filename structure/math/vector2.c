/*******************************************************
 * vector2.c: Two-component vector operations
 *******************************************************
 * Created by Jba03 on 2022-10-04
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#include "vector2.h"

#pragma mark - Vector2

VECTOR2 struct Vector2 vector2_new(const float x, const float y)
{
    struct Vector2 v;
    v.x = x;
    v.y = y;
    
    return v;
}

VECTOR2 struct Vector2 vector2_add(const struct Vector2 a, const struct Vector2 b)
{
    struct Vector2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

VECTOR2 struct Vector2 vector2_sub(const struct Vector2 a, const struct Vector2 b)
{
    struct Vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

VECTOR2 struct Vector2 vector2_mul(const struct Vector2 a, const struct Vector2 b)
{
    struct Vector2 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    
    return result;
}

VECTOR2 struct Vector2 vector2_mulf(const struct Vector2 a, const float value)
{
    struct Vector2 result;
    result.x = a.x * value;
    result.y = a.y * value;
    
    return result;
}

VECTOR2 struct Vector2 vector2_div(const struct Vector2 a, const struct Vector2 b)
{
    struct Vector2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    
    return result;
}

VECTOR2 struct Vector2 vector2_negate(const struct Vector2 vector)
{
    struct Vector2 result;
    result.x = -vector.x;
    result.y = -vector.y;
    
    return result;
}
