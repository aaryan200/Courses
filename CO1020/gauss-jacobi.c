#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int matrix_multiply(double *m1,int row1,int col1, double *m2,int row2,int col2, double *out) {
    if (col1==row2) {
        int i,j,k;
        for (i=0;i<row1;i++) {
            for (j=0;j<col2;j++) {
                out[col2*i+j]=0.0;
                for (k=0;k<col1;k++) {
                    out[col2*i+j]+=m1[col1*i+k]*m2[col2*k+j];
                }
            }
        }
        return 0;
    }
    return -1;
}

int equate_array(double *lhs,double *rhs,int n) {
    for (int i=0;i<n;i++) lhs[i]=rhs[i];
    return 0;
}

int check_diag_dom(double *a,int n) {
    int i,j,count=0;
    for (i=0;i<n;i++) {
        int x=0;
        double m=abs(a[n*i+i]);
        for (j=0;j<n;j++) {
            if (j!=i && m>abs(a[n*i+j])) x+=1;
        }
        if (x==n-1) count+=1;
    }
    if (count==n) return 1;
    return -1;
}

double res(double *x,double *b,double *A,int n) {
    double *Ax;
    Ax=(double *)malloc(sizeof(double)*n);
    matrix_multiply(A,n,n,x,n,1,Ax);
    for (int i=0;i<n;i++) Ax[i]=b[i]-Ax[i];
    double r=0.0;
    for (int i=0;i<n;i++) r+=Ax[i]*Ax[i];
    free(Ax);
    return r;
}

double res1(double *x0,double *A,double *x,int n) {
    int i;
    double *Ax;
    Ax=(double *)malloc(sizeof(double)*n);
    for (i=0;i<n;i++) x0[i]-=x[i];
    matrix_multiply(A,n,n,x0,n,1,Ax);
    double r=0.0;
    for (int i=0;i<n;i++) r+=Ax[i]*Ax[i];
    free(Ax);
    return r;
}

int ga_ja(double *a,double *b,double *x,int n,double tol) {
    if (check_diag_dom(a,n)==-1) return -1;
    double *x0;
    x0=(double *)calloc(n,sizeof(double));
    double residual=res(x0,b,a,n);
    int i,j,it=0;
    while (residual>tol) {
        for (i=0;i<n;i++) {
            double sum=b[i];
            for (j=0;j<n;j++) {
                if (j!=i) sum-=a[n*i+j]*x0[j];
            }
            if (abs(a[n*i+i])<=1.0e-12) return -1;
            x[i]=sum/a[n*i+i];
        }
        residual=res1(x0,a,x,n);
        equate_array(x0,x,n);
        it+=1;
    }
    free(x0);
    return it;
}

int main(void) {
    int n;
    printf("Enter the number of variables: ");
    scanf("%d",&n);
    double eps;
    printf("Enter the precision:");
    scanf("%lf",&eps);
    double *A;
    A=(double *)calloc(n*n,sizeof(double));
    int i,j;
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            scanf("%lf",&A[n*i+j]);
            // A[n*i+j]=rand()%100;
            // printf("%lf ",A[n*i+j]);
        }
        //printf("\n");
    }
    printf("\n");
    double *b;
    b=(double *)calloc(n,sizeof(double));
    printf("b:\n");
    for (i=0;i<n;i++) {
        //b[i]=rand()%100;
        scanf("%lf",&b[i]);
        //printf("%lf\n",b[i]);
    }
    printf("\n");
    double *x;
    x=(double *)calloc(n,sizeof(double));
    int it=ga_ja(A,b,x,n,eps);
    if (it==-1) {
        printf("Data is inappropriate\n"); 
        return -1;
    }
    printf("x:\n");
    for (int i=0;i<n;i++) printf("x%d=%lf\n",i+1,x[i]);
    printf("\nNumber of iterations: %d\n",it);
    free(A);
    free(b);
    free(x);
    return 0;
}