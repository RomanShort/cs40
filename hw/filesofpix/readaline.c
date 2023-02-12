/**************************************************************
 *
 *                     readaline.c
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Implementation of readaline interface. Reads one line 
 *     using the provided file descriptor into the provided 
 *     char *.
 *     
 * 
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <except.h>

#include "readaline.h"

/* readaline
 *
 *    Purpose: read a single line from the provided file descriptor,
 *             and populate the provided char * with the resulting line
 * Parameters: file descriptor to read from, pointer to char * to
 *             populate 
 *    Returns: the number of bytes read into *datapp.
*/
size_t readaline(FILE *inputfd, char **datapp)
{
        assert(inputfd != NULL);
        assert(datapp != NULL);

        char c = fgetc(inputfd);
        if (c == EOF) {
                *datapp = NULL;
                return 0;
        }

        int size = 1000;
        int bytesReadIdx = 0;

        char *lineArray = (char *) malloc(size * sizeof(char));
        assert(lineArray != NULL);

        while (c != '\n') {
                if (bytesReadIdx == size - 1) {
                        size = size * 2;
                        lineArray = (char *) realloc(lineArray, 
                                                     size * sizeof(char));
                        assert(lineArray != NULL);
                }

                lineArray[bytesReadIdx] = c;
                bytesReadIdx++;
                c = fgetc(inputfd);
        }

        lineArray[bytesReadIdx] = '\n';  
        lineArray = (char *) realloc(lineArray, 
                                     (bytesReadIdx + 1) * sizeof(char));
        *datapp = lineArray;

        return (size_t) (bytesReadIdx + 1); 
}