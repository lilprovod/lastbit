#ifndef SUM_H
#define SUM_H

#include <stddef.h>

/** @brief Наивное сложение двух чисел */
double sum_naive(const double x, const double y);

/** @brief Сложение набора чисел с помощью алгоритма Кэхэна */
double sum_kahan(const double* values, size_t n);

#endif