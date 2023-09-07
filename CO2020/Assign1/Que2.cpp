/*
C++ Program to solve differential equation for population modelling
dx/dt = f(x) = rx(1- x/k ) - (x^2)/(1 + x^2)
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

// Step size
#define h 0.001

/* Returns value of function f(t, x) */
double f(double x, double r, double k)
{
    double out = r * x * (1 - x / k) - (x * x) / (1 + x * x);
    return out;
}

double r, k;

/* Euler Method to find convergent solution of IVP */
bool eulerMethod(double initial_value, double initial_time)
{
    double tolerance = 1.0e-8;

    // w is the approximated value
    double w_prev = initial_value;
    double t_prev = initial_time;

    // Initializing w_new with a value which will not stop the loop in first iteration
    double w_new = w_prev + 2 * tolerance;
    double t = t_prev + h;
    int count = 0;

    while (abs(w_new - w_prev) > tolerance)
    {
        if (count != 0)
            w_prev = w_new;
        double f_prev = f(w_prev, r, k);
        w_new = w_prev + f_prev * h;
        t_prev = t;
        t += h;
        count++;
    }
    if (abs(w_new - w_prev) <= tolerance)
    {
        printf("Population stabilises at time = %.5f\n", t);
        printf("Eventual population level : %.5f\n", w_new);
        return true;
    }
    return false;
}

int main()
{
    double inital_time = 0.0;
    r = 0.4;
    k = 20;

    // Part a
    double initialVal_1 = 2.44;
    printf("Initial Population : %.4f\n", initialVal_1);

    // approximate value using Euler's method
    if (!eulerMethod(initialVal_1, inital_time))
    {
        printf("Population didn't converged\n");
    }
    printf("\n");

    // Part b
    double initialVal_2 = 2.40;
    printf("Initial Population : %.4f\n", initialVal_2);

    // approximate value using Euler's method
    if (!eulerMethod(initialVal_2, inital_time))
    {
        printf("Population didn't converged\n");
    }

    return 0;
}