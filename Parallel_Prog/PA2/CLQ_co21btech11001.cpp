#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
using namespace std;
using namespace std::chrono;
#define M 1.0e6
#define CAPACITY 5000

class CLQ {
private:
    int head, tail;
    vector<int> items;
    sem_t _lock;

public: 
    CLQ(int capacity) {
        head = 0; tail = 0;
        sem_init(&_lock, 0, 1);
        items.resize(capacity);
    }
    void enq(int x) {
        sem_wait(&_lock);
        if (tail - head == items.size()) {
            // cout << "Queue is full" << endl;
            // Queue is full
        }
        else {
            items[tail % items.size()] = x;
            tail++;
        }
        sem_post(&_lock);
    }
    int deq() {
        sem_wait(&_lock);
        if (tail == head) {
            // cout << "Queue is empty, returning -1" << endl;
            // Queue is empty, so return -1 and release the lock
            sem_post(&_lock);
            return -1;
        }
        int x = items[head % items.size()];
        head++;
        sem_post(&_lock);
        return x;
    }
    ~CLQ() {
        sem_destroy(&_lock);
    }
};

// Using FILE* to write log to a file
FILE *output_log_ptr;

// Input parameters
int n, numOps;
double rndLt, _lambda;

// Average operation time
double avgOperationTime;

// Initialize the queue
CLQ qObj(CAPACITY);

// The thread enqueue times array
vector<pair<double,int>> thrEnqTimes;

// The thread dequeue times array
vector<pair<double,int>> thrDeqTimes;

// Runner function of thread
void testThread(int threadId) {

    // Initialize the random number generator with threadId and time as seed
    unsigned seed = static_cast<unsigned>(threadId) * static_cast<unsigned>(time(nullptr));

    // Initialize the random number generator
    default_random_engine random_number_generator(seed);
    uniform_real_distribution<double> unifRandObj(0.0, 1.0);
    uniform_int_distribution<int> unifRandObj2(1, M); // random positive integer value
    exponential_distribution<double> expRandObj(1.0 / _lambda);

    for (int i = 0; i < numOps; i++) {
        double p = unifRandObj(random_number_generator);

        bool isEnq = false;

        double timeTaken;

        // Enqueue
        if (p < rndLt) {
            isEnq = true;

            // random positive integer value
            int x = unifRandObj2(random_number_generator);

            // time before enqueue
            auto startTime = system_clock::now().time_since_epoch();

            // Enqueue
            qObj.enq(x);

            // End time using chrono in microseconds
            auto endTime = system_clock::now().time_since_epoch();

            timeTaken = duration_cast<chrono::microseconds>(endTime - startTime).count();

            // cout << "Thread " << threadId << " enqueued " << x << '\n';
        }
        // Dequeue
        else {
            // time before dequeue
            auto startTime = system_clock::now().time_since_epoch();

            // Dequeue
            int x = qObj.deq();

            // End time using chrono in microseconds
            auto endTime = system_clock::now().time_since_epoch();

            timeTaken = duration_cast<chrono::microseconds>(endTime - startTime).count();

            // cout << "Thread " << threadId << " dequeued " << x << '\n';
        }

        // Time taken
        timeTaken = timeTaken*1.0 / M;

        // Store the execution time of the current operation
        if (isEnq) {
            thrEnqTimes[threadId].first += timeTaken;
            thrEnqTimes[threadId].second++;
        }
        else {
            thrDeqTimes[threadId].first += timeTaken;
            thrDeqTimes[threadId].second++;
        }

        // Simulate perform some other tasks using sleep
        double sleepTime = expRandObj(random_number_generator);

        usleep(sleepTime*M);
    }
}

void computeStats() {
    double totalEnqTime = 0.0, totalDeqTime = 0.0, totalOperationTime = 0.0;
    int totalEnqOps = 0, totalDeqOps = 0;
    for (int i = 0; i < n; i++) {
        totalEnqOps += thrEnqTimes[i].second;
        totalDeqOps += thrDeqTimes[i].second;

        totalEnqTime += thrEnqTimes[i].first;
        totalDeqTime += thrDeqTimes[i].first;
    }
    double avgEnqTime = totalEnqTime / (totalEnqOps*1.0);
    double avgDeqTime = totalDeqTime / (totalDeqOps*1.0);
    avgOperationTime = (totalEnqTime + totalDeqTime) / (totalEnqOps*1.0 + totalDeqOps*1.0);
    long throughput = 1 / avgOperationTime;
    cout << "Average enqueue time: " << avgEnqTime << "s" << '\n';
    cout << "Average dequeue time: " << avgDeqTime << "s" << '\n';
    cout << "Average operation time: " << avgOperationTime << "s" << '\n';
    cout << "Throughput: " << throughput << '\n';
}

// Please pass input file name as command line argument
int main(int argc, char* argv[]) {
    FILE* input_file_ptr = fopen(argv[1], "r");

    if (fscanf(input_file_ptr, "%d %d %lf %lf", &n, &numOps, &rndLt, &_lambda) != 4) {
        printf("Error in reading input file");
        exit(1);
    }

    fclose(input_file_ptr);
    output_log_ptr = fopen("NLQ-out.txt", "w");

    // Initialize the thread times array
    thrEnqTimes.resize(n, {0.0, 0});
    thrDeqTimes.resize(n, {0.0, 0});

    // n threads
    vector<thread> threads;

    // Creating threads
    for (int i = 0; i < n; i++) {
        threads.push_back(thread(testThread, i));
    }

    // Joining all threads
    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    cout << "n = " << n << " numOps = " << numOps << " rndLt = " << rndLt << " lambda = " << _lambda << endl;
    
    computeStats();

    // Writing average operation time to file
    fprintf(output_log_ptr, "%lf\n", avgOperationTime);

    fclose(output_log_ptr);

    return 0;
}