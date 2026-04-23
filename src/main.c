#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "demo.h"
#include "dotprod.h"

#define MAX_SHOW_VECTOR 10

typedef enum {
    NORMAL_MODE,
    DEMO_MODE,
} ProgramMode;


int main(int argc, char* argv[])
{
    ProgramMode mode = NORMAL_MODE;
    const char* filename = NULL;

    OzakiConfig ozaki_config = OZAKI_CONFIG_DEFAULT;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--demo") == 0) {
            mode = DEMO_MODE;
        }
        else if (strcmp(argv[i], "-k") == 0) {
            if (i + 1 < argc) {
                char* endptr;
                size_t k = strtoull(argv[i + 1], &endptr, 10);

                if (argv[i + 1] == endptr) {
                    fprintf(stderr, "Ozaki layers value must be integer ('-k')\n");
                }

                ozaki_config.ozaki_layers = k;
            } else {
                fprintf(stderr, "Expected integer value for '-k'\n");
            }
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
        run_demo(&ozaki_config);
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
    
    if (data.n < MAX_SHOW_VECTOR) show_input(&data);

    hello_message();

    DotResults results = compute_all_dot(data.x, data.y, data.n, &ozaki_config);
    show_table_dot(&results, &ozaki_config);

    free_input(&data);

    return 0;
}