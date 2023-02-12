#ifndef MY_PNMRDR_INCLUDED
#define MY_PNMRDR_INCLUDED
#include "except.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define T My_Pnmrdr_T
typedef struct T *T;

extern const Except_T My_Pnmrdr_Badformat;   /* raised when not a pnm file   */
extern const Except_T My_Pnmrdr_Count; /* raised when wrong number of          */
                                    /* pixels read                          */
typedef enum { My_Pnmrdr_bit = 1, My_Pnmrdr_gray = 2, My_Pnmrdr_rgb = 3 } 
        My_Pnmrdr_maptype;

typedef struct {
        My_Pnmrdr_maptype type;
        unsigned width, height;
        unsigned denominator;     /* (gray & color) used to scale integers  */
                                  /* to be read                             */
} My_Pnmrdr_mapdata;

extern char *My_Pnmrdr_maptype_names[];

extern T My_Pnmrdr_new(FILE *fp);            /* raises My_Pnmrdr_Badformat      */
extern My_Pnmrdr_mapdata My_Pnmrdr_data(T rdr);
extern unsigned My_Pnmrdr_get(T rdr);  /* raises My_Pnmrdr_Count if exhausted     */
extern void My_Pnmrdr_free(T *rdr);    /* raises My_Pnmrdr_Count unless either    */
                                    /* no pixels or all pixels were read    */
void read_token(FILE *source, const char *fmt, void *address); 
#undef T
#endif