#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
using namespace std;

vector <float> matrix_multiply(vector <float> m1,int row1,int col1,vector <float> m2,int row2,int col2) {
    vector <float> v1;
    if (col1==row2) {
        int i,j,k;
        vector <float> out(row1*col2,0.0);
        for (i=0;i<row1;i++) {
            for (j=0;j<col2;j++) {
                for (k=0;k<col1;k++) {
                    out[col2*i+j]+=m1[col1*i+k]*m2[col2*k+j];
                }
            }
        }
        return out;
    }
    return v1;
}

int main(void) {
    
    int m,n,s,t;
    cout<<"Enter the dimensions of first matrix:";
    cin>>m>>n;
    cout<<"Enter the dimensions of second matrix:";
    cin>>s>>t; 
    if (n==s) {
        vector <float> m1(m*n,0.0);
        vector <float> m2(s*t,0.0);
        cout<<"Enter the first matrix:";
        for (int i=0;i<m;i++) {
            for (int j=0;j<n;j++) {
                cin>>m1[n*i+j];
            }
        }
        cout<<"Enter the second matrix:";
        for (int i=0;i<s;i++) {
            for (int j=0;j<t;j++) {
                cin>>m2[t*i+j];
            }
        }
        /*for (int i=0;i<m;i++) {
            for (int j=0;j<n;j++) cout<<m1[n*i+j]<<" ";
            cout<<"\n";
        }
        for (int i=0;i<m;i++) {
            for (int j=0;j<t;j++) cout<<m2[t*i+j]<<" ";
            cout<<"\n";
        }*/
        vector <float> out;//{matrix_multiply(m1,m,n,m2,s,t).begin(),matrix_multiply(m1,m,n,m2,s,t).end()};
        out=matrix_multiply(m1,m,n,m2,s,t);
        for (int i=0;i<m;i++) {
            for (int j=0;j<t;j++) cout<<out[t*i+j]<<" ";
            cout<<"\n";
        }
        return 0;
    }
    else {
        cout<<"Multiplication is not possible";
        return 0;
    }
}