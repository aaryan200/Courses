#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int lu(double *a,int n,double *l,double *u) {
    int k,i,j;
    for (i=0;i<n;i++) l[n*i+i]=1.0;
    for (j=0;j<n;j++) {
        for (i=0;i<=j;i++) {
            if (i!=j) l[n*i+j]=0.0;
            double sum=a[n*i+j];
            for (k=0;k<i;k++) sum-=l[n*i+k]*u[n*k+j];
            u[n*i+j]=sum;
        }
        for (i=j+1;i<n;i++) {
            u[n*i+j]=0.0;
            if (abs(u[n*j+j])<=1.0e-12) {
                return -1;
            }
            else {
                double sum=a[n*i+j];
                for (k=0;k<j;k++) sum-=l[n*i+k]*u[n*k+j];
                l[n*i+j]=sum/u[n*j+j];
            }
        }
    }
    return 0;
}

int main(void) {
    int n;
    printf("Enter the number of variables: ");
    scanf("%d",&n);
    // double time_spent=0.0;  
    // clock_t begin=clock();
    double *A;
    A=(double *)calloc(n*n,sizeof(double));
    int i,j;
    //printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            A[n*i+j]=rand()%100;
            //printf("%lf ",A[n*i+j]);
        }
        //printf("\n");
    }
    //printf("\n");
    double *l;
    l=(double *)calloc(n*n,sizeof(double));
    double *u;
    u=(double *)calloc(n*n,sizeof(double));
    if (lu(A,n,l,u)==0) {
        // printf("L:\n");
        // for (i=0;i<n;i++) {
        //     for (j=0;j<n;j++){
        //         printf("%lf ",l[n*i+j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        // printf("U:\n");
        // for (i=0;i<n;i++) {
        //     for (j=0;j<n;j++){
        //         printf("%lf ",u[n*i+j]);
        //     }
        //     printf("\n");
        // }
        //printf("\n");
    }
    else printf("LU decomposition of this matrix is not possible \n");
    free(A);
    free(l);
    free(u);
    return 0;
}