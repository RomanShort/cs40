/**************************************************************
 *
 *                     Bit2.c
 *
 *     Assignment: Homework 2, iii
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: February 5th, 2023
 *
 *     summary
 *     Implementation of Bit2. Implements the functions defined
 *     by Bit2.h for the client to interact with a 2d Bit array
 *     
 **************************************************************/

#include "bit2.h"

#define T Bit2_T

struct T {
    int width, height;
    Bit_T array;
};

T Bit2_new(int width, int height)
{
        assert(width >= 0);
        assert(height >= 0);

        T new_array;
        NEW(new_array);

        new_array->width = width;
        new_array->height = height;
        new_array->array = Bit_new(width * height);

        return new_array;
}

void Bit2_free(T *Bit2)
{
        assert(Bit2 != NULL && *Bit2 != NULL);
        Bit_free(&(*Bit2)->array);
        FREE(*Bit2);
}

int Bit2_width(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->width;
}

int Bit2_height(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->height;
}

int Bit2_get(T Bit2, int col, int row)
{
        assert(Bit2 != NULL);
        assert(col >= 0 && col <= Bit2->width);
        assert(row >= 0 && row <= Bit2->height);

        int idx = col + (row * Bit2->width);
        return Bit_get(Bit2->array, idx);
}

int Bit2_put(T Bit2, int col, int row, int bit)
{
        assert(Bit2 != NULL);
        assert(col >= 0 && col <= Bit2->width);
        assert(row >= 0 && row <= Bit2->height);
        assert(bit == 0 || bit == 1);

        int idx = col + (row * Bit2->width);
        return Bit_put(Bit2->array, idx, bit);
}

void Bit2_map_row_major(T Bit2, void apply(int i, int j, T Bit2,
                               int bit, void *cl), void *cl)
{
        assert(Bit2 != NULL);

        for (int j = 0; j < Bit2->height; j++) {
                for (int i = 0; i < Bit2->width; i++) {
                        int idx = i + (j * Bit2->width);
                        int current_bit = Bit_get(Bit2->array, idx);
                        apply(i, j, Bit2, current_bit, cl);
                }
        }
}

void Bit2_map_col_major(T Bit2, void apply(int i, int j, T Bit2,
                               int bit, void *cl), void *cl)
{
        assert(Bit2 != NULL);

        for (int i = 0; i < Bit2->height; i++) {
                for (int j = 0; j < Bit2->width; j++) {
                        int idx = i + (j * Bit2->width);
                        int current_bit = Bit_get(Bit2->array, idx);
                        apply(i, j, Bit2, current_bit, cl);
                }
        }
}
