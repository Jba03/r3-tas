//
//  array.c
//  Intra
//
//  Created by Jba03 on 2022-10-04.
//

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Array

struct Array
{
    void** data;
    int counter;
    int size;
};

ARRAY struct Array* array_create(void)
{
    struct Array *array = malloc(sizeof *array);
    array->data = malloc(sizeof(void*) * 4);
    array->size = 4;
    array->counter = 0;
    
    memset(array->data, 0, sizeof(void*) * 4);
    
    return array;
}

ARRAY void array_push(struct Array* array, void* element)
{
    if (!array) return;
    if (!element) return;
    
    if (array->counter >= array->size)
    {
        array->size *= 2;
        array->data = realloc(array->data, sizeof(void*) * array->size);
    }
    
    array->data[array->counter] = element;
    array->counter++;
}

ARRAY void* array_get(struct Array* array, int index)
{
    if (!array) return NULL;
    if (array->counter > array->size) return NULL;
    return array->data[index];
}

ARRAY int array_element_count(struct Array* array)
{
    if (!array) return -1;
    return array->counter;
}

ARRAY void array_free(struct Array **array)
{
    if (!array) return;
    free((*array)->data);
    free((*array));
}
