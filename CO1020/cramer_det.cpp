#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

float rec_det(vector <float> A) {
    float a=sqrt(A.size());
    int n=int(a);
    if (n==1) {
        return A[0];
    }
    else if (n>=2) {
        float d=0.0;
        for (int j=0;j<n;j++) {
            vector <float> B((n-1)*(n-1),0.0);
            for (int col=0;col<j;col++) {
                for (int row=1;row<n;row++) {
                    B[(n-1)*(row-1)+col]=A[n*row+col];
                }
            }
            for (int col=j+1;col<n;col++) {
                for (int row =1;row<n;row++) {
                    B[(n-1)*(row-1)+col-1]=A[n*row+col];
                }
            }
            d+=(j%2==0 ? 1:-1)*rec_det(B)*A[j];
            //d+=pow(-1,j)*rec_det(B)*A[j];
        }
        //printf("%d ",n);
        return d;
    }
    return 0;
}

int main(void) {
    int n,i,j;
    cin>>n;
    vector <float> a(n*n,0.0);
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            a[n*i+j]=(i+1)*1000+j+1;
        }
    }
    // auto i=a.begin();
    // for (auto j=i;j<a.end()-n+2;j=j+n) {
    //     for (auto k=j;k<j+n;k++) cout<< *k<<" ";
    //     cout<<endl;
    // }
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) cout<< a[n*i+j]<<" ";
        cout<<endl;
    }
    cout<< rec_det(a)<<endl;
    return 0;
}