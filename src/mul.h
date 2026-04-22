#ifndef MUL_H
#define MUL_H

/** @brief Наивное умножение двух чисел */
double mul_naive(const double x, const double y);

/**
 * @brief Точное произведение двух чисел через FMA
 * 
 * @param[in] x Число А
 * @param[in] y Число Б
 * @param[out] hi Округленная часть умножения
 * @param[out] lo Ошибка округления
 * 
 * Результат точного умножения `x` * `y` = `hi` + `lo`
*/
void two_product_fma(const double x, const double y, double* hi, double* lo);

#endif