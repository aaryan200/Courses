#include <bits/stdc++.h>
#include <semaphore.h>
#include <atomic>
#include <thread>
#include <ctime>
using namespace std;
using namespace std::chrono;
#define M 1.0e6

// Using FILE* to write to a file
FILE *output_log_ptr;
// Total time taken from entry of passegner to exit of passenger
time_t totalPassengerTime = 0;

// Total ride time of car
time_t totalCarTime = 0;

// Semaphores to ensure mutual exclusion during incrementing of totalPassengerTime and totalCarTime
sem_t _totalPassengerTime;
sem_t _totalCarTime;

// Number of passengers, cars and number of ride requests per passenger
int n,m,k;
// Exponential wait time parameters
double lp, lc;

// Vector to store the semaphores for each car
vector<sem_t> car_sem;
// Vector to store the semaphores for each passenger
vector<sem_t> passenger_sem;
// Vector to keep track of which passenger is in which car
vector<int> passengerInCar;
// Vector to store the buffer of available cars
vector<int> car_buffer;
// Semaphore to ensure mutual exclusion during access to car_buffer
sem_t _mutex;
// Semaphore to ensure that a passenger can't take a ride if buffer is empty
sem_t _empty;

// Variable to store the total number of requests completed
int totalRequestsCompleted = 0;
// Semaphore to ensure mutual exclusion during increment of totalRequestsCompleted
sem_t _totalRequest;

// Function to get current time in string format
string currentTime(tm *t)
{
    string str(asctime(t));
    str.erase(str.end() - 1);
    return str;
}

// Thread function of passenger
void passenger(int passengerId) {
    // Entry time
    time_t entryTime = time(NULL);
    tm *entryTime_pass = localtime(&entryTime);
    fprintf(output_log_ptr, "Passenger %d enters the museum at %s\n", passengerId+1, currentTime(entryTime_pass).c_str());

    default_random_engine random_number_generator;
    exponential_distribution<double> passengerDelay(1.0 / lp);

    // Randomly generate time to roam around the museum
    double roamTime = passengerDelay(random_number_generator);
    usleep(roamTime*M);

    for (int i =0;i<k;i++) {
        time_t requestTime = time(NULL);
        tm *requestTime_pass = localtime(&requestTime);
        fprintf(output_log_ptr, "Passenger %d made a ride request at %s\n", passengerId+1, currentTime(requestTime_pass).c_str());

        // Wait for a car to be available
        sem_wait(&_empty);
        // Wait to remove a car from car_buffer
        sem_wait(&_mutex);

        // Id of the car which is available
        int carId = car_buffer.back();
        car_buffer.pop_back();

        // Store the passengerId in the array of passengerInCar
        passengerInCar[carId] = passengerId;

        // Release the mutex
        sem_post(&_mutex);

        // Signal the car that a passenger is ready to ride
        sem_post(&car_sem[carId]);
        
        // Wait for the car to finish the ride
        sem_wait(&passenger_sem[passengerId]);

        time_t rideEndTime = time(NULL);
        tm* rideEndTime_pass = localtime(&rideEndTime);
        fprintf(output_log_ptr, "Passenger %d finished riding at %s\n", passengerId+1, currentTime(rideEndTime_pass).c_str());

        // Increment totalRequestsCompleted
        sem_wait(&_totalRequest);

        totalRequestsCompleted++;
        // If all requests are completed, signal all the cars
        if (totalRequestsCompleted == n*k) {
            for (int i=0;i<m;i++) {
                sem_post(&car_sem[i]);
            }
        }
        // Release the mutex
        sem_post(&_totalRequest);

        // Passenger will not sleep if it was the last ride request
        if (i!=k-1) {
            double passengerWaitTime = passengerDelay(random_number_generator);
            usleep(passengerWaitTime*M);
        }
    }

    time_t exitTime = time(NULL);
    tm *exitTime_pass = localtime(&exitTime);
    fprintf(output_log_ptr, "Passenger %d exit the museum at %s\n", passengerId+1, currentTime(exitTime_pass).c_str());

    // Increment totalPassengerTime
    sem_wait(&_totalPassengerTime);
    totalPassengerTime += exitTime - entryTime;
    sem_post(&_totalPassengerTime);
}

// Thread function of car
void car(int carId) {
    time_t carStartTime = time(NULL);

    default_random_engine random_number_generator;
    exponential_distribution<double> carDelay(1.0 / lc);

    while (true) {
        // Wait for a passenger to be signal
        sem_wait(&car_sem[carId]);
        // If all requests are completed, break the loop
        if (totalRequestsCompleted == n*k) {
            break;
        }

        // Id of the passenger in the car
        int passengerId = passengerInCar[carId];

        fprintf(output_log_ptr, "Car %d accepts Passenger %d's request\n", carId+1, passengerId+1);

        time_t rideStartTime = time(NULL);
        tm *rideStartTime_pass = localtime(&rideStartTime);
        fprintf(output_log_ptr, "Passenger %d started riding at %s\n", passengerId+1, currentTime(rideStartTime_pass).c_str());

        fprintf(output_log_ptr, "Car %d is riding Passenger %d\n", carId+1, passengerId+1);

        double rideTime = carDelay(random_number_generator);
        usleep(rideTime*M);

        fprintf(output_log_ptr, "Car %d finished Passenger %d's tour\n", carId+1, passengerId+1);

        // Signal the passenger that the ride is over
        sem_post(&passenger_sem[passengerId]);

        // If all requests are completed, break the loop
        if (totalRequestsCompleted == n*k) {
            break;
        }

        // Randomly generate time to wait for next ride request
        double carWaitTime = carDelay(random_number_generator);
        usleep(carWaitTime*M);

        // Now, car is available. So, push itself to car_buffer
        sem_wait(&_mutex);

        car_buffer.push_back(carId);

        sem_post(&_mutex);
        sem_post(&_empty);
    }

    time_t carDoneTime = time(NULL);
    
    // Increment totalCarTime
    sem_wait(&_totalCarTime);
    totalCarTime += carDoneTime-carStartTime;
    sem_post(&_totalCarTime);

}

// Please pass input file name as command line argument
int main(int argc, char* argv[]) {
    FILE* input_file_ptr = fopen(argv[1], "r");

    if (fscanf(input_file_ptr, "%d %d %lf %lf %d", &n, &m, &lp, &lc, &k) != 5) {
        printf("Error in reading input file");
        exit(1);
    }

    fclose(input_file_ptr);
    output_log_ptr = fopen("output.txt", "w");

    car_sem.resize(m);
    passenger_sem.resize(n);
    passengerInCar.resize(m);
    car_buffer.resize(m);

    // Initializing semaphores
    for (int i = 0; i < m; i++) {
        sem_init(&car_sem[i], 0, 0);
        passengerInCar[i] = 0;
    }
    for (int i =0;i<n;i++) sem_init(&passenger_sem[i],0,0);
    sem_init(&_mutex,0,1);
    sem_init(&_empty,0,m);
    sem_init(&_totalPassengerTime,0,1);
    sem_init(&_totalCarTime,0,1);
    sem_init(&_totalRequest,0,1);

    // m car threads
    vector<thread> car_threads;
    // n passenger threads
    vector<thread> passenger_threads;
    
    // Creating passenger threads
    for (int i = 0; i < n; i++) {
        passenger_threads.push_back(thread(passenger, i));
    }

    // Creating car threads and pushing their id to car_buffer
    for (int i =0;i<m;i++) {
        car_threads.push_back(thread(car, i));
        car_buffer[i] = i;
    }
    // Joining all threads
    for (int i = 0; i < n; i++) {
        passenger_threads[i].join();
    }
    for (int i = 0; i < m; i++) {
        car_threads[i].join();
    }

    // Calculating average time
    double averagePassengerTime = totalPassengerTime / (n*1.0);
    double averageCarTime = totalCarTime / (1.0*m);
    printf("Average Passenger Time: %.3lf\n", averagePassengerTime);
    printf("Average Car Time: %.3lf\n", averageCarTime);

    // Closing output file
    fclose(output_log_ptr);

    // Destroying semaphores
    sem_destroy(&_mutex);
    sem_destroy(&_empty);
    sem_destroy(&_totalPassengerTime);
    sem_destroy(&_totalCarTime);
    sem_destroy(&_totalRequest);
    for (int i = 0; i < m; i++) {
        sem_destroy(&car_sem[i]);
    }
    for (int i =0;i<n;i++) sem_destroy(&passenger_sem[i]);

    return 0;
}