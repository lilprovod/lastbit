#include <math.h>
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

static double vector_max_abs(const double* vec, size_t n) {
    double max = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (fabs(vec[i]) > max) max = fabs(vec[i]);
    }

    return max;
}

static int ceil_log2(size_t n) {
    if (n <= 1) return 0;

    int log = 0;
    size_t value = n - 1;

    while (value > 0) {
        value >>= 1;
        log++;
    }

    return log;
}

static void split_vector_ozaki(const double* x, size_t n, size_t K, double* layers)
{
    if (n == 0 || K == 0) return;
    
    const int p_s = (IEEE_754_MANTISSA_BITS - ceil_log2(n) - 1) / 2;

    double* rem = malloc(n * sizeof(double));
    if (!rem) { return; }

    for (size_t i = 0; i < n; i++) {
        rem[i] = x[i];
    }

    for (size_t k = 0; k < K - 1; k++) {
        double mu = vector_max_abs(rem, n);
        
        // Ранний выход при остатке в векторе одних нулей
        if (mu == 0.0) {
            for (size_t kk = k; kk < K; kk++) {
                for (size_t i = 0; i < n; i++) {
                    layers[kk*n + i] = 0.0;
                }
            }

            free(rem);
            return;
        }
        
        int E;
        frexp(mu, &E);
        
        double sigma = 1.5 * ldexp(1.0, E + IEEE_754_MANTISSA_BITS - 1 - p_s);

        for (size_t i = 0; i < n; i++) {
            double xk_i = (rem[i] + sigma) - sigma;

            layers[k*n + i] = xk_i;

            rem[i] -= xk_i;
        }
    }

    for (size_t i = 0; i < n; i++) {
        layers[(K - 1)*n + i] = rem[i];
    }

    free(rem);
}

double dot_ozaki(const double* x, const double* y, size_t n, const OzakiConfig* ozaki)
{
    size_t K = ozaki->ozaki_layers;

    double* x_layers = malloc(K*n * sizeof(double));
    double* y_layers = malloc(K*n * sizeof(double));

    if (!x_layers || !y_layers) {free(x_layers); free(y_layers); return 0.0;}

    split_vector_ozaki(x, n, K, x_layers);
    split_vector_ozaki(y, n, K, y_layers);

    double* partials = malloc( (K*K) * sizeof(double) );
    if (!partials) { free(x_layers); free(y_layers); return 0.0; }

    size_t idx = 0;
    for (size_t i = 0; i < K; i++) {
        for (size_t j = 0; j < K; j++) {

            double sum = 0.0;
            for (size_t k = 0; k < n; k++) {
                sum += x_layers[i * n + k] * y_layers[j * n + k];
            }

            partials[idx++] = sum;
        }
    }

    double total = sum_kbn3(partials, idx);

    free(x_layers);
    free(y_layers);
    free(partials);

    return total;
}


DotResults compute_all_dot(const double* x, const double* y, size_t n, const OzakiConfig* ozaki)
{
    DotResults results = {0};

    results.naive       = dot_naive(x, y, n);
    results.kahan       = dot_kahan(x, y, n);
    results.kbn2        = dot_kbn2(x, y, n);
    results.kbn3        = dot_kbn3(x, y, n);
    results.ogita_oishi = dot_ogita_oishi(x, y, n);
    results.ozaki       = dot_ozaki(x, y, n, ozaki);
    results.reference   = dot_reference(x, y, n);

    return results;
}