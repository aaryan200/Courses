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

mutex m = mutex();
const int k = 50;


class DiningPhilosophers
{
public:
    enum {THINKING, HUNGRY, EATING} state[k];
    condition_variable self[k];

    DiningPhilosophers() {
        for (int i = 0; i < k; i++) {
            state[i] = THINKING;
        }
    }
    void test(int i) {
        if ((state[i] == HUNGRY) and (state[(i+k-1)%k] != EATING) and (state[(i+1)%k] != EATING)) {
            state[i] = EATING;
            self[i].notify_one();
        }
    }
    void pickup(int i) {
        state[i] = HUNGRY;
        test(i);
        if (state[i] != EATING) {
            unique_lock<mutex> lock(m);
            self[i].wait(lock);
        }
    }
    void putdown(int i) {
        state[i] = THINKING;
        test((i + k-1) % k);
        test((i + 1) % k);
    }
};

int main() {
    DiningPhilosophers d;
    vector<thread> threads;
    for (int i=0;i<k;i++) {
        threads.push_back(thread([&d, i](){
            d.pickup(i);
            cout << "Philosopher " << i+1 << " is eating" << '\n';
            this_thread::sleep_for(chrono::seconds(1));
            d.putdown(i);
        }));
    }
    // thread t1([&d](){
    //     d.pickup(0);
    //     cout << "Philosopher 0 is eating" << '\n';
    //     this_thread::sleep_for(chrono::seconds(1));
    //     d.putdown(0);
    // });
    // thread t2([&d](){
    //     d.pickup(1);
    //     cout << "Philosopher 1 is eating" << '\n';
    //     this_thread::sleep_for(chrono::seconds(1));
    //     d.putdown(1);
    // });
    // thread t3([&d](){
    //     d.pickup(2);
    //     cout << "Philosopher 2 is eating" << '\n';
    //     this_thread::sleep_for(chrono::seconds(1));
    //     d.putdown(2);
    // });
    // thread t4([&d](){
    //     d.pickup(3);
    //     cout << "Philosopher 3 is eating" << '\n';
    //     this_thread::sleep_for(chrono::seconds(1));
    //     d.putdown(3);
    // });
    // thread t5([&d](){
    //     d.pickup(4);
    //     cout << "Philosopher 4 is eating" << '\n';
    //     this_thread::sleep_for(chrono::seconds(1));
    //     d.putdown(4);
    // });
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
    for (int i=0;i<k;i++) {
        threads[i].join();
    }
    return 0;
}