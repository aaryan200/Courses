// C++ Program to demonstrate use of use std::barrier in 
#include <barrier> 
#include <iostream> 
#include <thread> 
#include<cstring>
#include<vector>
using namespace std;


int main() 
{ 
    int nx = 129, ny = 65, numThr = 8;
    for (int ct = 1; ct <= nx*ny; ct+=numThr) {
        int ip = (ct + ny - 1) / ny;
        int jp = ct - (ip - 1) * ny;
        cout << ip << " " << jp << endl;
    } 

	return 0; 
}
