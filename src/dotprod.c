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

double dot_kbn2(const double* x, const double* y, size_t n)
{
    return dot_generic(x, y, n, mul_naive, sum_kbn2);
}

double dot_kbn3(const double* x, const double* y, size_t n)
{
    return dot_generic(x, y, n, mul_naive, sum_kbn3);
}

double dot_ogita_oishi(const double* x, const double* y, size_t n) {
    return dot_generic(x, y, n, mul_naive, sum_ogita_oishi);
}

/** @brief Общий интерфейс для скалярного произведения через FMA */
static double dot_fma_generic(
    const double* x,
    const double* y,
    size_t        n,
    double        (*sum)(const double*, size_t)
)
{
    double* all_terms = malloc(2*n * sizeof(double));
    if (!all_terms) { return 0.0; }

    for (size_t i = 0; i < n; i++) {
        mul_fma(x[i], y[i], &all_terms[2*i], &all_terms[2*i + 1]);
    }

    double total = sum(all_terms, 2*n);

    free(all_terms);

    return total;
}

double dot_fma(const double* x, const double* y, size_t n)
{
    return dot_fma_generic(x, y, n, sum_kahan);
}

double dot_reference(const double* x, const double* y, size_t n)
{
    return dot_fma_generic(x, y, n, sum_gmp);
}