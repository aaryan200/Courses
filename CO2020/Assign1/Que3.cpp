/*
C++ Program to solve the  differential equation
dT/dt = f(T) = (2s/pd)(T_f^4 - T^4)/(Cp + T(dCp/dT))
where Cp = 0.162T + 446.47
Thermal Equilbrium will be reached when T = T_f
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

#define h 0.01 // Step size
#define sigma 5.67e-8
#define rho 7900.0
#define d 0.002
#define Tf 1500.0 // Temperature of furnace

/* Return value of Cp at temperature T*/
double Cp(double T)
{
    return (0.162 * T + 446.47);
}

/* Return value of derivative of Cp at temperature T*/
double Cp_deriv(double T)
{
    return 0.162;
}
/* Returns value of function f(T) */
double f(double T)
{
    double out = (2.0 * sigma / (rho * d)) * (pow(Tf, 4.0) - pow(T, 4.0)) / (Cp(T) + T * Cp_deriv(T));
    return out;
}

/* RK-4 Method to find convergent solution of IVP */
bool RK_4(double initial_value, double initial_time)
{
    double tolerance = 1.0e-8;

    // w is the approximated value
    double w_prev = initial_value;

    double t_prev = initial_time;
    double t = t_prev + h;

    double f_prev = f(w_prev);

    while (true)
    {
        double k1 = h * f(w_prev);
        double k2 = h * f(w_prev + k1 / 2);
        double k3 = h * f(w_prev + k2 / 2);
        double k4 = h * f(w_prev + k3);

        double w_new = w_prev + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
        t_prev = t;
        f_prev = f(w_new);
        if (abs(f_prev) <= tolerance)
            break;
        t += h;
        w_prev = w_new;
    }
    if (abs(f_prev) <= tolerance)
    {
        printf("Thermal Equilbrium is reached at time = %.5f\n", t);
        return true;
    }
    return false;
}

int main()
{
    double inital_time = 0.0;

    double initialVal = 300.0;
    printf("Initial Temperature : %.4f\n", initialVal);
    printf("Furnace Temperature : %.4f\n", Tf);

    // approximate value using RK-4 method
    if (!RK_4(initialVal, inital_time))
    {
        printf("Thermal Equilbrium will not be reached\n");
        return 0;
    }

    return 0;
}