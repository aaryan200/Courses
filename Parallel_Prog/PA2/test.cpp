#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
using namespace std;
using namespace std::chrono;
#define M 1.0e6

double _lambda = 5.0;
int numOps = 10;
double rndLt = 0.5;
void testThread(int threadId) {

    // Initialize the random number generator
    default_random_engine random_number_generator;
    uniform_real_distribution<double> unifRandObj(0.0, 1.0);
    exponential_distribution<double> expRandObj(1.0 / _lambda);

    time_point<system_clock> startTime, endTime;

    for (int i = 0; i < numOps; i++) {
        double p = unifRandObj(random_number_generator);
        // Enqueue
        if (p < rndLt) {
            // random integer value
            int x = unifRandObj(random_number_generator) * M;
            // time before enqueue
            startTime = system_clock::now();

            usleep(M);
            // End time using chrono in microseconds
            endTime = system_clock::now();
        }
        // Dequeue
        else {
            // time before dequeue
            startTime = system_clock::now();
            usleep(M);

            // End time using chrono in microseconds
            endTime = system_clock::now();
        }

        // Time taken
        double timeTaken = duration_cast<chrono::microseconds>(endTime - startTime).count();
        timeTaken = timeTaken*1.0 / M;

        cout << "Time taken: " << timeTaken << "s" << endl;

        // Store the execution time of the current operation
        // thrTimes[threadId] += timeTaken;

        // Simulate perform some other tasks using sleep
        double sleepTime = expRandObj(random_number_generator);
        // usleep(sleepTime*M);
        cout << "Sleep time: " << sleepTime << "s" << endl;
    }
}

vector<int> v;


int main() {
    // // generate random number between 0 and 1 using uniform distribution
    // default_random_engine random_number_generator;
    // exponential_distribution<double> expRandObj(1.0 / 5.0);
    // for (int i=0;i<50;i++) {
    //     double sleepTime = expRandObj(random_number_generator);
    //     cout << sleepTime << " ";
    //     // usleep(sleepTime*1.0e6);
    // }
    // cout << endl;
    // testThread(0);
    return 0;
}