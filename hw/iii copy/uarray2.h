/**************************************************************
 *
 *                    uarray2.h
 *
 *      Assignment: Homework 2 -- iii
 *      Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *      Date: February 5th, 2023
 *     
 *      summary:
 *      Interface for UArray2_T, a 2 dimensional unboxed array.
 *      Defines the functions for uarray2.c
 * 
 *
 **************************************************************/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uarray.h"
#include <mem.h>

#define T UArray2_T
typedef struct T *T;


/* UArray2_new
 *
 *      Purpose: Initialize and allocate a 2-dimensional unboxed array
 *               with the given width, height, and size of each element.
 *               Operates like an array with zero-based indexing, and 
 *               is implemented as a 1D array in row major order. 
 *   Parameters: width (width of the 2d array), height (height of the 2d
 *               array), and size (size of each element within the array)
 *
 *      Returns: A pointer to the UArray2_T struct, currently unpopulated.
 *
 * Expectations: Included UArray header file should handle most exceptions,
 *               but the size should be > 0, and the width and height
 *               should each be at least 0. 
 *
*/
extern T UArray2_new(int width, int height, int size);

/* UArray2_free
 *
 *      Purpose: Deallocate the memory associated with the 
 *               2d Uarray and the pointer itself. 
 *   Parameters: *uarray2 (pointer to the uarray2 to be freed).
 *
 *      Returns: none
 *
 * Expectations: the pointer to the uarray2 (*uarray2) and the 
 *               2d array itself (uarray2) should both be non-NULL.
 *
*/
extern void UArray2_free(T *uarray2);

/* UArray2_width
 *
 *      Purpose: Return the width of the 2d Uarray, which is essentially
 *               the number of columns. 
 *   Parameters: uarray2 (the UArray2_T to check width for)
 *
 *      Returns: the width as an int.
 *
 * Expectations: uarray2 should be non-NULL.
 *
*/
extern int UArray2_width(T uarray2);


/* UArray2_height
 *
 *      Purpose: Return the height of the 2d Uarray, which is essentially
 *               the number of rows. 
 *   Parameters: uarray2 (the UArray2_T to check height for)
 *
 *      Returns: the height as an int.
 *
 * Expectations: uarray2 should be non-NULL.
 *
*/
extern int UArray2_height(T uarray2);

/* UArray2_size
 *
 *      Purpose: Return the size of each element of the 2d Uarray
 *   Parameters: uarray2 (the UArray2_T to check width for)
 *
 *      Returns: the size of each element as an int.
 *
 * Expectations: uarray2 should be non-NULL.
 *
*/
extern int UArray2_size(T uarray2);


/* UArray2_at
 *
 *      Purpose: Return a pointer to the element at given column and row.
 *   Parameters: uarray2 (the UArray2_T to check width for), col
 *               (the column index), and row (the row index).
 *
 *      Returns: void pointer to the element, which can be cast as the
 *               known actual type of the elements of uarray2
 *
 * Expectations: uarray2 should be non-NULL. col and row should be 
 *               within the bounds of the array's dimensions. 
 *
*/
extern void *UArray2_at(T uarray2, int col, int row);


/* UArray2_map_col_major
 *
 *      Purpose: run the apply function on every element of uarray2, 
 *               in column major order, meaning each full column receives
 *               apply before moving onto the next column.
 *   Parameters: uarray2 (the UArray2_T to check width for),
 *               apply (the apply function for each element), and a 
 *               closure pointer if needed
 *
 *      Returns: none
 *
 * Expectations: uarray2 should be non-NULL. closure pointer can be NULL.
 *
*/
extern void UArray2_map_col_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                        void *val, void *cl),
                                                        void *cl);


/* UArray2_map_row_major
 *
 *      Purpose: run the apply function on every element of uarray2, 
 *               in row major order, meaning each full row receives
 *               apply before moving onto the next row.
 *   Parameters: uarray2 (the UArray2_T to check width for),
 *               apply (the apply function for each element), and a 
 *               closure pointer if needed
 *
 *      Returns: none
 *
 * Expectations: uarray2 should be non-NULL. closure pointer can be NULL.
 *
*/
extern void UArray2_map_row_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                 void *val, void *cl), 
                                                 void *cl);

#undef T
#endif

