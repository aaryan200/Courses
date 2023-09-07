#include <stdio.h>
#include <stdlib.h>

int add_col(double *A,int i,int j,int m,int x,int y,double *out) {
    int row,col;
    for (col=0;col<y;col++) {
        if (col<i) {
            for (row=0;row<x;row++) out[y*row+col]=A[y*row+col];
        }
        else if (col==i) {
            for (row=0;row<x;row++) out[y*row+col]=A[y*row+col]+m*A[y*row+j];
        }
        else if (col>i) {
            for (row=0;row<x;row++) out[y*row+col]=A[y*row+col];
        }
    }
    return 0;
}

/*Adds m times of jth row of A in ith row of A of size x,y*/
int add_row(double *A,int i,int j,int m,int x,int y,double *out) {
    int row,col;
    for (row =0 ;row<x;row++) {
        if (row<i) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*row+col];
        }
        else if (row==i) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*row+col]+m*A[y*j+col];
        }
        else if (row>i) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*row+col];
        }
    }
    return 0;
}

double det(double *A,int n) {
    double *b=(double *)malloc(sizeof(double)*n*n);
    int i,j,k;
    for (int col=0;col<n;col++) {
        for (int row=0;row<n;row++) b[(n)*row+col]=A[n*row+col];
    }
    double d=1.0;
    for (i=0;i<n;i++) {
        if (abs(b[(n)*i+i])<=1.0e-16) {
            int x=0;
            for (int row=i+1;row<n;row++) {
                if (b[(n)*row+i]!=0) {
                    add_row(b,i,row,1,n,n,b);
                    break;
                }
                else x+=1;
            }
            if (x==n-1-i) {
                return 0.0;
            }
            else {
                for (int row=i+1;row<n;row++) {
                    double r=b[(n)*row+i]/b[(n)*i+i];
                    for (int col=n-1;col>i-1;col-=1) {
                        b[(n)*row+col]-=r*b[(n)*i+col];
                    }
                }
            }
        }
        else {
            for (int row=i+1;row<n;row++) {
                double r=b[(n)*row+i]/b[(n)*i+i];
                for (int col=n-1;col>i-1;col-=1) {
                    b[(n)*row+col]-=r*b[(n)*i+col];
                }
            }
        }
        d=d*b[n*i+i];
    }
    free(b);
    return d;
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
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            A[n*i+j]=rand()%100;
            printf("%lf ",A[n*i+j]);
            // scanf("%lf",&A[n*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Det:%lf\n",det(A,n));
    free(A);
    return 0;
}