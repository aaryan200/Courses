/*
Aaryan
CO21BTECH11001
Parallel and Concurrent Programming Assignment 3
Inbuilt C++ implementation of MRMW atomic register
To compile: g++ MRMW_atomic_co21btech11001.cpp -o MRMW_atomic
To run: ./MRMW_atomic inp-params.txt
*/

#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;
#define M 1.0e6

// Global variables
// n is the number of threads
int n, numOps;
double _lambda;
// Log file
ofstream log_file;
// Vector to store the time each thread spends in reading and writing operations
vector<long long> operationTime;

// Atomic variable from C++
atomic<int> shVar(0);

void testAtomic(int threadId) {
    int lVar;

    // Initialize the random number generator with threadId and time as seed
    unsigned seed = static_cast<unsigned>(threadId) * static_cast<unsigned>(time(nullptr));

    // Initialize the random number generator
    default_random_engine random_number_generator(seed);
    uniform_real_distribution<double> unifRandObj(0.0, 1.0);
    exponential_distribution<double> expRandObj(1 / _lambda);

    for (int i=1; i<=numOps; i++) {
        double p = unifRandObj(random_number_generator);

        auto reqTime = chrono::system_clock::now();
        time_t reqTime_t = chrono::system_clock::to_time_t(reqTime);
        tm *reqTime_local = localtime(&reqTime_t);
        log_file << i << "th action requested at " << put_time(reqTime_local, "%X") << " by thread " << threadId << '\n';

        if (p < 0.5) {
            // read
            lVar = shVar.load();
            log_file << "Value read: " << lVar << '\n';
        }
        else {
            // write
            lVar = (threadId + 1) * numOps;

            shVar.store(lVar);

            log_file << "Value written: " << lVar << '\n';
        }

        auto complTime = chrono::system_clock::now();
        time_t complTime_t = chrono::system_clock::to_time_t(complTime);
        tm *complTime_local = localtime(&complTime_t);
        log_file << i << "th action completed at " << put_time(complTime_local, "%X") << " by thread " << threadId << '\n';

        // Update operation time of thread with id = threadId
        operationTime[threadId] += duration_cast<chrono::microseconds>(complTime - reqTime).count();

        // Sleep for an exponential time
        double sleepTime = expRandObj(random_number_generator);

        usleep(sleepTime * M);
    }
}

// Provide the name of input file as command line argument
int main(int argc, char *argv[]) {
    // Read the input file
    ifstream input_file;
    input_file.open(argv[1]);
    input_file >> n >> numOps >> _lambda;
    input_file.close();

    cout << "n: " << n << ", numOps: " << numOps << ", lambda: " << _lambda << endl;

    // Open the log file
    log_file.open("LogFile_atomic.txt");

    // Resize the operationTime vector
    operationTime.resize(n, 0);

    // Lambda is in millisecond
    _lambda *= 1.0e-3;

    // n threads
    vector<thread> threads;

    // Creating threads
    for (int i = 0; i < n; i++) {
        threads.push_back(thread(testAtomic, i));
    }

    // Joining all threads
    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    log_file.close();

    double totalOperationTime = 0.0;

    for (int i=0; i < n; i++) totalOperationTime += operationTime[i];

    double averageOperationTime = totalOperationTime / (n * numOps * 1.0);

    cout << fixed << setprecision(4);

    cout << "Average operation time: " << averageOperationTime << "\xC2\xB5s" << endl;
    // cout << averageOperationTime << endl;

    return 0;
}