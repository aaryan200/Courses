#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int random_tdm(int n,double *m) {
    m[0]=rand()%100;
    m[1]=rand()%100;
    m[n*n-2]=rand()%100;
    m[n*n-1]=rand()%100;
    for (int i=1;i<=n-2;i++) {
        m[n*i+i-1]=rand()%100;
        m[n*i+i]=rand()%100;
        m[n*i+i+1]=rand()%100;
    }
    return 0;
}

int tdma_sol(double *tdm,double *b,double *x,double* bdm,int n) {
    double *p,*q;
    p=(double *)malloc(sizeof(double)*n);
    q=(double *)malloc(sizeof(double)*n);
    p[0]=(-1)*tdm[1]/tdm[0];
    q[0]=b[0]/tdm[0];
    p[n-1]=0;
    for (int i=1;i<=n-1;i++) {
        double temp=tdm[n*i+i]+tdm[n*i+i-1]*p[i-1];
        if (abs(temp)<=1.0e-12) return -1;
        if (i!=n-1) p[i]=(-1)*tdm[n*i+i+1]/temp;
        q[i]=(b[i]-tdm[n*i+i-1]*q[i-1])/temp;
    }
    x[n-1]=q[n-1];
    for (int i=n-2;i>=0;i=i-1) x[i]=p[i]*x[i+1]+q[i];

    for (int i=0;i<n;i++) {
        bdm[n*i+i]=1;
        if (i!=n-1) bdm[n*i+i+1]=(-1)*p[i];
    }
    free(p);
    free(q);
    return 0;
}

int main(void) {
    int n;
    printf("Enter the number of variables: ");
    scanf("%d",&n);
    double *A;
    A=(double *)calloc(n*n,sizeof(double));
    int i,j;
    random_tdm(n,A);
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
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
    double *B;
    B=(double *)calloc(n*n,sizeof(double));
    double *x;
    x=(double *)calloc(n,sizeof(double));
    if (tdma_sol(A,b,x,B,n)==-1) {
        printf("TDMA method is not applicable for this matrix\n");
        return -1;
    }
    printf("Bidiagonal matrix for this is:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            printf("%lf ",B[n*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i=0;i<n;i++) printf("x%d = %lf\n",i+1,x[i]);
    printf("\n");
    free(A);
    free(B);
    free(x);
    free(b);
    return 0;
}