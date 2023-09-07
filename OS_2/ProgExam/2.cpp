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

void fifo(vector<int>& requests_to_page,int frames) {
    queue<int> q;
    set<int> s;
    int pgflts = 0;
    for (auto& r:requests_to_page) {
        // page not present
        if (s.find(r) == s.end()) {
            pgflts++;
            // number of pages present are less than frames available
            if (s.size() < frames) {
                q.push(r);
                s.insert(r);
            }
            // Frames are full
            else {
                int firstIn = q.front();
                q.pop();
                s.erase(firstIn);
                q.push(r);
                s.insert(r);
            }
        }
    }
    cout<<"Number of page faults with FIFO: "<<pgflts<<endl;
    return;
}

void lru(vector<int> requests_to_page, int frames) {
    map<int,int> m;
    int t = 0, pgflts = 0;
    for (auto& r:requests_to_page) {
        // page not present
        if (m.find(r)==m.end()) {
            pgflts++;
            // number of pages present are less than frames available
            if (m.size() < frames) {
                m[r] = t;
            }
            // Frames are full
            else {
                int minEl = INT_MAX, lru_element = 0;
                for (auto& it:m) {
                    if (it.second < minEl) {
                        minEl = it.second;
                        lru_element = it.first;
                    }
                }
                m.erase(lru_element);
                m[r] = t;
            }
        }
        else {
            m[r] = t;
        }
        t++;
    }
    cout<<"Number of page faults with LRU: "<<pgflts<<endl;
    return;
}

int main() {
    FILE* input_file = fopen("input.txt", "r");
    int frames, pgsize, temp;
    fscanf(input_file, "%d\n%d", &frames, &pgsize);
    cout<<frames<<" "<<pgsize<<endl;
    vector<int> request_to_pages;
    while (fscanf(input_file, "\n%d", &temp) != EOF ) {
        request_to_pages.push_back(temp/pgsize);
    }
    for (auto& it:request_to_pages) {
        cout<<it<<endl;
    }
    fifo(request_to_pages, frames);
    lru(request_to_pages, frames);
}