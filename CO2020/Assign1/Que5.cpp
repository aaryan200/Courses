/*
C++ Program to solve the  differential equation
dx/dt = f(t, x) =  (tx^2 - x)/t
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

#define h 0.005 // Step size

/* Returns value of function f(t, x) */
double f(double t, double x)
{
    double out = x * x - x / t;
    return out;
}

/* Exact solution in case of x(1) = -1/ln(2) */
double exactSolution(double t)
{
    double out = -1.0 / (t * log(2.0 * t));
    return out;
}

/* RK-2 Method to approximate solution of IVP at final_time */
double RK_2(double initial_value, double initial_time, double final_time, FILE *out_file)
{
    double w_prev = initial_value;
    double t_prev = initial_time;
    double w_new, y_new;
    double t = t_prev + h;

    fprintf(out_file, "Time Exact Value Approximate Value Error\n");
    fprintf(out_file, "%.5f %.8f %.8f %.9f\n", initial_time, exactSolution(t_prev), initial_value, abs((w_prev - w_prev) * 100 / w_prev));
    while (t < final_time)
    {
        double k1 = h * f(t_prev, w_prev);
        double k2 = h * f(t_prev + h, w_prev + k1);

        w_new = w_prev + (k1 + k2) / 2;
        y_new = exactSolution(t);

        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t_prev + h, y_new, w_new, abs((y_new - w_new) * 100 / y_new));
        w_prev = w_new;
        t_prev = t;
        t += h;
    }
    return w_new;
}

/* ABM-2 Method to approximate solution of IVP at final_time */
double ABM_2(double initial_value, double initial_time, double final_time, FILE *out_file)
{
    double w_prev = initial_value;
    double t_prev = initial_time;
    double t = t_prev + h;
    double f_1 = f(initial_time, initial_value);

    fprintf(out_file, "Time Exact Value Approximate Value Error\n");
    fprintf(out_file, "%.5f %.8f %.8f %.9f\n", initial_time, exactSolution(t_prev), initial_value, abs((w_prev - w_prev) * 100.0 / w_prev));

    // Calculating next value with Euler's method
    double w_new = initial_value + f_1 * h;
    double f_2 = f(t, w_new);
    double y_new = exactSolution(t);
    fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t, y_new, w_new, abs((y_new - w_new) * 100.0 / y_new));
    w_prev = w_new;
    t_prev = t;
    t += h;
    while (t <= final_time)
    {
        // Prediction Step
        double w_new_pred = w_prev + h * (3 * f_2 - f_1) / 2;
        double f_new_pred = f(t, w_new_pred);

        // Correction Step
        w_new = w_prev + h * (f_new_pred + f_2) / 2;

        double y_new = exactSolution(t);

        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t, y_new, w_new, abs((y_new - w_new) * 100.0 / y_new));
        w_prev = w_new;
        t_prev = t;
        f_1 = f_2;
        f_2 = f(t, w_new);
        t += h;
    }
    return w_new;
}

int main()
{
    double initial_time = 1.0;
    double initialVal = -1 / log(2.0);
    double final_time = 5.0;
    double exact_final_value = exactSolution(final_time);
    printf("Initial Value : %.4f\n", initialVal);
    printf("Exact Value at t = %.4f is %.4f\n\n", final_time, exact_final_value);

    // Output file containing approximated and exact value by RK-2 method will be generated
    FILE *out_rk2 = fopen("out_rk2.txt", "w");
    // approximate value using RK-2 method
    double approx_1 = RK_2(initialVal, initial_time, final_time, out_rk2);
    printf("Approximated Value by RK-2 : %.4f\n", approx_1);
    printf("Percentage Error by RK-2 : %.4f\n", abs((approx_1 - exact_final_value) * 100.0 / exact_final_value));

    printf("\n");
    // Output file containing approximated and exact value by ABM-2 method will be generated
    FILE *out_abm2 = fopen("out_abm2.txt", "w");
    // approximate value using ABM-2 method
    double approx_2 = ABM_2(initialVal, initial_time, final_time, out_abm2);
    printf("Approximated Value by ABM-2 : %.4f\n", approx_2);
    printf("Percentage Error by ABM-2 : %.4f\n", abs((approx_2 - exact_final_value) * 100.0 / exact_final_value));

    return 0;
}