#include <stack.h>
#include "Bit2.h"
#include <pnmrdr.h>


struct Bit {
        int col, row, value;
};

FILE *open_file(char *filename);
void process_image(FILE *fp);
Bit2_T pbmread(FILE *fp);
void set_values(int i, int j, Bit2_T bit2_image, int bit, void *cl);
void fix_black_edges(Bit2_T bit2_image);
void run_dfs(Bit2_T bit2_image, int col, int row);

int main (int argc, char *argv[])
{
        assert(argc < 3);
        
        FILE *fp;
        if (argc == 1) {
                assert(stdin != NULL);  
                fp = stdin;
                assert(fp != NULL);
        } else {
                fp = open_file(argv[1]);
        }
        process_image(fp);
        exit(1);
}

FILE *open_file(char *filename)
{
        assert(filename != NULL);
        FILE *fp = fopen(filename, "rb"); 
        assert(fp != NULL);
        return fp;
}

void process_image(FILE *fp)
{
        assert(fp != NULL); // not rly needed
        Bit2_T bit2_image = pbmread(fp);
        // 1. remove black edge pixels
                // a) remover black edges from row = 0, col = 0, row = h - 1, col = w - 1
                        // while processing every pixel, get neighbors, and call remove on them? 
                // b) remove black edge pixels from within the image
        fix_black_edges(bit2_image);
        // print_fixed_pbm(bit2_image);
        // 2. print the new Bit2_T without the black edge pixels
        
}

void fix_black_edges(Bit2_T bit2_image)
{
        // for col = 0
        for (int row = Bit2_height(bit2_image) - 1; row >= 0; row--) {
                if (Bit2_get(bit2_image, 0, row) == 1) {
                        run_dfs(bit2_image, 0, row);
                }
        }

        // for row = 0


        // for col = w - 1


        // for row = h - 1


}

void run_dfs(Bit2_T bit2_image, int col, int row)
{
        (void) bit2_image;
        (void) col;
        (void) row;
}

Bit2_T pbmread(FILE *fp)
{
        Pnmrdr_T image_rdr = Pnmrdr_new(fp);
        assert(image_rdr != NULL); 
        Pnmrdr_mapdata data = Pnmrdr_data(image_rdr);
        if (data.type != 1 || data.width <= 0 || data.height <= 0) { 
                Pnmrdr_free(&image_rdr);
                assert(data.type == 1 && data.width != 0 && data.height != 0);
                // above line is to ensure we free the pnmrdr reader 
                // before the assert and terminate. not actually necessary 
                // since a CRE can leave memory on the heap. 
                // maybe ask about this? just for style 
        }

        Bit2_T bit2_image = Bit2_new(data.width, data.height);
        Bit2_map_row_major(bit2_image, set_values, image_rdr);
        Pnmrdr_free(&image_rdr);
        fclose(fp);
        return bit2_image; 
}

void set_values(int i, int j, Bit2_T bit2_image, int bit, void *cl)
{
        Bit2_put(bit2_image, i, j, Pnmrdr_get(cl));
        (void) bit;
        // below line should work, can use other if not.
        // bit = (int) Pnmrdr_get(cl);
        // (void) i;
        // (void) j;
        // (void) bit2_image;
}

