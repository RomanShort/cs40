#include <stack.h>
#include "Bit2.h"
#include <pnmrdr.h>
#include <stdbool.h>

struct Bit_struct {
        int col, row, value;
};

FILE *open_file(char *filename);
void process_image(FILE *fp);
Bit2_T pbmread(FILE *fp);
void set_values(int i, int j, Bit2_T bit2_image, int bit, void *cl);
void fix_black_edges(Bit2_T bit2_image);
void run_dfs(Bit2_T bit2_image, int col, int row);
bool in_image_bounds(Bit2_T bit2_image, int col, int row);
void check_neighbors(struct Bit_struct *curr_bit, 
                     Bit2_T bit2_image, Stack_T stack);
struct Bit_struct *make_bit(Bit2_T bit2_image, int col, int row);
void check_neighbor(Bit2_T bit2_image, Stack_T stack, int col, int row);
void print_fixed_pbm(Bit2_T bit2_image);
void print_helper(int i, int j, Bit2_T bit2_image, int bit, void *cl);

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
        return 0;
        // exit(1);
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
        print_fixed_pbm(bit2_image);
        printf("%s", "\n\n");
        fix_black_edges(bit2_image);
        print_fixed_pbm(bit2_image);
        Bit2_free(&bit2_image);
        // 2. print the new Bit2_T without the black edge pixels
        
}

void print_fixed_pbm(Bit2_T bit2_image)
{
        printf("%s\n%d %d\n", "P1", Bit2_width(bit2_image), Bit2_height(bit2_image));
        Bit2_map_row_major(bit2_image, print_helper, NULL);
}

void print_helper(int i, int j, Bit2_T bit2_image, int bit, void *cl)
{
        printf("%d", bit);
        
        if (i == Bit2_width(bit2_image) - 1) {
                printf("%s", "\n");
        }
        (void) j;
        (void) cl;
}

void fix_black_edges(Bit2_T bit2_image)
{
        // for col = 0
        for (int row = Bit2_height(bit2_image) - 1; row >= 0; row--) {
                if (Bit2_get(bit2_image, 0, row) == 1) {
                        printf("%s %d%s", "running dfs on 0,", row, "\n");
                        run_dfs(bit2_image, 0, row);
                }
        }
        //printf("%s", "made it after col 0");
        // for row = 0
        for (int col = 0; col < Bit2_width(bit2_image); col++) {
                if (Bit2_get(bit2_image, col, 0) == 1) {
                        run_dfs(bit2_image, col, 0);
                }
        }
        //printf("%s", "made it after row 0");
        // for col = w - 1
        for (int row = 0; row < Bit2_height(bit2_image); row++) {
                if (Bit2_get(bit2_image, 
                             Bit2_width(bit2_image) - 1, row) == 1) {
                        run_dfs(bit2_image, Bit2_width(bit2_image) - 1, row);
                }
        }
        //printf("%s", "made it after col last");
        // for row = h - 1
        for (int col = Bit2_width(bit2_image) - 1; col >= 0; col--) {
                if (Bit2_get(bit2_image, 
                             col, Bit2_height(bit2_image) - 1) == 1) {
                        run_dfs(bit2_image, col, 0);
                }
        }
        //printf("%s", "made it after row bottom");
}

void run_dfs(Bit2_T bit2_image, int col, int row)
{
        printf("%s %d%s", "in dfs, running dfs on 0,", row, "\n");

        Stack_T stack = Stack_new();
        struct Bit_struct *curr_bit; // = make_Bit(bit2_image, col, row);
        curr_bit = malloc(sizeof(*curr_bit));
        curr_bit->value = Bit2_get(bit2_image, col, row); // (1)
        curr_bit->col = col; // (0)
        curr_bit->row = row; // (4)
        printf("%s %d %d %d %s", "in dfs, curr bit col, row, val: ", 
                curr_bit->col, curr_bit->row, 
                curr_bit->value, "\n");
        Stack_push(stack, curr_bit); // stack contains (0,4)
        while (Stack_empty(stack) == 0) { // or better practice for != 1? 
                curr_bit = (struct Bit_struct *) Stack_pop(stack); // (0,4)
                printf("%s %d %d %d %s", "in while, popped col, row, val: ", 
                        curr_bit->col, curr_bit->row, 
                        curr_bit->value, "\n");
                check_neighbors(curr_bit, bit2_image, stack);
                printf("%s", "after check neighbors call\n");
                Bit2_put(bit2_image, curr_bit->col, curr_bit->row, 0);
                free(curr_bit);
        }
        //free(curr_bit);
        Stack_free(&stack);
}

void check_neighbor(Bit2_T bit2_image, Stack_T stack, int col, int row)
{
        printf("%s %d %d %s", "in c_n, curr bit col, row: ", 
                col, row, "\n");
        if (in_image_bounds(bit2_image, col, row)) {
                if (Bit2_get(bit2_image, col, row) == 1) {
                        struct Bit_struct *neighbor_bit = make_bit(bit2_image,
                                                                col, row);
                        Stack_push(stack, neighbor_bit);
                }
        }
}

void check_neighbors(struct Bit_struct *curr_bit, 
                     Bit2_T bit2_image, Stack_T stack)
{
        printf("%s %d %d %d %s", "in c_ns, curr bit col, row, val: ", 
                curr_bit->col, curr_bit->row, 
                curr_bit->value, "\n");
        int curr_row = (curr_bit->row) - 1;
        int curr_col = curr_bit->col;
        check_neighbor(bit2_image, stack, curr_col, curr_row);

        curr_row += 1;
        curr_col += 1;
        check_neighbor(bit2_image, stack, curr_col, curr_row);
        
        curr_row += 1;
        curr_col -= 1;
        check_neighbor(bit2_image, stack, curr_col, curr_row);

        curr_row -= 1;
        curr_col -= 1;
        check_neighbor(bit2_image, stack, curr_col, curr_row);
}

struct Bit_struct *make_bit(Bit2_T bit2_image, int col, int row)
{
        struct Bit_struct *b;
        b = malloc(sizeof(*b));
        b->value = Bit2_get(bit2_image, col, row);
        b->col = col;
        b->row = row;
        return b;
}

bool in_image_bounds(Bit2_T bit2_image, int col, int row)
{
        return col >= 0 && col < Bit2_width(bit2_image) &&
               row >= 0 && row < Bit2_height(bit2_image);
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

