#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
#include <chrono>
using namespace std;
// using namespace std::chrono;
#define M 1.0e3

// Using FILE* to write to a file
FILE *output_log_ptr;
// Using FILE* to write time to a file
FILE *output_time_ptr;
int n, m, N;

// Array to store the prime numbers
vector<int> primes;

// Semaphore to ensure mutual exclusion during access to primes array
sem_t primes_mutex;

// Returns true if num is prime
bool isPrime(int num) {
    if (num == 1) return false;
    for (int i=2;i*i<=num;i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Runner function of thread
void runner(int threadId) {
    for (int nu = 2*threadId + 1 ; nu <= N; nu += 2*m) {
        if (isPrime(nu)) {
            sem_wait(&primes_mutex);
            primes.push_back(nu);
            sem_post(&primes_mutex);
        }
    }
}


// Please pass input file name as command line argument
int main(int argc, char* argv[]) {
    FILE* input_file_ptr = fopen(argv[1], "r");

    if (fscanf(input_file_ptr, "%d %d", &n, &m) != 2) {
        printf("Error in reading input file");
        exit(1);
    }
    fclose(input_file_ptr);

    cout << "n = " << n << " m = " << m << endl;

    N = (int)(pow(10, n) + 0.5);

    output_log_ptr = fopen("output_sam1.txt", "w");
    output_time_ptr = fopen("time_sam1.txt", "w");

    sem_init(&primes_mutex,0,1);

    // 2 is already pushed into the array
    primes.push_back(2);

    // m threads
    vector<thread> threads;
    
    // Start time using chrono in microseconds
    auto start = chrono::system_clock::now().time_since_epoch();

    // Creating threads
    for (int i = 0; i < m; i++) {
        threads.push_back(thread(runner, i));
    }

    // Joining all threads
    for (int i = 0; i < m; i++) {
        threads[i].join();
    }

    // End time using chrono in microseconds
    auto endTime = chrono::system_clock::now().time_since_epoch();

    double timeTaken = chrono::duration_cast<chrono::microseconds>(endTime - start).count();
    timeTaken = timeTaken*1.0 / M;

    printf("Time taken: %.6lf ms\n", timeTaken);
    fprintf(output_time_ptr, "%.6lf ms", timeTaken);

    sort(primes.begin(), primes.end());
    for (auto& num:primes) {
        fprintf(output_log_ptr, "%d ", num);
    }
    // Closing output file
    fclose(output_log_ptr);
    fclose(output_time_ptr);

    // Destroying semaphores
    sem_destroy(&primes_mutex);

    return 0;
}