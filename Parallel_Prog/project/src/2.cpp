#include <barrier> 
#include <iostream> 
#include <thread> 
#include<cstring>
#include<vector>
using namespace std;

barrier b1(4); // Increase the count to 4
barrier b2(4);

void f1(int i) 
{ 
    b1.arrive_and_wait();
    cout << "Thread " << i << " has finished phase 1" << '\n';
    b2.arrive_and_wait();
} 

void f2() {
    b1.arrive_and_wait(); // Wait for all f1() threads to finish
    cout << "Flag is true" << '\n';
    b2.arrive_and_wait();
    cout << "Thread 4 has finished phase 2" << '\n';
}

int main() 
{ 
    vector<thread> v;
    for (int i=0;i<3;i++) v.push_back(thread(f1, i));

    thread t1(f2);

    for (int i=0;i<3;i++) v[i].join();
    t1.join();

    cout << "All threads have finished" << '\n'; 

    return 0; 
}