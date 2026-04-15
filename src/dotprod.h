#ifndef DOTPROD_H
#define DOTPROD_H

#include <stddef.h>

/** @brief Наивное скалярное произведение равноразмерных векторов */
double dot_naive(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой Кэхэна равноразмерных векторов */
double dot_kahan(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с произведением FMA */
double dot_fma(const double* x, const double* y, size_t n);

#endif