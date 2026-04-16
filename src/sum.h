#ifndef SUM_H
#define SUM_H

#include <stddef.h>

typedef struct {
    double naive;
    double kahan;
    double kbn2;
    double kbn3;
    double ogita_oishi;
    double gmp;
} SumResults;

/** @brief Наивное сложение набора чисел */
double sum_naive(const double* values, size_t n);

/** @brief Сложение набора чисел с помощью алгоритма Кэхэна */
double sum_kahan(const double* values, size_t n);

/** @brief Сложение набора чисел с помощью KBN 2-го порядка */
double sum_kbn2(const double* values, size_t n);

/** @brief Сложение набора чисел с помощью KBN 3-го порядка */
double sum_kbn3(const double* values, size_t n);

/** @brief Сложение набора чисел с помощью алгоритма Ogita-Oishi */
double sum_ogita_oishi(const double* values, size_t n);

/** @brief Сложение набора чисел с помощью длинной арифметики GMP */
double sum_gmp(const double* values, size_t n);


/** @brief Рассчитывает сумму набора чисел разными методами */
SumResults compute_all_sum(const double* values, size_t n);

#endif