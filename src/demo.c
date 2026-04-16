#include <stdio.h>

#include "demo.h"
#include "dotprod.h"
#include "io.h"
#include "mul.h"
#include "sum.h"

static void run_demo_case(
    const char*     title,
    const char*     description,
    const double*   x,
    const double*   y,
    size_t          n
)
{
    printf("\n[DEMO CASE] %s", title);
    printf("\t- %s\n", description);

    InputData data = {.x = (double*)x, .y = (double*)y, .n = n};

    show_input(&data);

    printf("\n\n");

    DotResults results = compute_all_dot(x, y, n);

    show_table_dot(&results);

    printf("\n");
}

/** @brief Пример, где наивное скалярное произведение ошибается */
static void naive_cancellation()
{
    static const int size = 3;
    static const double vec_x[] = {1e16, 1.0, -1e16};
    static const double vec_y[] = {1.0,  1.0,  1.0 };

    run_demo_case(
        "Naive cancellation",
        "Small value is lost between two large opposite terms",
        vec_x,
        vec_y,
        size
    );
}

/** @brief Пример, где ошибка в скалярном произведении накапливается */
static void error_accumulation()
{
    static const int size = 5;
    static const double vec_x[] = {1e16, 1.0, 1.0, 1.0, -1e16};
    static const double vec_y[] = {1.0,  1.0, 1.0, 1.0,  1.0 };

    run_demo_case(
        "Accumulation of small errors",
        "A few small values accumulate near large ones",
        vec_x,
        vec_y,
        size
    );
}

/**
 * @brief Пример, где важен порядок чисел внутри векторов скалярного
 * произведения
 */
static void order_sensitivity()
{
    static const int size = 4;
    static const double vec_x[] = {1e16, 1.0, -1e16, 1.0};
    static const double vec_y[] = {1.0,  1.0, 1.0,   1.0};

    run_demo_case(
        "Order sensitivity",
        "Result depends on summation order",
        vec_x,
        vec_y,
        size
    );
}


/** 
 * @brief Пример, где наивное суммирование дает неверный ответ,
 * а остальные - верный.
 */
static void s_example_1()
{
    static const int size = 3;
    static const double values[] = {1e16, 1.0, 1.0};

    printf("\n[SPECIAL EXAMPLE] Naive summation is bad.\n\n");
    printf("\tExample:\t1e16 + 1.0 + 1.0 = 1e16 + 2.0\n");

    printf("\n\n");

    SumResults results = compute_all_sum(values, size);
    show_table_sum(&results);

    printf("\n");
}

/**
 * @brief Пример, где алгоритм Кэхэна делает ошибка, а более точные методы
 * дают правильный ответ.
 */
static void s_example_2()
{
    static const int size = 5;
    static const double values[] = {1e16, 1.0, 1.0, 1.0, -1e16};

    printf("\n[SPECIAL EXAMPLE] Kahan is bad.\n\n");
    printf("\tExample:\t1e16 + 1.0 + 1.0 + 1.0 - 1e16 = 3.0\n");

    printf("\n\n");

    SumResults results = compute_all_sum(values, size);
    show_table_sum(&results);

    printf("\n");
}

/**
 * @brief Пример, где наивное умножение дает неверный ответ,
 * а остальные алгоритмы верный.
 */
static void s_example_3()
{
    static const double x = 1.0000000000000002;
    static const double y = 1.0000000000000002;

    const double naive = mul_naive(x, y);

    static double hi, lo;
    mul_fma(x, y, &hi, &lo);

    printf("\n[SPECIAL EXAMPLE] Naive multiplication is bad.\n\n");
    printf("\tExample:\t1.0000000000000002 * 1.0000000000000002 =\n"
           "\t        \t1.00000000000000040000000000000004 = 1 + 4e-16 + 4e-32\n");

    printf("\n\n");

    printf("%-12s | %-24s\n", "Method", "Result");
    printf("-------------+------------------------\n");
    printf("%-12s | %.17g\n", "naive", naive);
    printf("%-12s | %.17g + %.17g\n", "fma", hi, lo);

    printf("\n");
}

void run_demo()
{
    printf("\n[Demo cases for dot product algorithms]\n");

    naive_cancellation();
    error_accumulation();
    order_sensitivity();

    printf("\n[Special examples for summation and multiplication]\n");

    s_example_1();
    s_example_2();
    s_example_3();
}