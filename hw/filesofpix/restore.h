/**************************************************************
 *
 *                     restore.h
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Interface of restore program. Defines the necessary 
 *     functions for restore.c
 *     
 * 
 *
 **************************************************************/

#ifndef RESTORE_H
#define RESTORE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <atom.h>
#include <seq.h>
#include <table.h>
#include <stdbool.h> 
#include <ctype.h>

#include "FixFile.h"
#include "readaline.h"

extern void print_line(char *datapp);

extern char *make_heap_array(char *filler);

extern void free_table(const void *key, void **value, void *cl);
extern void free_sequences(Seq_T original_lines);
extern void free_sequence(Seq_T curr_seq);

extern void restore(FILE *fp);
extern void split_line(char *datapp, int line_bytes, Seq_T digits, 
                       char *filler);
extern void manage_line(char *datapp, int line_bytes, Table_T table, 
                        Seq_T original_lines, char **correct_key);

extern bool manage_table(Table_T table, char *filler, Seq_T digits, 
                         Seq_T original_lines);


#endif