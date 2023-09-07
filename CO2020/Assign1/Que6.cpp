/*
C++ Program to solve the  differential equation
dx/dt = f(t, x) =  4*x/t + (t^4)*e^t
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

#define h 0.001 // Step size

/* Returns value of function f(t, x) */
double f(double t, double x)
{
    double out = 4 * x / t + pow(t, 4.0) * exp(t);
    return out;
}

/* Exact solution in case of x(1) = 0 */
double exactSolution(double t)
{
    double out = pow(t, 4.0) * (exp(t) - exp(1.0));
    return out;
}

/* RK-4 Method to approximate solution of IVP at final_time */
double RK_4(double initial_value, double initial_time, double final_time, FILE *out_file)
{
    double w_prev = initial_value;
    double t_prev = initial_time;
    double w_new, y_new;
    double t = t_prev + h;

    fprintf(out_file, "Time Exact Value Approximate Value Error\n");
    fprintf(out_file, "%.5f %.8f %.8f %.9f\n", initial_time, exactSolution(t_prev), initial_value, abs((w_prev - w_prev) * 100.0 / w_prev));
    while (t < final_time)
    {
        double k1 = h * f(t_prev, w_prev);
        double k2 = h * f(t_prev + h / 2, w_prev + k1 / 2);
        double k3 = h * f(t_prev + h / 2, w_prev + k2 / 2);
        double k4 = h * f(t_prev + h, w_prev + k3);

        w_new = w_prev + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        y_new = exactSolution(t);

        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t_prev + h, y_new, w_new, abs((y_new - w_new) * 100.0 / y_new));
        w_prev = w_new;
        t_prev = t;
        t += h;
    }
    return w_new;
}

/* ABM-4 Method to approximate solution of IVP at final_time */
double ABM_4(double initial_value, double initial_time, double final_time, FILE *out_file)
{
    double w_prev = initial_value;
    double t_prev = initial_time;
    double t = t_prev + h;
    double w_new, y_new;

    // Stores value of f_i, f_(i-1), f_(i-2), f_(i-3)
    double f_i[4];
    f_i[0] = f(initial_time, initial_value);

    fprintf(out_file, "Time Exact Value Approximate Value Error\n");
    fprintf(out_file, "%.5f %.8f %.8f %.9f\n", initial_time, exactSolution(t_prev), initial_value, abs((w_prev - w_prev) * 100.0 / w_prev));

    // Calculating next three values with Euler's method
    for (int i = 1; i < 4; i++)
    {
        w_new = w_prev + f_i[i - 1] * h;
        y_new = exactSolution(t);
        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t, y_new, w_new, abs((y_new - w_new) * 100.0 / y_new));
        f_i[i] = f(t, w_new);
        w_prev = w_new;
        t_prev = t;
        t += h;
    }
    while (t <= final_time)
    {
        double w_new_pred = w_prev + h * (55 * f_i[3] - 59 * f_i[2] + 37 * f_i[1] - 9 * f_i[0]) / 24;
        double f_new_pred = f(t, w_new_pred);
        w_new = w_prev + h * (9 * f_new_pred + 19 * f_i[3] - 5 * f_i[2] + f_i[1]) / 24;
        double y_new = exactSolution(t);

        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t, y_new, w_new, abs((y_new - w_new) * 100.0 / y_new));
        w_prev = w_new;
        t_prev = t;
        for (int i = 0; i < 3; i++)
            f_i[i] = f_i[i + 1];
        f_i[3] = f(t, w_new);
        t += h;
    }
    return w_new;
}

int main()
{
    double initial_time = 1.0;
    double initialVal = 0.0;
    double final_time = 2.0;
    double exact_final_value = exactSolution(final_time);
    printf("Initial Value : %.4f\n", initialVal);
    printf("Exact Value at t = %.4f is %.4f\n\n", final_time, exact_final_value);

    // Output file containing approximated and exact value by RK-4 method will be generated
    FILE *out_rk4 = fopen("out_rk4.txt", "w");
    // approximate value using RK-4 method
    double approx_1 = RK_4(initialVal, initial_time, final_time, out_rk4);
    printf("Approximated Value by RK-4 : %.4f\n", approx_1);
    printf("Percentage Error by RK-4 : %.4f\n", abs((approx_1 - exact_final_value) * 100.0 / exact_final_value));

    printf("\n");
    // Output file containing approximated and exact value by ABM-2 method will be generated
    FILE *out_abm4 = fopen("out_abm4.txt", "w");
    // approximate value using ABM-2 method
    double approx_2 = ABM_4(initialVal, initial_time, final_time, out_abm4);
    printf("Approximated Value by ABM-4 : %.4f\n", approx_2);
    printf("Percentage Error by ABM-4 : %.4f\n", abs((approx_2 - exact_final_value) * 100.0 / exact_final_value));

    return 0;
}