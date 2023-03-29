//
//  fnTransform.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-29.
//

#include "stTransform.h"

static tdstMatrix4D fnTransformGetMatrix(const tdstTransform *transform)
{
    return transform ? matrix4_host_byteorder(transform->matrix) : matrix4_identity;
}
