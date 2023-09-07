#include<bits/stdc++.h>
#include<atomic>
#include<thread>
#include <ctime>
using namespace std;
using namespace std::chrono;

FILE* input_file;

class params_TAS {
public:
    atomic<bool>* lock;
    int id;
    int k;
    int l1;
    int l2;
    params_TAS() {}
};

class params_CAS {
public:
    atomic<int>* lock;
    int id;
    int k;
    int l1;
    int l2;
    params_CAS() {}
};

class params_boundedCAS {
public:
    atomic<int>* lock;
    vector<atomic<bool>> waiting;
    int id;
    int k;
    int l1;
    int l2;
    int n;
    params_boundedCAS(vector<atomic<bool>>& waiting) {
        this -> waiting = waiting;
    }
};

bool test_and_set(atomic<bool>* lock) {
    bool rv = *lock;
    *lock = true;
    return rv;
}

void TAS_entry_section(atomic<bool>* lock) {
    while (test_and_set(lock));
} 

void TAS_exit_section(atomic<bool>* lock) {
    *lock = false;
}

bool compare_and_swap(atomic<int>* lock,int expected,int new_value) {
    int temp = *lock;
    if (temp == expected) {
        *lock = new_value;
    }
    return temp;
}

void CAS_entry_section(atomic<int>* lock) {
    while (compare_and_swap(lock,0,1) != 0);
}

void CAS_exit_section(atomic<int>* lock) {
    *lock = 0;
}

void boundedCAS_entry_section(atomic<int>* lock, vector<atomic<bool>>& waiting, int id) {
    waiting[id] = true;
    int key = 1;
    while (waiting[id] and key == 1) {
        key = compare_and_swap(lock,0,1);
    }
    waiting[id] = false;
}

void boundedCAS_exit_section(atomic<int>* lock, vector<atomic<bool>>& waiting, int id, int n) {
    int j = (id+1)%n;
    while (j!=id and !waiting[j]) {
        j = (j+1)%n;
    }
    if (j==id) {
        *lock = 0;
    } 
    else {
        waiting[j] = false;
    }
}

char* currentTime() {
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char *time_string;
    time_string = (char*)malloc(sizeof(char)*9);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", local_time);
    return time_string;
}

void testCS_TAS(params_TAS param) {
    int id = param.id;
    int k = param.k;
    int l1 = param.l1;
    int l2 = param.l2;
    for (int i = 0;i<k;i++) {
        fprintf(input_file,"%dth CS Request at %s by thread %d\n",i,currentTime(),id);
        TAS_entry_section(param.lock); // entry section

        sleep(l1);               // critical section

        TAS_exit_section(param.lock);  // exit section
        fprintf(input_file,"%dth CS Exit at %s by thread %d\n",i,currentTime(),id);
        sleep(l2);               // remainder section
    }
}

void testCS_CAS(params_CAS param) {
    int id = param.id;
    int k = param.k;
    int l1 = param.l1;
    int l2 = param.l2;
    for (int i = 0;i<k;i++) {
        fprintf(input_file,"%dth CS Request at %s by thread %d\n",i,currentTime(),id);
        CAS_entry_section(param.lock); // entry section

        sleep(l1);               // critical section

        CAS_exit_section(param.lock);  // exit section
        fprintf(input_file,"%dth CS Exit at %s by thread %d\n",i,currentTime(),id);
        sleep(l2);               // remainder section
    }
}

void testCS_boundedCAS(params_boundedCAS param) {
    int id = param.id;
    int k = param.k;
    int l1 = param.l1;
    int l2 = param.l2;
    int n = param.n;
    for (int i = 0;i<k;i++) {
        fprintf(input_file,"%dth CS Request at %s by thread %d\n",i,currentTime(),id);
        boundedCAS_entry_section(param.lock,param.waiting,id);   // entry section

        sleep(l1);                                               // critical section

        boundedCAS_exit_section(param.lock,param.waiting,id,n);  // exit section
        fprintf(input_file,"%dth CS Exit at %s by thread %d\n",i,currentTime(),id);
        sleep(l2);                                               // remainder section
    }
}


int main(int argc, char **argv) {
    int n,k,l1,l2;
    input_file = fopen(argv[1],"r");
    fscanf(input_file,"%d %d %d %d",&n,&k,&l1,&l2);

    // Testing critical section by Test and Set mutual exclusion algorithm
    vector<thread> threads_TAS(n);
    fprintf(input_file,"TAS ME Output:\n");

    atomic<bool> TAS_lock = false;

    params_TAS TAS_params[n];

    for (int i = 0;i<n;i++) {
        TAS_params[i].id = i+1;
        TAS_params[i].k = k;
        TAS_params[i].l1 = l1;
        TAS_params[i].l2 = l2;
        TAS_params[i].lock = &TAS_lock;
    }

    for (int i=0;i<n;i++)  threads_TAS[i] = thread(testCS_TAS,TAS_params[i]);
    for (int i=0;i<n;i++)  threads_TAS[i].join();

    // Testing critical section by Compare and Swap mutual exclusion algorithm
    vector<thread> threads_CAS(n);
    fprintf(input_file,"CAS ME Output:\n");

    atomic<int> CAS_lock = 0;

    params_CAS CAS_params[n];

    for (int i = 0;i<n;i++) {
        CAS_params[i].id = i+1;
        CAS_params[i].k = k;
        CAS_params[i].l1 = l1;
        CAS_params[i].l2 = l2;
        CAS_params[i].lock = &CAS_lock;
    }
    for (int i=0;i<n;i++)  threads_CAS[i] = thread(testCS_CAS,CAS_params[i]);
    for (int i=0;i<n;i++)  threads_CAS[i].join();

    // Testing critical section by Bounded Compare and Swap mutual exclusion algorithm
    thread threads_BCAS[n];
    fprintf(input_file,"Bounded CAS ME Output:\n");

    atomic<int> boundedCAS_lock = 0;
    vector<atomic<bool>> waiting(n,false);

    vector<params_boundedCAS> boundedCAS_params(n, params_boundedCAS(waiting));
    
    for (int i = 0;i<n;i++) {
        boundedCAS_params[i].id = i+1;
        boundedCAS_params[i].k = k;
        boundedCAS_params[i].l1 = l1;
        boundedCAS_params[i].l2 = l2;
        boundedCAS_params[i].n = n;
        boundedCAS_params[i].lock = &boundedCAS_lock;
    }
    for (int i=0;i<n;i++)  threads_BCAS[i] = thread(testCS_boundedCAS,boundedCAS_params[i]);
    for (int i=0;i<n;i++)  threads_BCAS[i].join();
    fclose(input_file);
    return 0;
}