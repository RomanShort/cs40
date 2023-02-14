/**************************************************************
 *
 *                     sudoku.c
 *
 * Assignment: Hw2 iii
 * Authors: Roman Short (rshort02), Henry Lee (jelee02)
 * Date: Feb 13, 2023
 *     
 * This file contains the program used to identify valid sudoku
 * solutions. The sudoku program operates as a predicate by returning
 * the exit code of the program as either 0 (EXIT_SUCCESS) or 1 (EXIT_FAILURE)
 * to indicate whether the puzzle is solved or not. 
 *
 **************************************************************/

#include "uarray2.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pnmrdr.h"
#include <math.h>


struct rdr_struct {
        Pnmrdr_T rdr;
        bool valid;
};

FILE *open_file(char *filename);
bool process_file(FILE *fp);
UArray2_T pgmread(FILE *fp);
bool check_sudoku(UArray2_T sudoku_grid, int helper(UArray2_T sudoku_grid,
                                                    int chunk,
                                                    int idx));
bool check_properties(Pnmrdr_mapdata data);
void set_values(int i, int j, UArray2_T grid, void *val, void *cl);
int row_helper(UArray2_T grid, int chunk, int idx);
int col_helper(UArray2_T grid, int chunk, int idx);
int mini_grid_helper(UArray2_T grid, int chunk, int idx);

/* function: main()
 * parameter: argc, the number of arguments passed to the program
 * and argv, the array of arguments
 * purpose: to start the sudoku program
 /
int main(int argc, char *argv[])
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
        bool is_solved_sudoku = process_file(fp);
        if (is_solved_sudoku) {
                return 0;
        } 
        return 1;
        
        //exit(1); // should not get here
}

/* function: row_helper
 * parameter: a grid of numbers as an 2D array, the size of a row(width), 
   and an index of a number within the row 
 * purpose: to get the numbers from a row
 */
int row_helper(UArray2_T grid, int chunk, int idx) 
{
    return *((int *) UArray2_at(grid, chunk, idx));
}

/* function: col_helper
 * parameter: a grid of numbers as an 2D array, the size of a column(height),
   and an index of a number within a columnn 
 * purpose: to get the numbers from a column
 * 
 */
int col_helper(UArray2_T grid, int chunk, int idx) 
{
    return *((int *) UArray2_at(grid, idx, chunk));
}

/* function: mini_grid_helper
 * parameter: a grid of numbers as an 2D array, the size of a 3*3 matrix, 
   and an index of a number within 3*3 matrix. 
 * purpose: to get the numbers from a mini grid
 * 
 */
int mini_grid_helper(UArray2_T grid, int chunk, int idx) 
{
    return  *((int *) UArray2_at(grid, 3 * (chunk / 3) + idx % 3,
                                3 * (chunk / 3) + idx / 3));
}

/* function: check_sudoku
 * parameter: a grid of numbers as an 2D array, the size of a 3*3 matrix, 
   and an index of a number within 3*3 matrix. 
 * purpose: create a boolean array and if a pixel has a same intensity return true
 * otherwise, return false
 * 
 */
bool check_sudoku(UArray2_T sudoku_grid, int helper(UArray2_T sudoku_grid,
                                                    int chunk,
                                                    int idx))
{       
        int chunk_size = sqrt(UArray2_width(sudoku_grid) * 
                              UArray2_height(sudoku_grid));
        
        for (int chunk = 0; chunk < chunk_size; chunk++) {
                bool numbers[UArray2_width(sudoku_grid)];
                unsigned numbers_size = sizeof(numbers)/sizeof(bool);

                for (unsigned i = 0; i < numbers_size; i++) {
                        numbers[i] = false;
                }
                for (int i = 0; i < chunk_size; i++) {
                        numbers[helper(sudoku_grid, 
                                       chunk, i) - 1] = true;
                }
                for (unsigned i = 0; i < numbers_size; i++) {
                        if (numbers[i] == false) {
                                return false;
                        }
                } 
        }
        return true;
}

/* function: check_properties
 * parameter: the struct Pnmrdr_mapdata
 * purpose: A solved sudoku puzzle's the maximum pixel intensity is 9
 * so set its width, height, and denominator as 9
 */
bool check_properties(Pnmrdr_mapdata data)
{
        unsigned required_val = 9;
        return (data.width == required_val && 
                data.height == required_val && 
                data.denominator == required_val);
}

/* function: pgm_read
 * parameter: a file pointer
 * purpose: A solved sudoku puzzle's the maximum pixel intensity is 9
 * so set its width, height, and denominator as 9
 */
UArray2_T pgmread(FILE *fp)
{
        Pnmrdr_T image_rdr = Pnmrdr_new(fp);
        assert(image_rdr != NULL); // QUESTION: IS THIS SUITABLE TO ENSURE "CORRECT" INPUT?
        Pnmrdr_mapdata data = Pnmrdr_data(image_rdr);
        if (data.type != 2 && data.type != 5) { // WITH THIS AS WELL? OR TRY/EXCEPT INSTEAD?
                Pnmrdr_free(&image_rdr);
                assert(data.type == 2 || data.type == 5);
        }


        bool is_sudoku_file = check_properties(data);
        if (!is_sudoku_file) {
                Pnmrdr_free(&image_rdr);
                fclose(fp);
                exit(1); 
                //return NULL; // shouldn't get here? 
        } 
        UArray2_T grid = UArray2_new(data.width, data.height, sizeof(int));

        // method 1: 
        
        struct rdr_struct *r;
        r = malloc(sizeof(*r));
        assert(r != NULL);
        r->rdr = image_rdr;
        r->valid = true;
        UArray2_map_row_major(grid, set_values, r);
        Pnmrdr_free(&image_rdr);
        fclose(fp);
                
        if (r->valid == false) {
                free(r);   
                UArray2_free(&grid);
                exit(1);
        }
        free(r);
        // method 2: 
        //UArray2_map_row_major(grid, set_values, image_rdr);
        return grid; 
}

/* function: set_values
 * parameter: A grid of 2d array, a void pointer val, cl. 
 * purpose: to set the values in a grid afterreading from Pnmrdr
 */
void set_values(int i, int j, UArray2_T grid, void *val, void *cl)
{
        
        int current = Pnmrdr_get(cl);
        // *(Pnmrdr_T *)(cl);
        Pnmrdr_T rdr = ((struct rdr_struct *)cl)->rdr;
        int current = Pnmrdr_get(rdr);
        if (current < 1 || current > 9) {
                ((struct rdr_struct *)cl)->valid = false;
                //Pnmrdr_free(&rdr); // causing memory issue..?
                //UArray2_free(&grid);
                //exit(1);
                //return; // not needed?
        }
        *(int *) val = current;

        (void) grid;
        (void) i;
        (void) j;
}
/* function: open_file
 * parameter: a filename, the name of a input graymap file  
 * purpose: to set the values in a grid after reading from Pnmrdr
 */
FILE *open_file(char *filename)
{
        assert(filename != NULL);
        FILE *fp = fopen(filename, "rb"); 
        assert(fp != NULL);
        return fp;
}
/* function: process_file
 * parameter: a file pointer
 * purpose: to check if sudoku is solved or not.
 * read from file and check if in each row, or in each column,
 * or in three-by-three submap, no two pixels have the same intensity. 
 */
bool process_file(FILE *fp)
{
        assert(fp != NULL);
        
        UArray2_T sudoku_grid = pgmread(fp);
        bool check_solved = check_sudoku(sudoku_grid, row_helper) && 
                            check_sudoku(sudoku_grid, col_helper) && 
                            check_sudoku(sudoku_grid, mini_grid_helper);

        // fclose(fp);
        UArray2_free(&sudoku_grid);
        return check_solved;
}
