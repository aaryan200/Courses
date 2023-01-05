#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

double *cramer_rule(double *A,double *b,double *x,int n) {
    double det_A=rec_det(A,n);
    for (int i=0;i<n;i++) {
        double *y;
        y=(double *)calloc(n*n,sizeof(double));
        for (int col=0;col<n;col++) {
            if (col<i) {
                for (int row=0;row<n;row++) y[n*row+col]=A[n*row+col];
            }
            else if (col==i) {
                for (int row=0;row<n;row++) y[n*row+col]=b[row];
            }
            else if (col>i) {
                for (int row=0;row<n;row++) y[n*row+col]=A[n*row+col];
            }
        }
        x[i]=rec_det(y,n)/det_A;
        free(y);
    }
    return x;
}

int main(void) {
    int n;
    printf("Enter the number of variables: ");
    scanf("%d",&n);
    double *A;
    A=(double *)calloc(n*n,sizeof(double));
    int i,j;
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            A[n*i+j]=rand()%100;
            printf("%lf ",A[n*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    double *b;
    b=(double *)calloc(n,sizeof(double));
    for (i=0;i<n;i++) {
        b[i]=rand()%100;
        printf("%lf\n",b[i]);
    }
    printf("\n");
    double *x;
    x=(double *)calloc(n,sizeof(double));
    cramer_rule(A,b,x,n);
    for (int i=0;i<n;i++) printf("x%d = %lf\n",i+1,x[i]);
    printf("\n");
    free(x);
    free(b);
    free(A);
    return 0;
}