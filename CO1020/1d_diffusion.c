#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*Solves the system of equations tdm*x=b and return 0 if solution is possible
and return -1 if not*/
int tdma_sol(double *tdm,double *b,double *x,int n) {
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
    free(p);
    free(q);
    return 0;
}

int main(void) {
    int n;
    double Ts,Tl;
    printf("Enter the number of nodes (including first and last node):");
    scanf("%d",&n);
    printf("Enter the temperature at first node:");
    scanf("%lf",&Ts);
    printf("Enter the temperature at last node:");
    scanf("%lf",&Tl);
    double time_spent=0.0;  
    clock_t begin=clock();
    double *a;
    a=(double *)malloc(sizeof(double)*(n-2)*(n-2));
    int dim=n-2;
    a[0]=-2;
    a[1]=1;
    a[dim*dim-2]=1;
    a[dim*dim-1]=-2;
    for (int i=1;i<dim-1;i++) {
        a[dim*i+i-1]=1;
        a[dim*i+i+1]=1;
        a[dim*i+i]=-2;
    }
    double *t;
    t=(double *)malloc(sizeof(double)*dim);
    double *b;
    b=(double *)calloc(dim,sizeof(double));
    b[0]=(-1)*Ts;
    b[dim-1]=(-1)*Tl;
    tdma_sol(a,b,t,dim);
    printf("T at start= %lf\n",Ts);
    printf("In Middle, Temperature varies as follows:\n");
    for (int i=0;i<dim;i++) printf("%d) %lf\n",i+1,t[i]);
    printf("Temperature at the end= %lf\n",Tl);
    free(t);
    free(b);
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken to run code: %lfs\n",time_spent);
    return 0;
}