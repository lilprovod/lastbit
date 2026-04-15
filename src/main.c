#include <stdio.h>
#include <string.h>

#include "io.h"
#include "dotprod.h"

typedef enum {
    NORMAL_MODE,
    DEMO_MODE,
} ProgramMode;

int main(int argc, char* argv[])
{
    ProgramMode mode = NORMAL_MODE;
    const char* filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--demo") == 0) {
            mode = DEMO_MODE;
        }
        else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage();
            return 1;
        }
        else {
            if (filename != NULL) {
                fprintf(stderr, "Too many input files\n");
                return 1;
            }

            filename = argv[i];
        }
    }

    if (mode == DEMO_MODE) {
        printf("Demo mode coming soon...\n");
        return 0;
    }

    if (filename == NULL) {
        fprintf(stderr, "No input file specified\n");
        print_usage();
        return 1;
    }

    FILE* fp = fopen(filename, "r");

    if (!fp) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    InputData data;
    InputStatus status = read_input(fp, &data);
    fclose(fp);

    switch (status) {
        case EMPTY_INPUT:
            fprintf(stderr, "Vectors size must be greater than zero\n"); return 1;

        case INCORRECT_SIZE:
            fprintf(stderr, "Incorrect vectors size value\n");           return 1;

        case MALLOC_ERROR:
            fprintf(stderr, "Malloc error\n");                           return 1;

        case INCORRECT_VEC_X:
            fprintf(stderr, "Invalid value detected at vector X (vector 1)\n"); return 1;

        case INCORRECT_VEC_Y:
            fprintf(stderr, "Invalid value detected at vector Y (vector 2)\n"); return 1;
            
        case SUCCESS:
            break;
    }
    
    show_input(&data);

    hello_message();

    double naive_result = dot_naive(data.x, data.y, data.n);
    double kahan_result = dot_kahan(data.x, data.y, data.n);
    double fma_result   = dot_fma(data.x, data.y, data.n);

    show_table(naive_result, kahan_result, fma_result);

    free_input(&data);

    return 0;
}