/*******************************************************
 * vector4.c: Four-component vector operations
 *******************************************************
 * Created by Jba03 on 2022-10-04
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#include "vector4.h"

#pragma mark - Vector4

VECTOR4 union Vector4 vector4_read(const address addr)
{
    union Vector4 v = { .offset = addr };
    v.x = memory.read_float(addr + 4 * 0);
    v.y = memory.read_float(addr + 4 * 1);
    v.z = memory.read_float(addr + 4 * 2);
    v.w = memory.read_float(addr + 4 * 3);
    
    return v;
}

VECTOR4 union Vector4 vector4_new(const float x, const float y, const float z, const float w)
{
    union Vector4 v;
    v.offset = 0x00;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    
    return v;
}

VECTOR4 union Vector4 vector4_add(const union Vector4 a, const union Vector4 b)
{
    union Vector4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    
    return result;
}

VECTOR4 union Vector4 vector4_sub(const union Vector4 a, const union Vector4 b)
{
    union Vector4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    
    return result;
}

VECTOR4 union Vector4 vector4_mul(const union Vector4 a, const union Vector4 b)
{
    union Vector4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    
    return result;
}

VECTOR4 union Vector4 vector4_div(const union Vector4 a, const union Vector4 b)
{
    union Vector4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    
    return result;
}

VECTOR4 union Vector4 vector4_mulf(const union Vector4 vector, const float value)
{
    union Vector4 result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    result.w = vector.w * value;
    
    return result;
}
