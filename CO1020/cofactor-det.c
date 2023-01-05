#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double cofactor(double *A,int size,int row,int col) {
    if (size==2) {
        double out;
        out=(((row+col)%2==0) ? 1:-1)*A[size*(size-1-row)+size-1-col];
        return out;
    }
    else if (size>=3) {
        double out;
        double *B;
        B=(double *)calloc((size-1)*(size-1),sizeof(double));
        int i,j;
        for (i=0;i<row;i++) {
            for (j=0;j<col;j++) B[(size-1)*i+j]=A[size*i+j];
        }
        for (i=0;i<row;i++) {
            for (j=col+1;j<size;j++) B[(size-1)*i+j-1]=A[size*i+j];
        }
        for (i=row+1;i<size;i++) {
            for (j=0;j<col;j++) B[(size-1)*(i-1)+j]=A[size*i+j];
        }
        for (i=row+1;i<size;i++) {
            for (j=col+1;j<size;j++) B[(size-1)*(i-1)+j-1]=A[size*i+j];
        }
        double detb=0.0;
        for (i=0;i<size-1;i++) {
            detb+=B[i]*cofactor(B,size-1,0,i);
        }
        out=(((row+col)%2==0) ? 1:-1)*detb;
        free(B);
        return out;
    }
    return 0;
}

double det(double *A, int size) {
    if (size==1) return A[0];
    else if (size>=2) {
        double out=0.0;
        for (int i=0;i<size;i++) out+=A[i]*cofactor(A,size,0,i);
        return out;
    }
    return 0.0;
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
            a[n*i+j]=(i+1)*1000+j+1;
            printf("%lf ",a[n*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    // int row,col;
    // scanf("%d %d",&row,&col);
    printf("%lf\n",det(a,n));
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time taken: %lf\n",time_spent);
    return 0;
}