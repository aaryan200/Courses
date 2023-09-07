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
#define M 5.0e5

int k, m, n;
int numStudentsCompleted = 0;
double l;
sem_t _numStudentsCompleted;
sem_t _subject;                     // Semaphore to be accessed when a student of another subject wants to enter the lab
vector<sem_t> sameSubject;          // Student of subject i needs to access sameSubject[i] to enter the lab
vector<int> numStudents;            // numStudents[i] = number of students of subject i in the lab
// vector<sem_t> numStudentsUpdate;    // numStudentsUpdate[i] = semaphore to be accessed when a student of subject i wants to update numStudents[i]
sem_t numSystemAvail;               // Semaphore to be accessed when a student wants to use a system
time_t totalWaitTime = 0.0;         // Total time of all students to enter the lab after making a request
sem_t _totalWaitTime;               // Semaphore to be accessed when a student wants to update totalWaitTime
vector<time_t> queueTime;

FILE* output_log_ptr;               // Output file pointer

// Function to get current time in string format
string currentTime(tm *t)
{
    string str(asctime(t));
    str.erase(str.end() - 1);
    return str;
}

// Thread parameters
class thread_params
{
public:
    int subjectId;
    int studentId;
    thread_params(int subjectId, int studentId)
    {
        this->subjectId = subjectId;
        this->studentId = studentId;
    }
};

// Thread function of student
void student(thread_params params) {
    int i = params.subjectId, studentId = params.studentId;
    time_t requestTime = time(NULL);
    queueTime.push_back(requestTime);
    tm *requestTime_pass = localtime(&requestTime);
    fprintf(output_log_ptr, "Student %d group k%d requested the lab at %s\n", studentId+1, i+1, currentTime(requestTime_pass).c_str());

    sem_wait(&sameSubject[i]);
    // if (numStudentsCompleted >= n) {
    //     sem_post(&sameSubject[i]);
    //     return;
    // }
    numStudents[i]++;
    if (numStudents[i] == 1) {
        sem_wait(&_subject);
        // if (numStudentsCompleted >= n) {
        //     sem_post(&_subject);
        //     sem_post(&sameSubject[i]);
        //     return;
        // }
    }
    sem_post(&sameSubject[i]);
    sem_wait(&numSystemAvail);
    // if (numStudentsCompleted >= n) {
    //     sem_post(&numSystemAvail);
    //     sem_post(&sameSubject[i]);
    //     sem_post(&_subject);
    //     return;
    // }
    // sem_wait(&_numStudentsCompleted);
    // numStudentsCompleted++;
    // sem_post(&_numStudentsCompleted);

    time_t entryTime = time(NULL);
    tm *entryTime_pass = localtime(&entryTime);
    fprintf(output_log_ptr, "Student %d group k%d entered the lab at %s\n", studentId+1, i+1, currentTime(entryTime_pass).c_str());

    sem_wait(&_totalWaitTime);
    totalWaitTime += entryTime - requestTime;
    sem_post(&_totalWaitTime);

    // sleep for 0.5s
    usleep((int)M);

    time_t exitTime = time(NULL);
    tm *exitTime_pass = localtime(&exitTime);
    fprintf(output_log_ptr, "Student %d group k%d exited the lab at %s\n", studentId+1, i+1, currentTime(exitTime_pass).c_str());

// 
    

    sem_wait(&sameSubject[i]);
    numStudents[i]--;
    if (numStudents[i] == 0)
        sem_post(&_subject);
    sem_post(&sameSubject[i]);
    sem_post(&numSystemAvail);
}

int main() {
    double tm;

    FILE* input_file_ptr = fopen("input.txt", "r");

    if (fscanf(input_file_ptr, "%d %d %lf %d", &k, &m, &l, &n) != 4) {
        printf("Error in reading input file");
        exit(1);
    }
    double critical_time = 0.5;

    fclose(input_file_ptr);
    output_log_ptr = fopen("output.txt", "w");
    sameSubject.resize(k);
    numStudents.resize(k);

    sem_init(&_numStudentsCompleted, 0, 1);
    sem_init(&_subject, 0, 1);
    sem_init(&numSystemAvail, 0, m);
    sem_init(&_totalWaitTime, 0, 1);
    for (int i = 0; i < k; i++) {
        sem_init(&sameSubject[i], 0, 1);
        numStudents[i] = 0;
    }

    srand(time(NULL));
    default_random_engine random_number_generator;
    exponential_distribution<double> threadDelay(1.0 / l);

    vector<int> numStudentsOfSubject(k, 0);
    vector<thread> students;
    int numThreads = 0;
    while (true) {
        for (int i=0;i<numThreads;i++) {
            time_t currTime = time(NULL);
            if (currTime - queueTime[i] >= critical_time) {
                // terminate the thread
                printf("Student %d group k left the queue\n", i+1);
                students[i].detach();
                numThreads--;
                students.erase(students.begin() + i);
                queueTime.erase(queueTime.begin() + i);
                i--;
            }
        }
        printf("Number of students completed: %d\n", numStudentsCompleted);
        if (numStudentsCompleted >= n)
            break;
        int subjectId = rand() % k;
        numStudentsOfSubject[subjectId]++;
        int studentId = numStudentsOfSubject[subjectId] - 1;
        thread_params params(subjectId, studentId);
        students.push_back(thread(student, params));
        tm = threadDelay(random_number_generator);
        usleep(tm * M);
        numThreads++;
        // if (numStudentsCompleted >= n)
        //     break;
    }

    for (int i = 0; i < numThreads; i++)
        students[i].join();

    printf("Average waiting time: %.5lf\n", (double)totalWaitTime / n);
    printf("Number of students completed: %d\n", numStudentsCompleted);

    fclose(output_log_ptr);
    return 0;
}