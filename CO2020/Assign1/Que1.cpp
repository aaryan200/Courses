/*
C++ Program to solve differential equation
dx/dt = f(t, x) = -( 1 + t + t^2 ) - ( 2t + 1)*x - x*x
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

// Step size
#define h 1.0e-2

/* Returns value of function f(t, x) */
double f(double t, double x)
{
    double out = -1 * (1 + t + t * t) - (2 * t + 1) * x - x * x;
    return out;
}

/* Exact solution in case of x(0) = -0.5 */
double exactSolution1(double t)
{
    double out = -1 * t - (1 / (1 + exp(t)));
    return out;
}

/* Exact solution in case of x(0) = -1 */
double exactSolution2(double t)
{
    double out = -1 * t - 1;
    return out;
}

/* Euler method find solution of IVP at final_time */
double eulerMethod(double initial_value, double initial_time, double final_time, function<double(double)> exact_sol, FILE *out_file)
{
    // w is the approximated value

    double w_prev = initial_value;
    double t_prev = initial_time;
    double w_new, y_new;
    double t = t_prev + h;

    fprintf(out_file, "Time Exact Value Approximate Value Error\n");
    fprintf(out_file, "%.5f %.8f %.8f 0.000000000\n", initial_time, initial_value, initial_value);
    while (t < final_time)
    {
        double f_prev = f(t_prev, w_prev);
        w_new = w_prev + f_prev * h;
        y_new = exact_sol(t);
        fprintf(out_file, "%.5f %.8f %.8f %.9f\n", t_prev + h, y_new, w_new, abs((y_new - w_new) * 100 / y_new));
        w_prev = w_new;
        t_prev = t;
        t += h;
    }
    return w_new;
}

int main()
{
    double inital_time = 0.0;
    double final_time = 3.0;

    double initialVal_1 = -0.5;

    // Output file containing approximated and exact value in case of x(0) = -0.5 will be generated
    FILE *out_file1 = fopen("out1.txt", "w");

    // approximate value using Euler's method
    double approx_1 = eulerMethod(initialVal_1, inital_time, final_time, &exactSolution1, out_file1);

    // Final Exact Value
    double exact_final_val1 = exactSolution1(final_time);

    printf("Initial Value : %.9f\n", initialVal_1);
    printf("Exact Final Value : %.9f\n", exact_final_val1);
    printf("Approximated Final Value : %.9f\n", approx_1);
    printf("Percentage Error : %.9f\n\n", abs(exact_final_val1 - approx_1) * 100 / exact_final_val1);

    double initialVal_2 = -1;

    // Output file containing approximated and exact value in case of x(0) = -1 will be generated
    FILE *out_file2 = fopen("out2.txt", "w");

    // approximate value using Euler's method
    double approx_2 = eulerMethod(initialVal_2, inital_time, final_time, &exactSolution2, out_file2);

    // Final Exact Value
    double exact_final_val2 = exactSolution2(final_time);

    printf("Initial Value : %.9f\n", initialVal_2);
    printf("Exact Final Value : %.9f\n", exact_final_val2);
    printf("Approximated Final Value : %.9f\n", approx_2);
    printf("Percentage Error : %.9f\n", abs(exact_final_val2 - approx_2) * 100 / exact_final_val2);

    fclose(out_file1);
    fclose(out_file2);
    return 0;
}