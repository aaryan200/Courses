/*
Aaryan
CO21BTECH11001
Parallel and Concurrent Programming Assignment 4
// Wait Free Snapshot Implementation
To compile: g++ WFS_co21btech11001.cpp -o wfs
To run: ./wfs inp-params.txt
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
#define MULT 1.0e6
#define RANGE 100
#define SIZE 20000
#define int short int

// StampedValue class
template <typename T>
class StampedValue {
public:
    T value;
    int stamp;
    int pid;
    int _; // Extra variable to make the size of StampedValue equal to 8 bytes
    // Default constructor
    StampedValue() {
        this->value = 0;
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
    StampedValue(int thread_id, int ts, T v) {
        this->pid = thread_id;
        this->value = v;
        this->stamp = ts;
    }
    static bool isEqual(StampedValue<T> x, StampedValue<T> y) {
        if (x.value == y.value && x.stamp == y.stamp && x.pid == y.pid) return true;
        return false;
    }
};

// Wait Free Snapshot Implementation
template <typename T>
class MRMW_WFS_Snap {
private:
    // Atomic MRMW registers
    atomic<StampedValue<T>> *a_table;
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
        this->a_table = new atomic<StampedValue<T>>[capacity];
        this->a_size = capacity;
        for (int i = 0; i < a_size; i++) {
            this->a_table[i].store(StampedValue<T>());
        }
        this->num_threads = num_threads;
        this->helpsnap = new T*[num_threads];
    };

    void update(int thread_id, int value, int location) {
        StampedValue<T> oldval = a_table[location].load();
        StampedValue<T> newval = StampedValue<T>(thread_id, oldval.stamp + 1, value);
        
        a_table[location].store(newval);

        helpsnap[thread_id] = snapshot();
        return;
    };

    T* snapshot() {
        StampedValue<T> *oldCopy, *newCopy;
        vector<bool> moved(a_size, false);
        oldCopy = collect();
        while (true) {
            bool broken = false;
            newCopy = collect();
            for (int j=0;j<a_size;j++) {
                // Check if oldCopy[j] is equal to newCopy[j]
                bool equal = StampedValue<int>::isEqual(oldCopy[j], newCopy[j]);
                if (!equal) {
                    if (moved[j]) {
                        return helpsnap[newCopy[j].pid];
                    }
                    else {
                        broken = true;
                        moved[j] = true;
                        oldCopy = newCopy;
                        break;
                    }
                }
            }
            if (broken) continue;
            T* result = new T[a_size];
            for (int i=0;i<a_size;i++) {
                result[i] = newCopy[i].value;
            }
            return result;
        }
    }
};

int nw, ns, M, k;
double muw, mus;
ofstream log_file;

vector<vector<long long>> updateOperationTime; // Stores the update times taken by each thread
vector<vector<long long>> snapshotOperationTime; // Stores the snapshot times taken by each thread

// A variable to inform writer threads they have to terminate
atomic<bool> term;
// A pointer to snapshot object, initialized to null, later initialized in main
MRMW_WFS_Snap<int> *MRMW_Snap_Obj = nullptr;

// An array to store the log of threads
vector<string> log_strings(SIZE);
// An atomic variable to store the index of the log_strings array
atomic<int> log_index = 0;

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

        auto startTime = chrono::system_clock::now();

        MRMW_Snap_Obj->update(thread_id, v, l);

        auto endTime = chrono::system_clock::now();
        time_t endTime_t = chrono::system_clock::to_time_t(endTime);
        tm *endTime_local = localtime(&endTime_t);

        // Grab the index of the log_strings array
        int index = log_index.fetch_add(1);
        ostringstream log_string;

        log_string << "Thr" << thread_id+1 << "'s write of " << v << " on location " << l+1 << " at " << put_time(endTime_local, "%X");

        log_strings[index] = log_string.str();

        long long time_taken = duration_cast<chrono::nanoseconds>(endTime - startTime).count();

        updateOperationTime[thread_id].push_back(time_taken);

        sleep_time = expRandObj(random_number_generator);
        usleep(sleep_time * MULT);
    }
}

// Snapshot function
void snapshot_f(int thread_id) {
    double sleep_time;

    // Initialize the random number generator with threadId and time as seed
    unsigned seed = static_cast<unsigned>(thread_id) * static_cast<unsigned>(time(nullptr));

    // Initialize the random number generator
    default_random_engine random_number_generator(seed);

    exponential_distribution<double> expRandObj(1.0 / mus);

    for (int i = 0; i < k; i++) {
        auto startTime = chrono::system_clock::now();

        int* snap = MRMW_Snap_Obj->snapshot();

        auto endTime = chrono::system_clock::now();
        time_t endTime_t = chrono::system_clock::to_time_t(endTime);
        tm *endTime_local = localtime(&endTime_t);

        // Grab the index of the log_strings array
        int index = log_index.fetch_add(1);
        ostringstream log_string;

        log_string << "Snapshot Thr" << thread_id+1 <<"'s snapshot: ";
        for (int j=0;j<M;j++) {
            log_string << "l"<<j+1<<"-"<<snap[j] << " ";
        }
        log_string << "which finished at " << put_time(endTime_local, "%X");

        log_strings[index] = log_string.str();

        long long time_taken = duration_cast<chrono::nanoseconds>(endTime - startTime).count();

        snapshotOperationTime[thread_id].push_back(time_taken);

        sleep_time = expRandObj(random_number_generator);
        usleep(sleep_time * MULT);
    }
}

void computeStats() {
    long long numUpdates = 0, totalUpdateTime = 0, numSnapshots = 0, totalSnapshotTime = 0;
    long long worstCaseUpdateTime = 0, worstCaseSnapshotTime = 0;
    for (int i = 0; i < nw; i++) {
        numUpdates += updateOperationTime[i].size();
        for (auto& t:updateOperationTime[i]) {
            totalUpdateTime += t;
            worstCaseUpdateTime = max(worstCaseUpdateTime, t);
        }
    }
    for (int i = 0; i < ns; i++) {
        numSnapshots += snapshotOperationTime[i].size();
        for (auto& t:snapshotOperationTime[i]) {
            totalSnapshotTime += t;
            worstCaseSnapshotTime = max(worstCaseSnapshotTime, t);
        }
    }
    long long total_op_time = totalUpdateTime + totalSnapshotTime, total_ops = numUpdates + numSnapshots;
    long long worstCaseOperationTime = max(worstCaseUpdateTime, worstCaseSnapshotTime);

    double avgUpdateTime = (double)totalUpdateTime / (1.0 * numUpdates);
    double avgSnapshotTime = (double)totalSnapshotTime / (1.0 * numSnapshots);
    double avgOpTime = (double)total_op_time / (1.0 * total_ops);

    cout << "Average update time: " << avgUpdateTime << " ns" << '\n';
    cout << "Average snapshot time: " << avgSnapshotTime << " ns" << '\n';
    cout << "Average operation time: " << avgOpTime << " ns" << '\n';
    cout << "Worst case update time: " << worstCaseUpdateTime << " ns" << '\n';
    cout << "Worst case snapshot time: " << worstCaseSnapshotTime << " ns" << '\n';
    cout << "Worst case operation time: " << worstCaseOperationTime << " ns" << '\n';
    return;
}

// Please provide input file as command line argument
signed main(signed argc, char* argv[]) {
    // Read the input file
    ifstream input_file;
    input_file.open(argv[1]);
    input_file >> nw >> ns >> M >> muw >> mus >> k;
    input_file.close();

    cout << "nw: " << nw << ", ns: " << ns << ", M: " << M << ", muw: " << muw << ", mus: " << mus << ", k: " << k << '\n';

    // Open the log file
    log_file.open("LogFile_WFS.txt");

    // Lambda is in millisecond
    muw *= 1.0e-3;
    mus *= 1.0e-3;

    // Resize the operationTime vectors
    updateOperationTime.resize(nw, {});
    snapshotOperationTime.resize(ns, {});

    // Snapshot object with capacity M and number of writer threads nw
    MRMW_Snap_Obj = new MRMW_WFS_Snap<int>(M, nw);

    // A variable to inform writer threads they have to terminate
    term.store(false);

    // Create nw writer threads
    vector<thread> writer_threads;
    for (int i = 0; i < nw; i++) {
        writer_threads.push_back(thread(writer_f, i));
    }

    // Create ns snapshot threads
    vector<thread> snapshot_threads;
    for (int i = 0; i < ns; i++) {
        snapshot_threads.push_back(thread(snapshot_f, i));
    }

    // Wait until all the snapshot threads terminate
    for (int i = 0; i < ns; i++) {
        snapshot_threads[i].join();
    }

    term.store(true); // Inform all writer threads that they have to terminate

    // Wait until all writer threads terminate
    for (int i = 0; i < nw; i++) {
        writer_threads[i].join();
    }

    // Compute statistics
    computeStats();

    // Log the log_strings array
    for (int i = 0; i < log_index; i++) {
        log_file << log_strings[i] << '\n';
    }

    log_file.close();

    return 0;
}
