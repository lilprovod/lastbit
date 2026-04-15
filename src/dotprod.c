#include <stdlib.h>

#include "dotprod.h"
#include "mul.h"
#include "sum.h"

/** @brief Общий интерфейс для скалярного произведения */
static double dot_generic(
    const double* x,
    const double* y,
    size_t        n,
    double        (*mul)(const double, const double),
    double        (*sum)(const double*, size_t)
)
{
    double* terms = malloc(n * sizeof(double));
    if (!terms) { return 0.0; }

    for (size_t i = 0; i < n; i++) {
        terms[i] = mul(x[i], y[i]);
    }

    double total = sum(terms, n);

    free(terms);

    return total;
}

double dot_naive(const double* x, const double* y, size_t n)
{
    return dot_generic(x, y, n, mul_naive, sum_naive);
}

double dot_kahan(const double* x, const double* y, size_t n)
{
    return dot_generic(x, y, n, mul_naive, sum_kahan);
}

double dot_ogita_oishi(const double* x, const double* y, size_t n) {
    return dot_generic(x, y, n, mul_naive, sum_ogita_oishi);
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