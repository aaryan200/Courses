
/* This program takes the number of rows/columns of a square matrix in 1st line
   Then it prints the matrix of a definite pattern
   Then it prints the determinant of that matrix followed by time taken to run this code */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Input - a square matrix, the number of rows/columns of that matrix
   Output - determinant of given matrix
   Return type - double */
double rec_det(double *A,int n) {
    if (n==1) {
        return A[0];
    }
    else if (n>=2) {
        double d=0.0;
        for (int j=0;j<n;j++) {
            double *B;
            B=(double *)malloc((n-1)*(n-1)*sizeof(double));
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
            d+=(j%2==0 ? 1:-1)*rec_det(B,n-1)*A[j];
            free(B);
        }
        return d;
    }
    return 0;
}


int main(void) {
    int n,i,j;
    scanf("%d",&n);
    double time_spent=0.0;  
    clock_t begin=clock();
    double *a;
    a=(double *)malloc(sizeof(double)*n*n);
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            a[n*i+j]=rand()%100;
            printf("%lf ",a[n*i+j]);
        }
        printf("\n");  
    }
    printf("\n");
    printf("%lf\n",rec_det(a,n));
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}