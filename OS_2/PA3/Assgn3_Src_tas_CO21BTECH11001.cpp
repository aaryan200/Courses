#include <bits/stdc++.h>
#include <atomic>
#include <thread>
#include <ctime>
using namespace std;
using namespace std::chrono;

FILE *out_file;

// Mutex lock initialized to false
atomic_flag _lock = ATOMIC_FLAG_INIT;

time_t totalWaitTime = 0.0;

time_t worstWaitTime = -1.0;

// Class to store the parameters of each thread
class params_TAS
{
public:
    int id;     // Thread ID
    int k;      // Number of times to enter CS
    double l1;  // Mean of sleep time of critical section of thread
    double l2;  // Mean of sleep time of remainder section of thread
    params_TAS(int id, int k, double l1, double l2)
    {
        this->id = id;
        this->k = k;
        this->l1 = l1;
        this->l2 = l2;
    }
};

// Function to get current time in string format
string currentTime(tm *t)
{
    string str(asctime(t));
    str.erase(str.end() - 1);
    return str;
}

// Function to test critical section
void testCS(params_TAS param)
{
    int id = param.id;
    int k = param.k;
    double l1 = param.l1;
    double l2 = param.l2;

    default_random_engine random_number_generator;

    exponential_distribution<double> delay_1(1.0 / l1);
    exponential_distribution<double> delay_2(1.0 / l2);

    for (int i = 0; i < k; i++)
    {

        // Request to enter time
        time_t reqEnterTime = time(NULL);
        tm *reqEnterTime_pass = localtime(&reqEnterTime);

        fprintf(out_file, "%dth CS Request at %s by thread %d\n", i + 1, currentTime(reqEnterTime_pass).c_str(), id + 1);

        // Entry Section
        while (_lock.test_and_set())
            ;

        // Entry time
        time_t actEnterTime = time(NULL);
        tm *actEnterTime_pass = localtime(&actEnterTime);

        fprintf(out_file, "%dth CS Entry at %s by thread %d\n", i + 1, currentTime(actEnterTime_pass).c_str(), id + 1);

        // Critical Section
        double t1 = delay_1(random_number_generator);
        sleep(t1);

        // Time for exiting
        time_t exitTime = time(NULL);
        tm *exitTime_pass = localtime(&exitTime);

        fprintf(out_file, "%dth CS Exit at %s by thread %d\n", i + 1, currentTime(exitTime_pass).c_str(), id + 1);

        // Exit section
        // Resetting lock to false
        _lock.clear();

        // Remainder Section
        double t2 = delay_2(random_number_generator);
        sleep(t2);

        totalWaitTime += (actEnterTime - reqEnterTime);
        worstWaitTime = max(worstWaitTime, (actEnterTime - reqEnterTime));
    }
}

// Please pass input file name as command line argument
int main(int argc, const char *argv[])
{
    int n, k;
    double l1, l2;

    // Taking input from file
    FILE *input_file = fopen(argv[1], "r");
    fscanf(input_file, "%d %d %lf %lf", &n, &k, &l1, &l2);

    // Opening output file
    out_file = fopen("output_tas.txt", "w");

    // Testing critical section by Test and Set mutual exclusion algorithm
    fprintf(out_file, "TAS ME Output:\n");

    // Creating n threads
    vector<thread> threads_TAS(n);

    // Creating parameters for each thread
    vector<params_TAS> TAS_params(n, params_TAS(0, 0, 0.0, 0.0));

    // Passing parameters to each thread and creating them
    for (int i = 0; i < n; i++)
    {
        TAS_params[i].id = i;
        TAS_params[i].k = k;
        TAS_params[i].l1 = l1;
        TAS_params[i].l2 = l2;
        threads_TAS[i] = thread(testCS, TAS_params[i]);
    }

    // Joining all threads
    for (int i = 0; i < n; i++)
        threads_TAS[i].join();

    cout << "Average wait time: " << totalWaitTime / (n * k * 1.0) << "s" << endl;
    cout << "Worst wait time: " << worstWaitTime << "s" << endl;

    fclose(out_file);
    fclose(input_file);
    return 0;
}