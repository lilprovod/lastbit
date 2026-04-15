#ifndef SUM_H
#define SUM_H

#include <stddef.h>

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

#endif