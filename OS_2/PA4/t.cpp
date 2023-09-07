#include <bits/stdc++.h>
#include <semaphore.h>
using namespace std;
int main() {
    default_random_engine random_number_generator;
    exponential_distribution<double> exp1(1.0/15.0);
    double t = exp1(random_number_generator);
    cout << t << endl;
    sleep(t);
    return 0;
}