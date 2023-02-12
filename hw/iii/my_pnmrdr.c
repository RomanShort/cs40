#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "mem.h"
#include "my_pnmrdr.h"

#define T My_Pnmrdr_T

static void read_token(FILE *source, const char *fmt, void *address);

struct T {
        My_Pnmrdr_mapdata data;
        FILE *source;
        const char *plain_format;     /* for a plain format, format
                                       * string used to read a pixel:
                                       * %1d for bitmaps; %u for others.
                                       * for a raw format, NULL.
                                       */
        unsigned (*read)(T);          /* read next integer                  */
        unsigned ints_left;
        bool     got_an_int;
        unsigned bits_left_in_row;    /* number of bits left in current row */
        unsigned char current_byte;   /* last byte read in raw format       */
        unsigned char next_bit_mask;  /* mask of next bit to be read 
                                       * (0 if all bits read)
                                       */
};

#include "stackoverflow.h"
 // Roman had to add this
My_Pnmrdr_mapdata My_Pnmrdr_data (T rdr) { return rdr->data; }

static unsigned read_plain(T rdr)
{
        unsigned n;
        int rc = fscanf(rdr->source, rdr->plain_format, &n);

        if (rc != 1)
                RAISE(My_Pnmrdr_Badformat);
        return n;
}

static unsigned read_raw_bit(T rdr) 
{
        if (rdr->bits_left_in_row == 0) {
                rdr->bits_left_in_row = rdr->data.width;
                rdr->next_bit_mask = 0;
        }
        if (rdr->next_bit_mask == 0) {
                int c = getc(rdr->source);
                if (c == EOF)
                        RAISE(My_Pnmrdr_Count);
                rdr->next_bit_mask = 1<<7;
                rdr->current_byte = c;
        }
        unsigned bit = (rdr->current_byte & rdr->next_bit_mask) != 0;
        rdr->bits_left_in_row--;
        rdr->next_bit_mask >>= 1;
        return bit;
}

static unsigned read_raw_char(T rdr)
{
        int c = getc(rdr->source);
        if (c == EOF)
                RAISE(My_Pnmrdr_Count);
        return c;
}

static unsigned read_raw_pair(T rdr)
{
        int hi = getc(rdr->source);
        int lo = hi == EOF ? EOF : getc(rdr->source);
        if (lo == EOF)
                RAISE(My_Pnmrdr_Count);
        return (hi << 8) + lo;
}

unsigned My_Pnmrdr_get (T rdr)
{
        assert(rdr && rdr->source);
        if (rdr->ints_left > 0) {
                rdr->got_an_int = true;
                rdr->ints_left--;
                return rdr->read(rdr);
        } else {
                RAISE(My_Pnmrdr_Count);
                /* code not reached, but the compiler doesn't know that, so: */
                return 0;
        }
}

void My_Pnmrdr_free (T *prdr)
{
        unsigned leftover;
        assert(*prdr);
        leftover = (*prdr)->ints_left;
        bool got_an_int = (*prdr)->got_an_int;
        FREE(*prdr);
        if (leftover > 0 && got_an_int)
                RAISE(My_Pnmrdr_Count);
}

void read_token(FILE *source, const char *fmt, void *address) 
{
        int rc;
        int c;
        for (;;) { /* skip comments */
                do { c = getc(source); } while (c != EOF && isspace(c)); /* skip whitespace */
                if (c == '#') { /* scan to newline */
                        do { c = getc(source); } while (c != EOF && c != '\n');
                        if (c == EOF) {
                                RAISE(My_Pnmrdr_Badformat);
                        }
                /* otherwise continue the for loop */
                } else if (c == EOF) {
                        RAISE(My_Pnmrdr_Badformat);
                } else { /* put back the non-space, non-comment character c & we're done */
                        ungetc(c, source); 
                        break; 
                }
        }
        rc = fscanf(source, fmt, address);
        if (rc != 1)
                RAISE(My_Pnmrdr_Badformat);
}


T My_Pnmrdr_new (FILE *fp) {
        do {
                static int initialized;
                if (!initialized) {
                        initialized = 1;
                        report_stack_overflow();
                }
        } while(0);
        assert(fp);
        T rdr = ALLOC(sizeof(*rdr));
        TRY
                rdr->source = fp;
                //static void read_token(FILE *source, const char *fmt, void *address);

                /* read first token and check for a pbmplus 'magic number' */
                char token[20];   // used to read 'magic number'
                read_token(rdr->source, "%10s", token); /* 10 is much smaller than sizeof(token) */
                if (token[0] != 'P' || token[1] < '1' || token[1] > '6' || token[2] != '\0') {
                        RAISE(My_Pnmrdr_Badformat);
                        return NULL; // code not reached, but the compiler doesn't know that
                } else {
                /* we have a magic number; set type, integer format, and reader function */
                switch (token[1]) {
                        case '1': rdr->data.type = Pnmrdr_bit;  
                                rdr->plain_format = "%1d"; break;
                        case '2': rdr->data.type = Pnmrdr_gray; 
                                rdr->plain_format = "%u";  break;
                        case '3': rdr->data.type = Pnmrdr_rgb;  
                                rdr->plain_format = "%u";  break;
                        case '4': rdr->data.type = Pnmrdr_bit;  
                                rdr->plain_format = NULL;  break;
                        case '5': rdr->data.type = Pnmrdr_gray; 
                                rdr->plain_format = NULL;  break;
                        case '6': rdr->data.type = Pnmrdr_rgb;  
                                rdr->plain_format = NULL;  break;
                        default : assert(0); break;
                }
                
                /* read the rest of the header and return success */
                        read_token(rdr->source, "%u", &rdr->data.width);
                        read_token(rdr->source, "%u", &rdr->data.height);
                if (rdr->data.type == My_Pnmrdr_bit)
                        rdr->data.denominator = 1;
                else
                        read_token(rdr->source, "%u", &rdr->data.denominator);
                int c = getc(fp); // single whitespace or newline following denominator
                if (!isspace(c))
                        RAISE(My_Pnmrdr_Badformat);
                switch(token[1]) {
                        case '1': case '2': case '3':
                                rdr->read = read_plain;
                                break;
                        case '4':
                                rdr->bits_left_in_row = 0; // trigger read of next byte
                        r       dr->read = read_raw_bit;
                                break;
                        case '5': case '6':
                                rdr->read = rdr->data.denominator > 255 ? read_raw_pair : read_raw_char;
                                break;
                }
                rdr->ints_left = rdr->data.width * rdr->data.height;
                rdr->got_an_int = false;
                if (rdr->data.type == My_Pnmrdr_rgb)
                        rdr->ints_left *= 3;
                RETURN rdr;
                }
                ELSE
                FREE(rdr);
                RERAISE;
                return NULL; // code not reached, but the compiler doesn't know that
        END_TRY;
}

switch (token[1]) {
  case '1': rdr->data.type = My_Pnmrdr_bit;  rdr->plain_format = "%1d"; break;
  case '2': rdr->data.type = My_Pnmrdr_gray; rdr->plain_format = "%u";  break;
  case '3': rdr->data.type = My_Pnmrdr_rgb;  rdr->plain_format = "%u";  break;
  case '4': rdr->data.type = My_Pnmrdr_bit;  rdr->plain_format = NULL;  break;
  case '5': rdr->data.type = My_Pnmrdr_gray; rdr->plain_format = NULL;  break;
  case '6': rdr->data.type = My_Pnmrdr_rgb;  rdr->plain_format = NULL;  break;
  default : assert(0); break;
}

switch(token[1]) {
  case '1': case '2': case '3':
    rdr->read = read_plain;
    break;
  case '4':
    rdr->bits_left_in_row = 0; // trigger read of next byte
    rdr->read = read_raw_bit;
    break;
  case '5': case '6':
    rdr->read = rdr->data.denominator > 255 ? read_raw_pair : read_raw_char;
    break;
}
const Except_T My_Pnmrdr_Count = { "Read too many or too few integers in pnm map" };
const Except_T My_Pnmrdr_Badformat = { "Input is not a correctly formatted pnm map" };


