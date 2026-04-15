#include <stdlib.h>

#include "dotprod.h"
#include "mul.h"
#include "sum.h"

double dot_naive(const double* x, const double* y, size_t n)
{
    double* terms = malloc(n * sizeof(double));
    if (!terms) { return 0.0; }

    for (size_t i = 0; i < n; i++) {
        terms[i] = mul_naive(x[i], y[i]);
    }

    double sum = sum_naive(terms, n);

    return sum;
}

double dot_kahan(const double* x, const double* y, size_t n)
{
    double* terms = malloc(n * sizeof(double));
    if (!terms) return 0.0;

    for (size_t i = 0; i < n; i++) {
        terms[i] = mul_naive(x[i], y[i]);
    }

    double sum = sum_kahan(terms, n);

    free(terms);
    return sum;
}

double dot_fma(const double* x, const double* y, size_t n)
{
    double* hi_terms = malloc(n * sizeof(double));
    if (!hi_terms) { return 0.0; }

    double* lo_terms = malloc(n * sizeof(double));
    if (!lo_terms) { free(hi_terms); return 0.0; }

    for (size_t i = 0; i < n; i++) {
        mul_fma(x[i], y[i], &hi_terms[i], &lo_terms[i]);
    }

    double hi_sum = sum_kahan(hi_terms, n);
    double lo_sum = sum_kahan(lo_terms, n);

    double sum = hi_sum + lo_sum;

    free(hi_terms);
    free(lo_terms);

    return sum;
}