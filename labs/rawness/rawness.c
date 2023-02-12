#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

void convert(FILE *fp);

int main(int argc, char *argv[]) {
    if (argc == 1) {
                convert(stdin);
        } else {
                for (int i = 1; i < argc; i++) {
                        FILE *fp = fopen(argv[i], "r");
                        if (fp == NULL) {
                                fprintf(stderr, 
                                        "%s: %s %s %s\n",
                                        argv[0], "Could not open file ",
                                        argv[i], "for reading");
                                exit(1);
                        }
                        convert(fp);
                        fclose(fp);
                }
        }
        return EXIT_SUCCESS;
}

void convert(FILE *fp){
    Pnmrdr_T reader = Pnmrdr_new(fp);
    Pnmrdr_mapdata readerData = Pnmrdr_data(reader);
    unsigned width, height;
    width = readerData.width;
    height = readerData.height;
    
    printf("P5\n%u %u\n%u\n", width, height, readerData.denominator);
    

    for (unsigned i = 0; i < height; i++ ){
        for (unsigned j = 0; j < width; j++){
            
            char char_p = (char) (Pnmrdr_get(reader));
            // char char_p = (char) decimal;
            
            printf("%c", char_p);
        }
        //printf("\n");
    }

    Pnmrdr_free(&reader);
}