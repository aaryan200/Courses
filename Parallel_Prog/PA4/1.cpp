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
#define int short int

typedef struct {
    // int a, b;
    // pair<int,int> b;
    // tuple<int,int,int> c;
    int a;
    int b;
    int c;
    int d;
    // bool a, b, c;
} x;

template <typename T>
class StampedValue {
public:
    T value;
    int stamp;
    int pid;
    int extra;

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
    StampedValue(int thread_id, int ts, T v) {
        this->pid = thread_id;
        this->value = v;
        this->stamp = ts;
    }
    // static StampedValue max(StampedValue x, StampedValue y) {
    //     if (x.stamp > y.stamp) return x;
    //     return y;
    // }
};

signed main() {
    // atomic<x> a;
    // a.store({1, 0, 0, 0});
    // x c = {1, 0, 0, 0};
    // cout << sizeof(c) << endl;
    atomic<StampedValue<int>> a;
    a.store(StampedValue<int>(0, 0, 0));
    // a = {1, {2, 3}};
    // atomic<tuple<int,int,int>> a;
    return 0;
}