/*
Aaryan
CO21BTECH11001
Parallel and Concurrent Programming Assignment 4
// Description
To compile: g++ file.cpp -o exec
To run: ./exec inp-params.txt
*/

#include <bits/stdc++.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;
#define MULT 1.0e6
#define RANGE 1000

// StampedValue class
template <typename T>
class StampedValue {
public:
    T value;
    long long stamp;
    int pid;
    // Default constructor
    StampedValue() {
        this->value = (T)NULL;
        this->stamp = 0;
        this->pid = -1;
    }
    // initial value with zero timestamp
    StampedValue(T init) {
        this->value = init;
        this->stamp = 0;
        this->pid = -1;
    }
    // later values with timestamp provided
    StampedValue(int thread_id, long long ts, T v) {
        this->pid = thread_id;
        this->value = v;
        this->stamp = ts;
    }
    // static StampedValue max(StampedValue x, StampedValue y) {
    //     if (x.stamp > y.stamp) return x;
    //     return y;
    // }
};

template <typename T>
class MRMW_WFS_Snap {
private:
    // Atomic MRMW registers
    // atomic<StampedValue<T>> *a_table;
    atomic<vector<int>> *a_table;
    // Size of the table
    int a_size;
    // Snapshot of the threads
    T **helpsnap;
    // Number of threads
    int num_threads;

    // Collect function
    StampedValue<T>* collect() {
        StampedValue<T> *copy = new StampedValue<T>[a_size];
        for (int i = 0; i < a_size; i++) {
            copy[i] = a_table[i].load();
        }
        return copy;
    }

public: 
    MRMW_WFS_Snap(int capacity, int num_threads) {
        this->a_table = new atomic<vector<int>>[capacity];
        this->a_size = capacity;
        for (int i = 0; i < a_size; i++) {
            this->a_table[i].store({-1, -1, -1});
        }
        this->num_threads = num_threads;
        this->helpsnap = new T*[num_threads];
    };

    void update(int thread_id, int value, int location) {
        // StampedValue<T> oldval = a_table[location].load();
        // StampedValue<T> newval = StampedValue<T>(thread_id, oldval.stamp + 1, value);
        // // CAS loop to make sure that the timestamp is correct
        // while (!a_table[location].compare_exchange_weak(oldval, newval)) {
        //     oldval = a_table[location].load();
        //     newval = StampedValue<T>(thread_id, oldval.stamp + 1, value);
        // }
        // helpsnap[thread_id] = snapshot();
    };

    T* snapshot() {
        // StampedValue<T> *oldCopy, *newCopy;
        // vector<bool> moved(a_size, false);
        // oldCopy = collect();
        // while (true) {
        //     newCopy = collect();
        //     bool broken = false;
        //     for (int j=0;j<a_size;j++) {
        //         if (oldCopy[j].stamp != newCopy[j].stamp) {
        //             if (moved[j]) {
        //                 return helpsnap[newCopy[j].pid];
        //             }
        //             else {
        //                 broken = true;
        //                 moved[j] = true;
        //                 oldCopy = newCopy;
        //                 break;
        //             }
        //         }
        //     }
        //     if (broken) continue;
        //     T* result = new T[num_threads];
        //     for (int i=0;i<num_threads;i++) {
        //         result[i] = newCopy[i].value;
        //     }
        //     return result;
        // }
    }
};

int nw, ns, M, k;
double muw, mus;
ofstream log_file;

vector<long long> updateOperationTime;
vector<long long> snapshotOperationTime;

atomic<bool> term;
MRMW_WFS_Snap<int> MRMW_Snap_Obj(1, 1);

// Writer function
void writer_f(int thread_id) {
    // Initialize the random number generator with threadId and time as seed
    unsigned seed = static_cast<unsigned>(thread_id) * static_cast<unsigned>(time(nullptr));

    // Initialize the random number generator
    default_random_engine random_number_generator(seed);
    exponential_distribution<double> expRandObj(1.0 / muw);
    uniform_int_distribution<int> unifRandObj(0, M-1);
    uniform_int_distribution<int> unifRandObj1(1, RANGE);

    int v, l;
    double sleep_time;

    while (!term.load()) {
        v = unifRandObj1(random_number_generator);
        l = unifRandObj(random_number_generator);



        MRMW_Snap_Obj.update(thread_id, v, l);



        // TODO: Time and logging
        sleep_time = expRandObj(random_number_generator);
        usleep(sleep_time * MULT);
    }
}

// Snapshot function
void snapshot_f(int thread_id) {
    double sleep_time;
}

// Please provide input file as command line argument
int main(int argc, char* argv[]) {
    // Read the input file
    ifstream input_file;
    input_file.open(argv[1]);
    input_file >> nw >> ns >> M >> muw >> mus >> k;
    input_file.close();

    cout << "nw: " << nw << ", ns: " << ns << ", M: " << M << ", muw: " << muw << ", mus: " << mus << ", k: " << k << endl;

    // Open the log file
    log_file.open("LogFile_WFS.txt");

    // Lambda is in millisecond
    muw *= 1.0e-3;
    mus *= 1.0e-3;

    // Resize the operationTime vectors
    updateOperationTime.resize(nw, 0);
    snapshotOperationTime.resize(ns, 0);

    // Snapshot object with capacity M and number of writer threads nw
    MRMW_Snap_Obj = MRMW_WFS_Snap<int>(M, nw);

    // A variable to inform writer threads they have to terminate
    term.store(false);

    // Create nw writer threads

    return 0;
}