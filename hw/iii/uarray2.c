/**************************************************************
 *
 *                    uarray2.c
 *
 *      Assignment: Homework 2 -- iii
 *      Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *      Date: February 5th, 2023
 *     
 *      summary:
 *      Implementation of the uarray2 interface, using a 
 *      UArray_T to implement the 2d array as one long array.
 *      Implements the functions defined by uarray2.h
 *
 **************************************************************/

#include "uarray2.h"

#define T UArray2_T

struct T {
        int width, height, size; // is this ok?
        UArray_T array;
};

T UArray2_new(int width, int height, int size) // or should it return a UArray_T? 
{     
        assert(width >= 0);
        assert(height >= 0);
        assert(size > 0);

        T new_array; // same as doing UArray2_T? 
        NEW(new_array);

        new_array->width = width; 
        new_array->height = height;
        new_array->size = size;
        new_array->array = UArray_new(width * height, size);

        return new_array;
}

void UArray2_free(T *uarray2)
{
        assert(uarray2 != NULL && *uarray2 != NULL); // do we need to check both?

        UArray_free(&(*uarray2)->array);
        FREE(*uarray2);
}

int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

void *UArray2_at(T uarray2, int col, int row)
{
        assert(col >= 0 && col < uarray2->width);
        assert(row >= 0 && row < uarray2->height);
        
        int idx = col + (row * uarray2->width);
        return UArray_at(uarray2->array, idx);
}

void UArray2_map_row_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                        void *val, void *cl),
                                                        void *cl)
{
        assert(uarray2 != NULL);

        for (int j = 0; j < uarray2->height; j++) {
                for (int i = 0; i < uarray2->width; i++) {
                        int idx = i + (j * uarray2->width);
                        void *current_p = UArray_at(uarray2->array, idx);
                        apply(i, j, uarray2, current_p, cl);
                }
        }
}

void UArray2_map_col_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                 void *val, void *cl), 
                                                 void *cl)
{
        assert(uarray2 != NULL);

        for (int i = 0; i < uarray2->width; i++) {
                for (int j = 0; j < uarray2->height; j++) {
                        int idx = i + (j * uarray2->width);
                        void *current_p = UArray_at(uarray2->array, idx);
                        apply(i, j, uarray2, current_p, cl);
                }
        }
}




