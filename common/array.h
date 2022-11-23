//
//  array.h
//  Intra
//
//  Created by Jba03 on 2022-10-04.
//

#ifndef array2_h
#define array2_h

#define ARRAY

struct Array;

ARRAY struct Array* array_create(void);

ARRAY void array_push(struct Array* array, void* element);

ARRAY void* array_get(struct Array* array, int index);

ARRAY int array_element_count(struct Array* array);

ARRAY void array_free(struct Array **array);

#endif /* array2_h */
