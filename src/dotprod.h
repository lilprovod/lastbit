#ifndef DOTPROD_H
#define DOTPROD_H

#include <stddef.h>

typedef struct {
    double naive;
    double kahan;
    double kbn2;
    double kbn3;
    double ogita_oishi;
    double reference;
} DotResults;

/** @brief Наивное скалярное произведение */
double dot_naive(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой Кэхэна */
double dot_kahan(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой КБН 2-го порядка */
double dot_kbn2(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой КБН 3-го порядка */
double dot_kbn3(const double* x, const double* y, size_t n);

/** @brief Скалярное произведение с суммой через алгоритм Ogita-Oishi */
double dot_ogita_oishi(const double* x, const double* y, size_t n);

/** @brief (Эталон*) Скалярное произведение с суммой GMP и произведением FMA */
double dot_reference(const double* x, const double* y, size_t n);


/** @brief Рассчитывает скалярное произведение всеми алгоритмами */
DotResults compute_all_dot(const double* x, const double* y, size_t n);

#endif