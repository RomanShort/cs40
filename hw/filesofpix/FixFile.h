/**************************************************************
 *
 *                     FixFile.h
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Interface of FixFile program. Defines the functions for
 *     FixFile.c.
 *     
 * 
 *
 **************************************************************/

#ifndef FIXFILE_H
#define FIXFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <atom.h>
#include <seq.h>
#include <table.h>
#include <stdbool.h> 
#include <ctype.h>


extern void fix_file(Seq_T original_lines);
extern void print_pgm(Seq_T original_lines, int height, int width);

#endif