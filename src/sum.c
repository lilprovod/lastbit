#include <gmp.h>
#include <math.h>

#include "sum.h"

#define GMP_PRECISION 256

double sum_naive(const double* values, size_t n)
{
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        sum += values[i];
    }

    return sum;
}

double sum_kahan(const double* values, size_t n)
{
    double sum = 0.0;
    double err = 0.0;

    for (size_t i = 0; i < n; i++) {
        double corrected = values[i] - err;
        double temp = sum + corrected;

        err = (temp - sum) - corrected;
        sum = temp;
    }

    return sum;
}

/** @brief Error-free transformation двух чисел с плавающей точкой. */
static void two_sum_knuth(double a, double b, double* x, double* y)
{
    *x = a + b;

    double z = *x - a;
    *y = (a - (*x - z)) + (b - z);
}

double sum_kbn2(const double* values, size_t n)
{
    double sum  = 0.0;
    double err1 = 0.0;
    double err2 = 0.0;

    for (size_t i = 0; i < n; i++) {
        double temp, delta0;
        two_sum_knuth(sum, values[i], &temp, &delta0);

        sum = temp;

        double new_err1, delta1;
        two_sum_knuth(err1, delta0, &new_err1, &delta1);

        err1 = new_err1;
        err2 += delta1;
    }

    double total = (err2 + err1) + sum;

    return total;
}

double sum_kbn3(const double* values, size_t n)
{
    double sum  = 0.0;
    double err1 = 0.0;
    double err2 = 0.0;
    double err3 = 0.0;

    for (size_t i = 0; i < n; i++) {
        double temp, delta0;
        two_sum_knuth(sum, values[i], &temp, &delta0);

        sum = temp;

        double new_err1, delta1;
        two_sum_knuth(err1, delta0, &new_err1, &delta1);
        
        err1 = new_err1;
        
        double new_err2, delta2;
        two_sum_knuth(err2, delta1, &new_err2, &delta2);

        err2 = new_err2;
        err3 += delta2;
    }

    double total = ((err3 + err2) + err1) + sum;

    return total;
}

double sum_ogita_oishi(const double* values, size_t n)
{
    if (n == 0) return 0.0;

    double pi_acc = values[0];
    double sigma  = 0.0;

    for (size_t i = 1; i < n; i++) {
        double q;

        two_sum_knuth(pi_acc, values[i], &pi_acc, &q);
        sigma += q;
    }

    double total = pi_acc + sigma;

    return total;
}

double sum_gmp(const double* values, size_t n)
{
    mpf_t sum;
    mpf_t term;

    mpf_init2(sum, GMP_PRECISION);
    mpf_init2(term, GMP_PRECISION);

    for (size_t i = 0; i < n; i++) {
        mpf_set_d(term, values[i]);
        mpf_add(sum, sum, term);
    }

    double total = mpf_get_d(sum);

    mpf_clear(sum);
    mpf_clear(term);

    return total;
}


SumResults compute_all_sum(const double* values, size_t n)
{
    SumResults results = {0};

    results.naive       = sum_naive(values, n);
    results.kahan       = sum_kahan(values, n);
    results.kbn2        = sum_kbn2(values, n);
    results.kbn3        = sum_kbn3(values, n);
    results.ogita_oishi = sum_ogita_oishi(values, n);
    results.gmp         = sum_gmp(values, n);

    return results;
}