#ifndef DOTPROD_H
#define DOTPROD_H

#include <stddef.h>

/** @brief Наивное скалярное произведение */
double dot_naive(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой Кэхэна */
double dot_kahan(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой КБН 2-го порядка */
double dot_kbn2(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой КБН 3-го порядка */
double dot_kbn3(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с произведением FMA */
double dot_fma(const double* x, const double* y, size_t n);

#endif