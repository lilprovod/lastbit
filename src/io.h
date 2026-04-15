#ifndef IO_H
#define IO_H

#include <stdio.h>

#define MAX_NUM_SIZE        128

typedef struct {
    size_t n;
    double* x;
    double* y;
} InputData;

typedef enum {
    SUCCESS,
    EMPTY_INPUT,
    INCORRECT_SIZE,
    MALLOC_ERROR,
    INCORRECT_VEC_X,
    INCORRECT_VEC_Y,
} InputStatus;


InputStatus read_input(FILE* fp, InputData* data);

void free_input(InputData* data);

void print_usage();

void hello_message();

void show_table(
    double naive, double kahan, double kbn2,
    double kbn3,  double oo,    double fma,
    double ref
);

//!DEBUG
void show_input(InputData* data);


#endif