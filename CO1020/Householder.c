#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ZERO 1.0e-16
#define EPS 1.0e-10

int print_matrix(double *a,int row,int col) {
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) printf("%lf ",a[col*i+j]);
        printf("\n");
    }
    printf("\n");
    return 0;
}

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

/*out =a +bm*/
int matrix_add(double *a,double *b,int row, int col,double *out,double m) {
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            out[col*i+j]=a[col*i+j]+m*b[col*i+j];
        }
    }
    return 0;
}


int my_eye(double *a,int n) {
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i==j) a[n*i+j]=1.0;
            else a[n*i+j]=0.0;
        }
    }
    return 0;
}

int transpose(double *a,int row,int col,double *out) {
    for (int i=0;i<col;i++) {
        for (int j=0;j<row;j++) out[row*i+j]=a[col*j+i];
    }
    return 0;
}

int my_sign(double a) {
    if (a>0) return 1;
    else if (a<0) return -1;
    return 0;
}

double norm_sq(double *r,int n) {
    double out=0.0;
    for (int i=0;i<n;i++) out+=r[i]*r[i];
    return out;
}

int Q_form(double *a,int k,double *out,int n) {
    int temp=n-k;
    double *v=(double *)malloc(sizeof(double)*(temp));
    for (int i=k;i<n;i++) v[i-k]=a[n*i+k];
    // print_matrix(a,n,n);
    double temp_norm=norm_sq(v,temp);
    v[0]=v[0]+my_sign(v[0])*sqrt(temp_norm);
    temp_norm=norm_sq(v,temp);
    double *F=(double *)malloc(sizeof(double)*temp*temp);
    double *temp1=(double *)malloc(sizeof(double)*(temp));
    double *temp2=(double *)malloc(sizeof(double)*temp);
    for (int i=0;i<temp;i++) temp1[i]=(-2)*v[i];
    transpose(v,temp,1,temp2);
    // print_matrix(temp2,1,temp);
    matrix_multiply(temp1,temp,1,temp2,1,temp,F);
    // printf("%lf\n",temp_norm);
    for (int i=0;i<temp;i++) {
        for (int j=0;j<temp;j++) F[temp*i+j]/=temp_norm;
    }
    for (int i=0;i<temp;i++) F[temp*i+i]+=1;
    // print_matrix(F,temp,temp);
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i<k) {
                if (i==j) out[n*i+j]=1.0;
                else out[n*i+j]=0.0;
            }
            else {
                if (j<k) out[n*i+j]=0.0;
                else out[n*i+j]=F[temp*(i-k)+j-k];
            }
        }
    }
    free(temp1);
    free(temp2);
    free(F);
    free(v);
    return 0;
}

int equate_array(double *lhs,double *rhs,int size) {
    for (int i=0;i<size;i++) lhs[i]=rhs[i];
    return 0;
}

int householder(double *a,int n,double *Q) {
    double *Q0=(double *)malloc(sizeof(double)*n*n);
    double *ac=(double *)malloc(sizeof(double)*n*n);
    equate_array(ac,a,n*n);
    my_eye(Q0,n);
    for (int k=0;k<n;k++) {
        double *temp=(double *)malloc(sizeof(double)*n*n);
        double *temp2=(double *)malloc(sizeof(double)*n*n);
        Q_form(ac,k,temp,n);
        // printf("Q:\n");
        // print_matrix(temp,n,n);
        // printf("Initial:\n");
        // print_matrix(ac,n,n);
        matrix_multiply(temp,n,n,ac,n,n,temp2);
        equate_array(ac,temp2,n*n);
        // printf("After:\n");
        // print_matrix(ac,n,n);
        double *temp1=(double *)malloc(sizeof(double)*n*n);
        transpose(temp,n,n,temp1);
        matrix_multiply(Q0,n,n,temp1,n,n,Q);
        // print_matrix(Q,n,n);
        equate_array(Q0,Q,n*n);
        free(temp1);
        free(temp);
        free(temp2);
    }
    free(Q0);
    free(ac);
    return 0;
}

int main(void) {
    int n;
    scanf("%d",&n);
    double *a=(double *)malloc(sizeof(double)*n*n);
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) scanf("%lf",&a[n*i+j]);
    }
    double *q=(double *)malloc(sizeof(double)*n*n);
    householder(a,n,q);
    printf("Final:\n");
    print_matrix(q,n,n);
    free(q);
    free(a);
    return 0;
}