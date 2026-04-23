#include <math.h>
#include <stdio.h>

#include "demo.h"
#include "dotprod.h"
#include "io.h"
#include "mul.h"
#include "sum.h"

static char mark_exact(double value, double exact)
{
    const double eps = 1e-9;
    return (fabs(value - exact) < eps) ? 'v' : 'x';
}

static void show_demo_comparison(const DotResults* r, const OzakiConfig* ozaki, double exact)
{
    char ozaki_name[32];
    snprintf(ozaki_name, sizeof(ozaki_name), "ozaki (k=%zu)", ozaki->ozaki_layers);

    printf("%-12s | %-24s | %-6s\n", "Method", "Result", "Is exact?"    );
    printf("-------------+---------------------------------\n"          );

    printf("%-12s | %.17g | %c\n", "naive", r->naive, mark_exact(r->naive, exact));
    printf("%-12s | %.17g | %c\n", "kahan", r->kahan, mark_exact(r->kahan, exact));
    printf("%-12s | %.17g | %c\n", "kbn2", r->kbn2, mark_exact(r->kbn2, exact));
    printf("%-12s | %.17g | %c\n", "kbn3", r->kbn3, mark_exact(r->kbn3, exact));
    printf("%-12s | %.17g | %c\n", ozaki_name, r->ozaki, mark_exact(r->ozaki, exact));
    printf("%-12s | %.17g | %c\n", "ogita-oishi", r->ogita_oishi, mark_exact(r->ogita_oishi, exact));
    printf("%-12s | %.17g | %c\n", "reference*", r->reference, mark_exact(r->reference, exact));
}

static void run_demo_case(
    const char*     title,
    const char*     description,
    const double*   x,
    const double*   y,
    size_t          n,
    OzakiConfig*    ozaki,
    const double    exact_value
)
{
    printf("\n[DEMO CASE] %s", title);
    printf("\t- %s\n", description);

    InputData data = {.x = (double*)x, .y = (double*)y, .n = n};

    show_input(&data);

    printf("\nExact value: %.17g", exact_value);

    printf("\n\n");

    DotResults results = compute_all_dot(x, y, n, ozaki);

    show_demo_comparison(&results, ozaki, exact_value);

    printf("\n");
}

/** @brief Пример с нулевым вектором */
static void zero_vector(OzakiConfig* ozaki)
{
    static const size_t size = 3;
    static const double vec_x[] = {0.0, 0.0, 0.0};
    static const double vec_y[] = {0.0, 5.7, -1e16};

    static const double exact = 0.0;

    run_demo_case(
        "Zero vector",
        "All results of the dot product must be zero",
        vec_x,
        vec_y,
        size,
        ozaki,
        exact
    );
}

/** @brief Простой пример на обычных данных */
static void simple_case(OzakiConfig* ozaki)
{
    static const size_t size = 3;
    static const double vec_x[] = {1.0, 2.0, 3.0};
    static const double vec_y[] = {3.0, 2.0, 1.0};

    static const double exact = 10.0;

    run_demo_case(
        "Simple case",
        "A small case with simple values",
        vec_x,
        vec_y,
        size,
        ozaki,
        exact
    );
}

/** @brief Пример, где наивное скалярное произведение ошибается */
static void naive_cancellation(OzakiConfig* ozaki)
{
    static const size_t size = 3;
    static const double vec_x[] = {1e16, 1.0, -1e16};
    static const double vec_y[] = {1.0,  1.0,  1.0 };

    static const double exact = 1.0;

    run_demo_case(
        "Naive cancellation",
        "Small value is lost between two large opposite terms",
        vec_x,
        vec_y,
        size,
        ozaki,
        exact
    );
}

/** @brief Пример, где ошибка в скалярном произведении накапливается */
static void error_accumulation(OzakiConfig* ozaki)
{
    static const size_t size = 5;
    static const double vec_x[] = {1e16, 1.0, 1.0, 1.0, -1e16};
    static const double vec_y[] = {1.0,  1.0, 1.0, 1.0,  1.0 };

    static const double exact = 3.0;

    run_demo_case(
        "Accumulation of small errors",
        "A few small values accumulate near large ones",
        vec_x,
        vec_y,
        size,
        ozaki,
        exact
    );
}

/**
 * @brief Пример, где важен порядок чисел внутри векторов скалярного
 * произведения
 */
static void order_sensitivity(OzakiConfig* ozaki)
{
    static const size_t size = 4;
    static const double vec_x[] = {1e16, 1.0, -1e16, 1.0};
    static const double vec_y[] = {1.0,  1.0, 1.0,   1.0};

    static const double exact = 2.0;

    run_demo_case(
        "Order sensitivity",
        "Result depends on summation order",
        vec_x,
        vec_y,
        size,
        ozaki,
        exact
    );
}


/** 
 * @brief Пример, где наивное суммирование дает неверный ответ,
 * а остальные - верный.
 */
static void s_example_1()
{
    static const size_t size = 3;
    static const double values[] = {1e16, 1.0, 1.0};

    printf("\n[SPECIAL EXAMPLE] Naive summation failure\n\n");
    printf("\tExample: 1e16 + 1.0 + 1.0\n");
    printf("\tExact value: 10000000000000002 (1e16 + 2.0)\n");

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
    static const size_t size = 5;
    static const double values[] = {1e16, 1.0, 1.0, 1.0, -1e16};

    printf("\n[SPECIAL EXAMPLE] Kahan failure example\n\n");
    printf("\tExample: 1e16 + 1.0 + 1.0 + 1.0 - 1e16\n");
    printf("\tExact value: 3.0\n");

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
    two_product_fma(x, y, &hi, &lo);

    printf("\n[SPECIAL EXAMPLE] Naive multiplication failure\n\n");
    printf("\tExample: 1.0000000000000002 * 1.0000000000000002\n");
    printf("\tExact value: 1.00000000000000040000000000000004 (1 + 4e-16 + 4e-32)\n");

    printf("\n\n");

    printf("%-12s | %-24s\n", "Method", "Result");
    printf("-------------+------------------------\n");
    printf("%-12s | %.17g\n", "naive", naive);
    printf("%-12s | %.17g + %.17g\n", "2ProductFMA", hi, lo);

    printf("\n");
}

/**
 * @brief Отдельный пример для алгоритма Ozaki
 */
static void s_example_4(OzakiConfig* ozaki)
{
    static const size_t size = 2;

    const double vec_x[] = {pow(2.0, 27) + 1.0, 1.0};
    const double vec_y[] = {pow(2.0, 27) - 1.0, -1.0};

    double naive = dot_naive(vec_x, vec_y, size);
    double o     = dot_ozaki(vec_x, vec_y, size, ozaki);
    double ref   = dot_reference(vec_x, vec_y, size);

    printf("\n[SPECIAL EXAMPLE] Ozaki validation example\n\n");
    printf("\tExample: (2^27 + 1, 1) * (2^27 - 1, -1)\n");
    printf("\tExact value: 18014398509481982 (2^54 - 2)\n");

    printf("\n\n");


    char ozaki_name[32];
    snprintf(ozaki_name, sizeof(ozaki_name), "ozaki (k=%zu)", ozaki->ozaki_layers);

    printf("%-12s | %-24s\n", "Method", "Result"     );
    printf("-------------+------------------------\n");

    printf("%-12s | %.17g\n", "naive",      naive);
    printf("%-12s | %.17g\n", ozaki_name,   o    );
    printf("%-12s | %.17g\n", "reference*", ref  );

    printf("\n");
}

void run_demo(OzakiConfig* ozaki)
{
    printf("\n-=== [DEMO CASES FOR DOT PRODUCT ALGORITHMS] ===-\n");

    zero_vector(ozaki);
    simple_case(ozaki);
    naive_cancellation(ozaki);
    error_accumulation(ozaki);
    order_sensitivity(ozaki);

    printf("\n-=== [SPECIAL EXAMPLES FOR SUMMATION AND MULTIPLICATION] ===-\n");

    s_example_1();
    s_example_2();
    s_example_3();
    s_example_4(ozaki);
}