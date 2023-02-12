/**************************************************************
 *
 *                     restore.c
 *
 *     Assignment: Homework 1, Filesofpix
 *     Authors: Roman Short (rshort02), Henry Lee (jelee02)
 *     Date: January 31 2023
 *
 *     summary
 *     Implementation of restore interface. Uncorrupts the 
 *     provided file and calls fix_file from FixFile.h
 *     
 * 
 *
 **************************************************************/

#include "restore.h"

// function contracts
// review code standards to ensure we followed them 
// really really make sure valgrind behaves correctly 

/* restore
 *
 *    Purpose: Driver for restore program. loops through file until
 *             datapp is NULL, i.e. reached EOF
 * Parameters: file to read and uncorrupt
 *    Returns: None
*/
void restore(FILE *fp)
{
        assert(fp != NULL);

        char *datapp;
        char *correct_key = NULL;
        int line_bytes; 
        Table_T table = Table_new(1, NULL, NULL);
        Seq_T original_lines = Seq_new(0);
        while (true) {
                line_bytes = readaline(fp, &datapp);
                if (datapp == NULL) {
                        break;
                } 
                manage_line(datapp, line_bytes, table,
                             original_lines, &correct_key); 
        }
        fix_file(original_lines);
        Table_map(table, free_table, NULL);
        Table_free(&table);
        free_sequences(original_lines);
        free(correct_key);
}

/* free_sequences
 *
 *    Purpose: free the original_lines Hanson sequence that contains 
 *             the sequences of digits, by freeing the internal sequences.
 * Parameters: original_lines sequence to free.
 *    Returns: None
*/
void free_sequences(Seq_T original_lines)
{
        int len = Seq_length(original_lines);
        for (int i = 0; i < len; i++) {
                Seq_T current = Seq_remlo(original_lines);
                free_sequence(current);
                
        }
        Seq_free(&original_lines);
}

/* free_sequence
 *
 *    Purpose: free the contents of the provided Hanson sequence, then free
 *             the sequence itself. 
 * Parameters: the sequence to be emptied and freed.
 *    Returns: None
*/
void free_sequence(Seq_T curr_seq)
{
        int len = Seq_length(curr_seq);
        for (int i = 0; i < len; i++) {
                free((int *) Seq_remlo(curr_seq));
        }

        Seq_free(&curr_seq);
}

/* free_table
 *
 *    Purpose: free the contents of the Hanson table that contains key-value 
 *             pairs for characters (filler) and numbers (digits). 
 * Parameters: the key, value, and pointer to pass to the Hanson Table_map
 *             function
 *    Returns: None
*/
void free_table(const void *key, void **value, void *cl)
{
    Seq_T *curr_seq = (Seq_T *) value;
    free_sequence(*curr_seq);
    (void)key;
    (void)cl;
}

/* manage_line
 *
 *    Purpose: manage an individual line read by readaline. overarchingly, 
 *             add or don't add the line's numerical contents 
 *             to original_lines.
 * Parameters: char* to be processed, number of bytes in the line read, 
 *             Hanson table, Hanson sequence of overall lines of numbers,
 *             pointer to correct_key char*
 *    Returns: None
*/
void manage_line(char *datapp, int line_bytes, Table_T table, 
                  Seq_T original_lines, char **correct_key)
{   
        char *filler = (char *) malloc(line_bytes); 
        assert(filler != NULL);

        Seq_T digits = Seq_new(0);                  
        split_line(datapp, line_bytes, digits, filler);
        
        if (*correct_key != NULL) {
                if (strcmp(filler, *correct_key) == 0) { 
                        Seq_addhi(original_lines, digits); 
                } else {
                        free_sequence(digits);
                }
        } else {
                bool found = manage_table(table, filler, digits,
                                          original_lines);
                if (found) {
                        *correct_key = make_heap_array(filler);                        
                }
        }

        free(datapp);
        free(filler);

        return;
}

/* split_line
 *
 *    Purpose: split an individual line read by readaline into a sequence
 *             of digits and array of non-digit characters. 
 * Parameters: char* to be processed, number of bytes in the line read, 
 *             Hanson sequence of digits to be populated,
 *             char* for the non-digit characters. 
 *    Returns: None
*/
void split_line(char *datapp, int line_bytes, Seq_T digits, char *filler)
{
        char c;
        int char_counter = 0;
        for (int i = 0; i < line_bytes; i++) {
                c = datapp[i];
                if (!isdigit(c)) {
                        if (c == '\0') {
                                c = '0';
                        }
                        if (c == '\n') {
                                c = '\0';
                        }
                        filler[char_counter] = c;
                        char_counter++;
                } else {
                        int num = c - '0';
                        int int_counter = 1;
                        while (true) {
                                char next_char = datapp[i + int_counter];
                                if (!isdigit(next_char)) {
                                        break;
                                }
                                c = next_char;
                                num = num * 10 + (c - '0');
                                int_counter++;
                        }
                        i = i + int_counter - 1;
                        int *num_p = (int *) malloc(sizeof(int));
                        assert(num_p != NULL);
                        *num_p = num;
                        Seq_addhi(digits, num_p);
                }
        }
}

/* manage_table
 *
 *    Purpose: manage an individual line read by readaline. overarchingly, 
 *             add or don't add the line's numerical contents 
 *             to original_lines.
 * Parameters: Hanson table, char* for the non-digit characters,
 *             Hanson sequence of digits, Hanson sequence of overall 
 *             lines (sequences) of numbers.
 *         
 *    Returns: bool for if correct_key has been found, i.e. Table_get
 *             did not return null. 
*/
bool manage_table(Table_T table, char *filler, Seq_T digits, 
                  Seq_T original_lines)
{
        const char *atom_filler = Atom_string(filler);

        if (Table_get(table, atom_filler) != NULL) {
                Seq_addhi(original_lines, Table_remove(table, atom_filler));
                Seq_addhi(original_lines, digits);

                
                return true;
        } else {
                Table_put(table, atom_filler, digits);
        }
        return false;
}

/* make_heap_array
 *
 *    Purpose: add the filler that is the correct_key to the heap 
 *             so that correct_key can be used with strcmp in 
 *             manage_line. Not doing the memcpy below and 
 *             assigning *correct_key = filler did not work 
 *             with the levels of absraction we have. 
 * Parameters: char* for the characters that compose the correct_key,
 *             i.e. the injected sequence for lines from original file.
 *         
 *    Returns: the char * for correct_key. 
*/
char *make_heap_array(char *filler) 
{
        char *heap_array = (char *) malloc(strlen(filler) + 1);
        assert(heap_array != NULL);

        memcpy(heap_array, filler, strlen(filler) + 1);
        assert(heap_array != NULL);
        
        return heap_array;
}