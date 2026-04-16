#include <stdio.h>
#include <stdlib.h>

#include "io.h"

InputStatus read_input(FILE* fp, InputData* data)
{
    char buffer_n[MAX_NUM_SIZE];
    char* endptr = NULL;

    data->n = 0;
    data->x = NULL;
    data->y = NULL;

    if (!fgets(buffer_n, sizeof(buffer_n), fp)) {
        return INCORRECT_SIZE;
    }

    data->n = (size_t)strtoull(buffer_n, &endptr, 10);

    if (endptr == buffer_n) {
        return INCORRECT_SIZE;
    }

    if (data->n == 0) {
        return EMPTY_INPUT;
    }

    data->x = malloc(data->n * sizeof(double));
    if (data->x == NULL) return MALLOC_ERROR;

    data->y = malloc(data->n * sizeof(double));
    if (data->y == NULL) { free_input(data); return MALLOC_ERROR; }

    for (size_t i = 0; i < data->n; i++) {
        if (fscanf(fp, "%lf", &data->x[i]) != 1) {
            free_input(data);
            return INCORRECT_VEC_X;
        }
    }

    for (size_t i = 0; i < data->n; i++) {
        if (fscanf(fp, "%lf", &data->y[i]) != 1) {
            free_input(data);
            return INCORRECT_VEC_Y;
        }
    }

    return SUCCESS;
}

void free_input(InputData* data)
{
    free(data->x);
    free(data->y);

    data->x = NULL;
    data->y = NULL;
    data->n = 0;
}

void print_usage()
{
    fprintf(stderr, "Usage: ./lastbit [filename] [--demo]\n"
                    "Flags:\n"
                    "\t--demo                  \tA demo mode with tests\n"
    );
}

void hello_message()
{
    printf( "\n[Dot product algorithm comparison]\n\n");
}

void show_table_dot(const DotResults* results)
{
    printf("%-12s | %-24s\n", "Method", "Result"                    );
    printf("-------------+------------------------\n"               );
    printf("%-12s | %.17g\n", "naive",       results->naive         );
    printf("%-12s | %.17g\n", "kahan",       results->kahan         );
    printf("%-12s | %.17g\n", "kbn2",        results->kbn2          );
    printf("%-12s | %.17g\n", "kbn3",        results->kbn3          );
    printf("%-12s | %.17g\n", "ogita-oishi", results->ogita_oishi   );
    printf("%-12s | %.17g\n", "fma",         results->fma           );
    printf("%-12s | %.17g\n", "reference*",  results->reference     );
}

void show_table_sum(const SumResults* results)
{
    printf("%-12s | %-24s\n", "Method", "Result"                    );
    printf("-------------+------------------------\n"               );
    printf("%-12s | %.17g\n", "naive",       results->naive         );
    printf("%-12s | %.17g\n", "kahan",       results->kahan         );
    printf("%-12s | %.17g\n", "kbn2",        results->kbn2          );
    printf("%-12s | %.17g\n", "kbn3",        results->kbn3          );
    printf("%-12s | %.17g\n", "ogita-oishi", results->ogita_oishi   );
}

//!DEBUG
void show_input(const InputData* data)
{
    printf(
        "Vectors size: %zu\n"
        "Vector X: [ ",
        data->n
    );

    for (size_t i = 0; i < data->n; i++) {
        printf("%.e ", data->x[i]);
    }

    printf(
        "]\n"
        "Vector Y: [ "
    );

    for (size_t i = 0; i < data->n; i++) {
        printf("%.e ", data->y[i]);
    }

    printf("]\n");
}