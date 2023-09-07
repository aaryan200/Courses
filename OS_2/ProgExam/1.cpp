#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <climits>
#include <thread>
#include <atomic>
#include <chrono>
#include <cerrno>
#include <unistd.h>
#include <random>
#include <cmath>
#include <semaphore.h>
#include <bits/stdc++.h>
using namespace std;
#define N 1.0e6

FILE* output_log_ptr;               // Output file pointer
int k, m, n;                        // Input variables
double a, b, l, mu;                 // Input variables

sem_t _full;                        // Initialized to 0, to be accessed by vendor before refilling
sem_t _empty;                       // Initialized to m, to be accessed by family before eating
int numPotsAvail;                   // Number of pots available, initialized to m
int numRefills = 0;                 // Number of refills done by vendor
int tim = 0;                        // time
sem_t _global_sem; // semaphore to ensure mutual exclusion whenever family has to update a global variable

vector<int> numTimesEat;  // numTimesEat[i] = number of times family i has eaten
vector<bool> eaten_n_times;  // if eaten_n_times[i] = true then ith family has eaten n times
bool allDone = false;  // If all family have eaten n times allDone = true

time_t totalWaitToEat = 0.0;        // total time taken by all families while waiting for eat
time_t totalWaitToRefill = 0.0;     // total time taken by vendor while waiting to refill

/*Family thread*/
void familyThread(int familyId) {
    srand(time(NULL));
    default_random_engine random_number_generator;
    exponential_distribution<double> eatTime(1.0 / a);
    exponential_distribution<double> serviceTime(1.0 / b);

    while (numTimesEat[familyId] < n) {
        time_t requestTime = time(NULL);

        sem_wait(&_global_sem);
        tim++;
        fprintf(output_log_ptr, "time%d: Family%d becomes hungry\n", tim, familyId+1);
        sem_post(&_global_sem);

        sem_wait(&_empty);

        time_t accessedTime = time(NULL);
        // sem_wait(&_totalWaitToEat);
        
        // sem_post(&_totalWaitToEat);

        sem_wait(&_global_sem);
        // Write in log
        tim++;
        // Update wait time
        totalWaitToEat += accessedTime - requestTime;
        fprintf(output_log_ptr, "time%d: Family%d eats from the Pot\n", tim, familyId+1);
        numPotsAvail--;
        if (numPotsAvail == 0) sem_post(&_full);

        sem_post(&_global_sem);

        /*Eating time*/
        double eating = eatTime(random_number_generator);
        usleep(eating*N);

        /*Servicing time*/
        double servicing = serviceTime(random_number_generator);
        usleep(servicing*N);

        numTimesEat[familyId]++;
    }
    // Family has eaten n times
    eaten_n_times[familyId] = true;

    sem_wait(&_global_sem);
    tim++;
    fprintf(output_log_ptr, "Family%d has eaten %d times. Hence, exits\n", familyId+1, n);

    for (auto it:eaten_n_times) {
        if (it == false) {
            sem_post(&_global_sem);
            return;
        }
    }
    // If all have eaten, signal vendor to return
    fprintf(output_log_ptr, "time%d: Last thread exits\n", tim);
    allDone = true;
    sem_post(&_full);

    sem_post(&_global_sem);

    return;
}

void vendorThread() {
    srand(time(NULL));
    default_random_engine random_number_generator;
    exponential_distribution<double> putTrayTime(1.0 / l);
    exponential_distribution<double> refillTime(1.0 / mu);

    while (true) {
        time_t initialTime = time(NULL);

        sem_wait(&_full);
        if (allDone) break;

        time_t gotRefillAccess = time(NULL);

        // sem_wait(&_totalWaitToRefill);
        totalWaitToRefill += gotRefillAccess - initialTime;
        // sem_post(&_totalWaitToRefill);

        double putTime = putTrayTime(random_number_generator);
        usleep(putTime*N);

        double fillTime = refillTime(random_number_generator);
        usleep(fillTime*N);

        numRefills++;

        sem_wait(&_global_sem);
        if (allDone) break;
        tim++;
        fprintf(output_log_ptr, "time%d: Vendor refills the ice cream %dth time.\n", tim, numRefills);
        sem_post(&_global_sem);

        // All pots are refilled
        numPotsAvail = m;
        // sem_init(&_empty, 0, m);
        for (int i=0;i<m;i++) {
            sem_post(&_empty);
        }
        if (allDone) break;
    }
    return;
}

int main() {

    FILE* input_file_ptr = fopen("input_semaphore.txt", "r");     // input file pointer
    if (fscanf(input_file_ptr, "%d %d %d %lf %lf %lf %lf", &k, &m, &n, &a, &b, &l, &mu) != 7) {
        printf("Error in reading input file");
        exit(1);
    }
    fclose(input_file_ptr);

    output_log_ptr = fopen("output.txt", "w");

    sem_init(&_full, 0, 0);
    sem_init(&_empty, 0, m);
    sem_init(&_global_sem, 0, 1);

    numTimesEat.resize(k);
    eaten_n_times.resize(k);
    numPotsAvail = m;

    // Vendor thread
    thread vendor(vendorThread);

    // Family threads
    vector<thread> families;

    fprintf(output_log_ptr, "time0: %d threads created.\n", k);
    for (int i=0;i<k;i++) {
        numTimesEat[i] = 0;
        eaten_n_times[i] = false;
        families.push_back(thread(familyThread, i));
    }

    for (int i=0;i<k;i++) families[i].join();
    vendor.detach();

    printf("Average wait time to eat: %.5lf\n", totalWaitToEat/(double)(n*k));
    printf("Average wait time to refill: %.5lf\n", totalWaitToRefill/(double)numRefills);

    fclose(output_log_ptr);
    return 0;
}