/*******************************************************
 * array.h: Dynamic array
 *******************************************************
 * Created by Jba03 on 2022-10-04
 * Copyright (c) 2022 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef array2_h
#define array2_h

#include <stdlib.h>
#include <string.h>

#define ARRAY

struct array
{
    void** data;
    int counter;
    int size;
};

ARRAY static inline struct array* array_create(void)
{
    struct array *array = (struct array*)malloc(sizeof *array);
    array->data = (void**)malloc(sizeof(void*) * 4);
    array->size = 4;
    array->counter = 0;
    
    memset(array->data, 0, sizeof(void*) * 4);
    
    return array;
}

ARRAY static inline void array_push(struct array* array, void* element)
{
    if (!array || !element) return;
    if (array->counter >= array->size)
    {
        array->size *= 2;
        array->data = (void**)realloc(array->data, sizeof(void*) * array->size);
    }
    
    array->data[array->counter] = element;
    array->counter++;
}

ARRAY static inline void* array_get(struct array* array, int index)
{
    if (!array) return NULL;
    if (index < 0) return NULL;
    if (array->counter > array->size) return NULL;
    return array->data[index];
}

ARRAY static inline unsigned array_element_count(struct array* array)
{
    if (!array) return 0;
    return array->counter;
}

ARRAY static inline void array_free(struct array **array)
{
    if (!array) return;
    free((*array)->data);
    free((*array));
}


#endif /* array2_h */
