/**************************************************************
 *
 *                     Bit.h
 *
 *     Assignment: Homework 2, iii
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Interface of Bit2. Defines the functions for the 
 *     2d Bit array program. 
 *     
 **************************************************************/

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#include <assert.h>
#include "bit.h"
#include <stdlib.h>
#include <stdio.h>
#include <mem.h>

#define T Bit2_T
typedef struct T *T;

extern T Bit2_new(int width, int height);
extern void Bit2_free(T *Bit2);

extern int Bit2_width(T Bit2);
extern int Bit2_height(T Bit2);

extern int Bit2_get(T Bit2, int col, int row);
extern int Bit2_put(T Bit2, int col, int row, int bit);

extern void Bit2_map_row_major(T Bit2, void apply(int i, int j, T Bit2,
                               int bit, void *cl), void *cl);
extern void Bit2_map_col_major(T Bit2, void apply(int i, int j, T Bit2,
                               int bit, void *cl), void *cl);

#undef T 
#endif