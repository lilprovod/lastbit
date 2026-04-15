#include <math.h>
#include "sum.h"

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

/*
Разложение суммы (`a` + `b`) = (`s` + `e`), где `s` - результат с округлением,
а `e` - потерянная часть суммы
*/
static void two_sum(double a, double b, double* s, double* e)
{
    *s = a + b;
    
    if (fabs(a) >= fabs(b)) {
        *e = (a - *s) + b;
    } else {
        *e = (b - *s) + a;
    }
}

double sum_kbn2(const double* values, size_t n)
{
    double sum  = 0.0;
    double err1 = 0.0;
    double err2 = 0.0;

    for (size_t i = 0; i < n; i++) {
        double temp, delta0;
        two_sum(sum, values[i], &temp, &delta0);

        sum = temp;

        double new_err1, delta1;
        two_sum(err1, delta0, &new_err1, &delta1);

        err1 = new_err1;
        err2 += delta1;
    }

    double total = sum + err1 + err2;

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
        two_sum(sum, values[i], &temp, &delta0);

        sum = temp;

        double new_err1, delta1;
        two_sum(err1, delta0, &new_err1, &delta1);
        
        err1 = new_err1;
        
        double new_err2, delta2;
        two_sum(err2, delta1, &new_err2, &delta2);

        err2 = new_err2;
        err3 += delta2;
    }

    double total = sum + err1 + err2 + err3;

    return total;
}

double sum_ogita_oishi(const double* values, size_t n)
{
    double sum = 0.0;
    double err = 0.0;

    for (size_t i = 0; i < n; i++) {
        double temp  = sum + values[i];
        double delta = (sum - temp) + values[i];

        sum = temp;
        err += delta;
    }

    return sum + err;
}