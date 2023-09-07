#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define EPS 1.0e-12

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

int equate_array(double *lhs,double *rhs,int size) {
    for (int i=0;i<size;i++) lhs[i]=rhs[i];
    return 0;
}

double onebyone(double *r,int n) {
    double s=0;
    for (int i=0;i<n;i++) s+=r[i]*r[i];
    return s;
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

double dAd(double *d0T,double *A,double *d0,int n) {
    double *temp;
    temp=(double *)malloc(n*sizeof(double));
    matrix_multiply(d0T,1,n,A,n,n,temp);
    double *out;
    out=(double *)malloc(sizeof(double));
    matrix_multiply(temp,1,n,d0,n,1,out);
    double a;
    a=out[0];
    free(out);
    free(temp);
    return a;
}

int conj_grad(double *A,double *b,double *x,int n) {
    double *r0,*r,*x0,*d,*d0;
    r0=(double *)malloc(n*sizeof(double));
    r=(double *)malloc(n*sizeof(double));
    x0=(double *)malloc(n*sizeof(double));
    d=(double *)malloc(n*sizeof(double));
    d0=(double *)malloc(n*sizeof(double));
    double *temp=(double *)malloc(n*sizeof(double));
    double s;
    int i,j;
    for (i=0;i<n;i++) x0[i]=0.0;
    s=res(x0,b,A,n);
    matrix_multiply(A,n,n,x0,n,1,temp);
    for (i=0;i<n;i++) r0[i]=b[i]-temp[i];
    equate_array(d0,r0,n);
    int count=0;
    while (s>EPS) {
        printf("s=%lf\n",s);
        if (count==10) break;
        double t=dAd(d0,A,d0,n);
        if (abs(t)<=1.0e-15) {
            for (i=0;i<n;i++) printf("x%d=%lf\n",i+1,x[i]);
            return 0;
        }
        printf("dTAd = %lf\n",t);
        double al=s/t;
        for (i=0;i<n;i++) x[i]=x0[i]+al*d0[i];
        double *Ad0=(double *)malloc(n*sizeof(double));
        matrix_multiply(A,n,n,d0,n,1,Ad0);
        for (i=0;i<n;i++) r[i]=r0[i]-al*Ad0[i];
        double be=(-1)*dAd(r,A,d0,n)/t;
        for (i=0;i<n;i++) d[i]=r[i]+be*d0[i];
        equate_array(r0,r,n);
        equate_array(d0,d,n);
        equate_array(x0,x,n);
        s=onebyone(r,n);
        free(Ad0);
        count+=1;
        // for (i=0;i<n;i++) printf("%lf ",x[i]);
        // printf("\n");
    }
    free(temp);
    free(r0);
    free(r);
    free(x0);
    free(d);
    free(d0);
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
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            // A[n*i+j]=rand()%100;
            // printf("%lf ",A[n*i+j]);
            scanf("%lf",&A[n*i+j]);
        }
        // printf("\n");
    }
    printf("\n");
    double *b;
    b=(double *)calloc(n,sizeof(double));
    printf("b:\n");
    for (i=0;i<n;i++) {
        // b[i]=rand()%100;
        // printf("%lf\n",b[i]);
        scanf("%lf",&b[i]);
    }
    printf("\n");
    double *x;
    x=(double *)calloc(n,sizeof(double));
    if (conj_grad(A,b,x,n)==-1) {
        printf("Solution NOT possible\n");
        return -1;
    }
    printf("x:\n");
    for (i=0;i<n;i++) printf("x%d=%lf\n",i+1,x[i]);
    free(A);
    free(b);
    free(x);
    return 0;
}