/*
C++ Program to solve the system of ODEs
du0/dt = u1
du1/dt = (1 - u0^2)u1 - u0
Author : Aaryan
CO21BTECH11001
*/
#include <bits/stdc++.h>
using namespace std;

#define h 1.0e-3 // Step size
#define close 1.0e-9

/* Returns value of function f(x) */
vector<double> f(vector<double>& x)
{
    vector<double> out = {x[1], -1*x[0]-x[1]*(x[0]*x[0]-1)};
    return out;
}

/* RK-2 Method to simulate first 4 cycles of periodic function using differential equations */
void RK_2(vector<double>& initial_values, double initial_time, FILE *out_file)
{
    int n = initial_values.size();
    vector<double> w_prev = initial_values;
    double t_prev = initial_time;
    vector<double> w_new(n,0);
    double t = t_prev + h;

    fprintf(out_file, "Time Value\n");
    fprintf(out_file, "%.5f %.9f\n", initial_time, initial_values[0]);
    // int count = 0;
    while (true)
    {
        vector<double> k1 = f(w_prev);
        for (auto& it:k1) it=it*h;

        vector<double> temp;
        for (int i=0;i<n;i++) temp.push_back(w_prev[i]+k1[i]);

        vector<double> k2 =  f(temp);
        for (auto& it:k2) it=it*h;

        for (int i=0;i<n;i++) w_new[i] = w_prev[i] + (k1[i]+k2[i])/2;

        fprintf(out_file, "%.5f  %.9f\n", t, w_new[0]);

        if (t-initial_time>=33.0) break;
        w_prev = w_new;
        t_prev = t;
        t += h;
    }
    return;
}

int main()
{
    double initial_time = 0.0;
    vector<double> initialVals = {0.5,0.1};

    printf("Initial Value of x(0) : %.4f\n", initialVals[0]);
    printf("Initial Value of x'(0) : %.4f\n", initialVals[1]);
    printf("Initial Value of x''(0) : %.4f\n\n", initialVals[2]);

    // Output file containing approximated values by RK-2 method will be generated
    FILE *out_file = fopen("out.txt", "w");

    RK_2(initialVals, initial_time, out_file);

    return 0;
}