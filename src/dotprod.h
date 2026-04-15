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


/** @brief Наивное сложение двух чисел */
double sum_naive(const double x, const double y);

/** @brief Наивное сложение набора чисел с помощью алгоритма Кэхэна */
double sum_kahan(const double* values, size_t n);


/** @brief Наивное умножение двух чисел */
double mul_naive(const double x, const double y);

/**
 * @brief Точное умножение двух чисел через FMA
 * 
 * @param[in] x Число А
 * @param[in] y Число Б
 * @param[out] hi Округленная часть умножения
 * @param[out] lo Ошибка округления
 * 
 * Результат точного умножения `x` * `y` = `hi` + `lo`
*/
void mul_fma(const double x, const double y, double* hi, double* lo);

#endif