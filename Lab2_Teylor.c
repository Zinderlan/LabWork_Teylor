#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#define _USE_MATH_DEFINES
#include "math.h"

// структура для ввода значений
typedef struct start_values{
    float x;                                      // число, которое будет в аргументе функции
    int final_coef;                          // до какого члена надо раскладывать ряд тейлора
    float term0;                                  // первый член в ряде
    float (*func_comp)(float x, int n);           // укащатель на функцию, с которой будем работать
} START_VAL;

// структура для выведения результата
typedef struct res {
    float res;                                    // сумма, которая получится
    float term_now;                               //последний просуммированный член
} RESULT;

// рекурентный множитель для sin
float func_sin(float x, int n) {
    return -(x * x) / ((2 * n + 2) * (2 * n + 3));
}

// рекурентный множитель для cos
float func_cos(float x, int n) {
    return -(x * x) / ((2 * n + 1) * (2 * n + 2));
}

// рекурентный множитель для exp
float func_exp(float x, int n) {
    return x / (n + 1);
}

// рекурентный множитель для ln
float func_ln(float z, int n) {
    return ((z*z)*(2*n + 1))/(2*n+3);
}

// рекурсивная функция суммирования
float pairwise_sum_recursive(float *arr, int left, int right) 
{
    if (left == right) return arr[left];
    if (left + 1 == right) return arr[left] + arr[right];

    int middle = (left + right) / 2;
    float left_pair_sum = pairwise_sum_recursive(arr, left, middle);
    float right_pair_sum = pairwise_sum_recursive(arr, middle + 1, right);

    return left_pair_sum + right_pair_sum;
}

// прямая сумма
RESULT direct_sum(START_VAL start_val) {
    RESULT Res = {0.0, 0.0};
    float term = start_val.term0;
    int n = 0;

    while (n < start_val.final_coef)
    {
        Res.res += term;
        term *= start_val.func_comp(start_val.x, n);  
        n++;
    }

    Res.term_now = term / start_val.func_comp(start_val.x, n);
    return Res;
 }

//обратная сумма
RESULT back_sum(START_VAL start_val) {
    RESULT Res = {0.0, 0.0};
    float term = start_val.term0;
    int n = 0;
    float* arr_term = (float*)(malloc(sizeof(float) * start_val.final_coef));
    if (arr_term == NULL) {
        printf("Error memory is empty\n");
        return Res;
    }

    while (n < start_val.final_coef) 
    {
        arr_term[n] = term;
        term *= start_val.func_comp(start_val.x, n);
        n++;
    }

    for (int i = n-1; i >=0; i--) 
    {
        Res.res += arr_term[i];
    }

    Res.term_now = arr_term[n-1];
    free(arr_term);

    return Res;
}

// олимпийское суммирование
RESULT pairwise_sum(START_VAL start_val)
{
    RESULT Res = { 0.0, 0.0 };
    float term = start_val.term0;
    int n = 0;
    float* arr_term = (float*)(malloc(sizeof(float) * start_val.final_coef));
    if (arr_term == NULL) {
        printf("Error memory is empty\n");
        return Res;
    }

    while (n < start_val.final_coef)
    {
        arr_term[n] = term;
        term *= start_val.func_comp(start_val.x, n);
        n++;
    }

    Res.res = pairwise_sum_recursive(arr_term, 0, start_val.final_coef - 1);
    free(arr_term);
    
    return Res;
}


int main()
{
    float x; // аргумент x
    int final_coef; // до какого члена в ряде тейлора раскладывать
    int choise_func;

    printf("Enter the value of x: ");
    scanf_s("%f", &x);
    printf("\n");

    printf("To what coefficient expanded(>1000): ");
    scanf_s("%d", &final_coef);
    printf("\n");

    printf("What function?\n");
    printf("0 - sin\n");
    printf("1 - cos\n");
    printf("2 - exp\n");
    printf("3 - ln\n");
    printf("4 - all\n");
    scanf_s("%d", &choise_func);

    printf("\n");

    if (final_coef < 1000) final_coef = 1000;

    // приведение x к радианам
    float two_pi = 2.0f * (float)M_PI;
    float x_rad = fmodf(x, two_pi);
    if (x_rad < 0.0f) {
        x_rad += two_pi;
    }

    float sin_m = sinf(x);
    float cos_m = cosf(x);
    float exp_m = expf(x);
    float log_m = logf(x);

    float z = (x - 1.0) / (x + 1.0);
    START_VAL start_val_ln = { z, 10, 2 * z, func_ln };

    if (choise_func == 0)
    {
        START_VAL start_val_sin = { x_rad, final_coef, x_rad, func_sin };
        
        RESULT direct_sin = direct_sum(start_val_sin);
        RESULT back_sin = back_sum(start_val_sin);
        RESULT pairwise_sin = pairwise_sum(start_val_sin);

        printf("Sin(x): \n");
        printf("Direct: %.8f\n", direct_sin.res);
        printf("Back: %.8f\n", back_sin.res);
        printf("Pairwise: %.8f\n\n", pairwise_sin.res);

        printf("Sin math: %.8f\n", sin_m);
    }
    else if (choise_func == 1) 
    {
        START_VAL start_val_cos = { x_rad, final_coef, 1, func_cos };
        
        RESULT direct_cos = direct_sum(start_val_cos);
        RESULT back_cos = back_sum(start_val_cos);
        RESULT pairwise_cos = pairwise_sum(start_val_cos);

        printf("Cos(x): \n");
        printf("Direct: %.8f\n", direct_cos.res);
        printf("Back: %.8f\n", back_cos.res);
        printf("Pairwise: %.8f\n\n", pairwise_cos.res);

        printf("Cos math: %.8f\n", cos_m);
    }
    else if (choise_func == 2) 
    {
        START_VAL start_val_exp = { x, final_coef, 1, func_exp };

        RESULT direct_exp = direct_sum(start_val_exp);
        RESULT back_exp = back_sum(start_val_exp);
        RESULT pairwise_exp = pairwise_sum(start_val_exp);

        printf("Exp(x): \n");
        printf("Direct: %.8f\n", direct_exp.res);
        printf("Back: %.8f\n", back_exp.res);
        printf("Pairwise: %.8f\n\n", pairwise_exp.res);

        printf("Exp math: %.8f\n", exp_m);

    }
    else if (choise_func == 3) 
    {
        float z = (x - 1.0) / (x + 1.0);
        START_VAL start_val_ln = { z, 10, 2 * z, func_ln };

        RESULT direct_ln = direct_sum(start_val_ln);
        RESULT back_ln = back_sum(start_val_ln);
        RESULT pairwise_ln = pairwise_sum(start_val_ln);

        printf("Ln(x): \n");
        printf("Direct: %.8f\n", direct_ln.res);
        printf("Back: %.8f\n", back_ln.res);
        printf("Pairwise: %.8f\n\n", pairwise_ln.res);

        printf("Ln math: %.8f\n", log_m);
    }
    else 
    {
        START_VAL start_val_sin = { x_rad, final_coef, x_rad, func_sin };
        START_VAL start_val_cos = { x_rad, final_coef, 1, func_cos };
        START_VAL start_val_exp = { x, final_coef, 1, func_exp };
        float z = (x - 1.0) / (x + 1.0);
        START_VAL start_val_ln = { z, final_coef, 2 * z, func_ln };

        RESULT direct_sin = direct_sum(start_val_sin);
        RESULT direct_cos = direct_sum(start_val_cos);
        RESULT direct_exp = direct_sum(start_val_exp);
        RESULT direct_ln = direct_sum(start_val_ln);

        RESULT back_sin = back_sum(start_val_sin);
        RESULT back_cos = back_sum(start_val_cos);
        RESULT back_exp = back_sum(start_val_exp);
        RESULT back_ln = back_sum(start_val_ln);

        RESULT pairwise_sin = pairwise_sum(start_val_sin);
        RESULT pairwise_cos = pairwise_sum(start_val_cos);
        RESULT pairwise_exp = pairwise_sum(start_val_exp);
        RESULT pairwise_ln = pairwise_sum(start_val_ln);

        printf("Sin(x): \n");
        printf("Direct: %.8f\n", direct_sin.res);
        printf("Back: %.8f\n", back_sin.res);
        printf("Pairwise: %.8f\n", pairwise_sin.res);
        printf("Sin math: %.8f\n\n", sin_m);

        printf("Cos(x): \n");
        printf("Direct: %.8f\n", direct_cos.res);
        printf("Back: %.8f\n", back_cos.res);
        printf("Pairwise: %.8f\n", pairwise_cos.res);
        printf("Cos math: %.8f\n\n", cos_m);

        printf("Exp(x): \n");
        printf("Direct: %.8f\n", direct_exp.res);
        printf("Back: %.8f\n", back_exp.res);
        printf("Pairwise: %.8f\n", pairwise_exp.res);
        printf("Exp math: %.8f\n\n", exp_m);

        printf("Ln(x): \n");
        printf("Direct: %.8f\n", direct_ln.res);
        printf("Back: %.8f\n", back_ln.res);
        printf("Pairwise: %.8f\n", pairwise_ln.res);
        printf("Ln math: %.8f\n\n", log_m);
    }

}