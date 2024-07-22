/*
Aaryan
CO21BTECH11001
Parallel and Concurrent Programming Assignment 5
// MCS Lock Implementation
To compile: g++ -std=c++20 MCS-co21btech11001.cpp -o mcs
To run: ./mcs inp-params.txt
*/

#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;
using namespace std::chrono;
#define M 1.0e6

class QNode {
public:
    // Volatile keyword is used to prevent the compiler from applying any optimizations
    volatile bool locked;
    volatile QNode* next;
    QNode() {
        this->locked = false;
        this->next = NULL;
    }
};

class MCSLock {
    atomic<QNode*> tail;
    thread_local static QNode* myNode;

public:
    MCSLock() {
        tail.store(NULL);
    }

    void lock() {
        QNode* qnode = myNode;
        QNode* pred = tail.exchange(qnode);
        if (pred != NULL) {
            qnode->locked = true;
            pred->next = qnode;
            // Wait until predecessor gives up the lock
            while (qnode->locked) {}
        }
    }

    void unlock() {
        QNode* qnode = myNode;
        if (qnode->next == NULL) {
            if (tail.compare_exchange_strong(qnode, NULL)) {
                return;
            }
            // Wait until successor fills in its next field
            while (qnode->next == NULL) {}
        }
        qnode->next->locked = false;
        qnode->next = NULL;        
    }
};

// Initialize thread local variables
thread_local QNode* MCSLock::myNode = new QNode();

// Global variables
int n, k;
double l1, l2;

auto startTime = high_resolution_clock::now();

// Lock object
MCSLock* Test = NULL;

long long csEnterTime = 0; // Measures the total time spent in CS by all threads in microseconds
ofstream log_file; // Log file

// Function to compute the time taken in nanoseconds
long long timeFromStart(auto t1, auto t2) {
    long long timeTaken = duration_cast<nanoseconds>(t2 - t1).count();
    return timeTaken;
}

void testCS(int thread_id) {
    // Initialize the random number generator with threadId and time as seed
    unsigned seed = static_cast<unsigned>(thread_id) * static_cast<unsigned>(time(nullptr));

    // Initialize the random number generator
    default_random_engine random_number_generator(seed);
    exponential_distribution<double> expRandObj1(1.0 / l1);
    exponential_distribution<double> expRandObj2(1.0 / l2);

    double t1, t2;

    for (int i=1; i <= k; i++) {
        auto reqEnterTime = high_resolution_clock::now();

        // log_file << i << "th CS Entry request at " << timeFromStart(startTime, reqEnterTime) << " ns by thread " << thread_id + 1 << '\n';

        Test->lock();

        auto actEnterTime = high_resolution_clock::now();

        // log_file << i << "th CS Entry at " << timeFromStart(startTime, actEnterTime) << " ns by thread " << thread_id + 1 << '\n';

        // Update csEnterTime
        csEnterTime += duration_cast<microseconds>(actEnterTime - reqEnterTime).count();

        t1 = expRandObj1(random_number_generator);
        // usleep(t1 * M); // Sleep for t1*10^6 microseconds = t1 seconds

        auto reqExitTime = high_resolution_clock::now();

        // log_file << i << "th CS Exit Request at " << timeFromStart(startTime, reqExitTime) << " ns by thread " << thread_id + 1 << '\n';

        Test->unlock();

        auto actExitTime = high_resolution_clock::now();

        // log_file << i << "th CS Exit at " << timeFromStart(startTime, actExitTime) << " ns by thread " << thread_id + 1 << '\n';

        t2 = expRandObj2(random_number_generator);
        // usleep(t2 * M); // Sleep for t2*10^6 microseconds = t2 seconds
    }
}

// Please pass input file name as command line argument
int main(int argc, char* argv[]) {
    FILE* input_file_ptr = fopen(argv[1], "r");

    if (fscanf(input_file_ptr, "%d %d %lf %lf", &n, &k, &l1, &l2) != 4) {
        printf("Error in reading input file\n");
        exit(1);
    }

    fclose(input_file_ptr);

    cout << "n: " << n << ", k: " << k << ", l1: " << l1 << ", l2: " << l2 << '\n';

    // Lambda in millisecond
    l1 = l1 * 1.0e-3;
    l2 = l2 * 1.0e-3;

    // Open the log file
    log_file.open("MCS-out.txt");

    // Initialize csEnterTime to 0
    csEnterTime = 0;

    // Start the timer
    startTime = high_resolution_clock::now();

    log_file << "The start time is " << timeFromStart(startTime, startTime) << " ns\n";

    // Initialize the lock
    Test = new MCSLock();

    // array of threads
    vector<thread> threads;

    // Create n threads
    for (int i = 0; i < n; i++) {
        threads.push_back(thread(testCS, i));
    }

    // Join all threads
    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    auto endTime = high_resolution_clock::now();

    log_file << "The end time is " << timeFromStart(startTime, endTime) << " ns\n";

    // Close the log file
    log_file.close();

    // Compute the average time spent in CS by all threads
    double avgCsEnterTime = csEnterTime / (n * k * 1.0);

    // Compute the throughput in operations per second
    double throughput = (n * k * M) / (duration_cast<microseconds>(endTime - startTime).count());

    printf("Average critical section entry time: %.9lf \xC2\xB5s\n", avgCsEnterTime);
    printf("Throughput: %.9lf operations/sec\n", throughput);

    return 0;
}