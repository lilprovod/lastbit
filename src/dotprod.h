#ifndef DOTPROD_H
#define DOTPROD_H

#include <stddef.h>

/**
 * @brief Наивное скалярное произведение равноразмерных векторов
 * 
 * @param[in] x Вектор А
 * @param[in] y Вектор Б
 * @param[in] n Размер векторов
 * 
 * @return Результат скалярного произведения векторов А и Б
 */
double dot_naive(const double* x, const double* y, size_t n);

/**
 * @brief Скалярное произведение с суммой Кэхэна равноразмерных векторов
 * 
 * @param[in] x Вектор А
 * @param[in] y Вектор Б
 * @param[in] n Размер векторов
 * 
 * @return Результат скалярного произведения векторов А и Б
 */
double dot_kahan(const double* x, const double* y, size_t n);

/**
 * @brief Скалярное произведение с произведением FMA
 * 
 * @param[in] x Вектор А
 * @param[in] y Вектор Б
 * @param[in] n Размер векторов
 * 
 * @return Результат скалярного произведения векторов А и Б
 */
double dot_fma(const double* x, const double* y, size_t n);

#endif