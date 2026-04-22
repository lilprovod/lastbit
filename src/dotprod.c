#include <stdlib.h>

#include "dotprod.h"
#include "mul.h"
#include "sum.h"

/**
 * @brief Общий интерфейс для скалярного произведения с применением
 * определённого алгоритма суммирования.
 * 
 * Применение EFT (Error-Free Transformation) для произведения
 * через TwoProductFMA: `a * b` = `p + e`, где `p` - округленное значение,
 * `e` - точная ошибка.
 */
static double dot_eft_generic(
    const double* x,
    const double* y,
    size_t        n,
    double        (*sum)(const double*, size_t)
)
{
    double* terms = malloc(2*n * sizeof(double));
    if (!terms) { return 0.0; }

    for (size_t i = 0; i < n; i++) {
        two_product_fma(x[i], y[i], &terms[n + i], &terms[i]);
    }

    double total = sum(terms, 2*n);

    free(terms);

    return total;
}

double dot_naive(const double* x, const double* y, size_t n)
{
    double* terms = malloc(n * sizeof(double));
    if (!terms) { return 0.0; }

    for (size_t i = 0; i < n; i++) {
        terms[i] = mul_naive(x[i], y[i]);
    }

    double total = sum_naive(terms, n);

    free(terms);

    return total;
}

double dot_kahan(const double* x, const double* y, size_t n)
{
    return dot_eft_generic(x, y, n, sum_kahan);
}

double dot_kbn2(const double* x, const double* y, size_t n)
{
    return dot_eft_generic(x, y, n, sum_kbn2);
}

double dot_kbn3(const double* x, const double* y, size_t n)
{
    return dot_eft_generic(x, y, n, sum_kbn3);
}

double dot_ogita_oishi(const double* x, const double* y, size_t n) {
    return dot_eft_generic(x, y, n, sum_ogita_oishi);
}

double dot_reference(const double* x, const double* y, size_t n)
{
    return dot_eft_generic(x, y, n, sum_gmp);
}


DotResults compute_all_dot(const double* x, const double* y, size_t n)
{
    DotResults results = {0};

    results.naive       = dot_naive(x, y, n);
    results.kahan       = dot_kahan(x, y, n);
    results.kbn2        = dot_kbn2(x, y, n);
    results.kbn3        = dot_kbn3(x, y, n);
    results.ogita_oishi = dot_ogita_oishi(x, y, n);
    results.reference   = dot_reference(x, y, n);

    return results;
}