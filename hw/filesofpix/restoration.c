/**************************************************************
 *
 *                     restoration.c
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Essentially the "main" file. Opens the provided file,
 *     and calls restore to uncorrupt and output the raw pgm.
 *     Closes the file. 
 *     
 * 
 *
 **************************************************************/



#include "restore.h"

/* function declaration(s): */
void process_file(char *filename);

/* main
 *
 *    Purpose: run the restoration program by opening the file and 
 *             uncorrupting with process_file
 * Parameters: number of arguments provided & array of arguments
 *    Returns: EXIT_SUCCESS on end of program
*/
int main(int argc, char *argv[]) 
{
        assert(argc < 3);
    
        if (argc == 1) {
                char filename[1000];
                printf("%s", "Enter corrupted file: ");
                scanf("%s", filename);
                process_file(filename);
        } else {
                process_file(argv[1]);
        }
        return EXIT_SUCCESS;
}

/* process_file
 *
 *    Purpose: Open the provided file, ensure success, and call restore.
 * Parameters: the filename to open 
 *    Returns: None
*/
void process_file(char *filename)
{
        assert(filename != NULL);
        FILE *fp = fopen(filename, "rb"); /* what's rb? */
        assert(fp != NULL);
        restore(fp);
        fclose(fp);
}