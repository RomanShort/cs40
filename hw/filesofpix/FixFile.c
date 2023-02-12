/**************************************************************
 *
 *                     FixFile.c
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Implementation of FixFile interface. Outputs the raw pgm 
 *     using the provided sequence of sequences of numbers.
 *     
 * 
 *
 **************************************************************/

#include "FixFile.h"
#include "pnmrdr.h"

/* fix_file
 *
 *    Purpose: get height and width from original_lines sequence.
 *             call print_pgm.
 * Parameters: the sequence of sequences of numbers to be printed
 *    Returns: None
*/
void fix_file(Seq_T original_lines)
{
        int height = Seq_length(original_lines);
        int width = Seq_length(Seq_get(original_lines, 0));
        
        print_pgm(original_lines, height, width);
}

/* print_pgm
 *
 *    Purpose: print the uncorrupted pgm as a P5.
 * Parameters: the sequence of sequences of numbers, the number of sequences
 *             (rows) and the number of values to be printed per row (cols).
 *    Returns: None
*/
void print_pgm(Seq_T original_lines, int height, int width)
{
        printf("%s\n%d %d\n%d\n", "P5", width, height, 255);

        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        printf("%c", 
                               *(int *) Seq_get(Seq_get(original_lines, 
                                                        i), j));
                }
        }
}
