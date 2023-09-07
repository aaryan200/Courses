#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "/home/aaryan/Desktop/CO1020/libraries/linalg.h"
#define ZERO 1.0e-16
#define EPS 1.0e-10
#define MAX_IT 5.0e6

// /*return the absolute value of variable x*/
// double my_abs(double x) {
//     double out=(x>0) ? x:(-1)*x;
//     return out;
// }

// /*Prints the matrix a with rows=row and columns=col*/
// int print_matrix(double *a,int row,int col) {
//     for (int i=0;i<row;i++) {
//         for (int j=0;j<col;j++) printf("%.4lf ",a[col*i+j]);
//         printf("\n");
//     }
//     printf("\n");
//     return 0;
// }

// /* Input - matrix m1 and its dimensions and matrix m2 and its dimensions and required output matrix
// Output - the matrix out becomes the multiplication of matrices m1,m2*/
// int matrix_multiply(double *m1,int row1,int col1, double *m2,int row2,int col2, double *out) {
//     if (col1==row2) {
//         int i,j,k;
//         for (i=0;i<row1;i++) {
//             for (j=0;j<col2;j++) {
//                 out[col2*i+j]=0.0;
//                 for (k=0;k<col1;k++) {
//                     out[col2*i+j]+=m1[col1*i+k]*m2[col2*k+j];
//                 }
//             }
//         }
//         return 0;
//     }
//     return -1;
// }

// /*out =a +bm*/
// int matrix_add(double *a,double *b,int row, int col,double *out,double m) {
//     for (int i=0;i<row;i++) {
//         for (int j=0;j<col;j++) {
//             out[col*i+j]=a[col*i+j]+m*b[col*i+j];
//         }
//     }
//     return 0;
// }

// /*makes a as an identity matrix of size nxn*/
// int my_eye(double *a,int n) {
//     for (int i=0;i<n;i++) {
//         for (int j=0;j<n;j++) {
//             if (i==j) a[n*i+j]=1.0;
//             else a[n*i+j]=0.0;
//         }
//     }
//     return 0;
// }

// /*makes out matrix as transpose of a*/
// int transpose(double *a,int row,int col,double *out) {
//     for (int i=0;i<col;i++) {
//         for (int j=0;j<row;j++) out[row*i+j]=a[col*j+i];
//     }
//     return 0;
// }

// /*return 1 if a>0
// return 0 if a=0
// else return -1*/
// int my_sign(double a) {
//     if (a>0) return 1;
//     else if (a<0) return -1;
//     return 0;
// }

// /*return square of n dimensional vector r */
// double norm_sq(double *r,int n) {
//     double out=0.0;
//     for (int i=0;i<n;i++) out+=r[i]*r[i];
//     return out;
// }

// /*Forms kth matrix in Householder method*/
// int Q_form(double *a,int k,double *out,int n) {
//     int temp=n-k;
//     double *v=(double *)malloc(sizeof(double)*(temp));
//     for (int i=k;i<n;i++) v[i-k]=a[n*i+k];
//     double temp_norm=norm_sq(v,temp);
//     v[0]=v[0]+my_sign(v[0])*sqrt(temp_norm);
//     temp_norm=norm_sq(v,temp);
//     if (my_abs(temp_norm)<=ZERO) return -1;
//     double *F=(double *)malloc(sizeof(double)*temp*temp);
//     double *temp1=(double *)malloc(sizeof(double)*(temp));
//     double *temp2=(double *)malloc(sizeof(double)*temp);
//     for (int i=0;i<temp;i++) temp1[i]=(-2)*v[i];
//     transpose(v,temp,1,temp2);
//     matrix_multiply(temp1,temp,1,temp2,1,temp,F);
//     for (int i=0;i<temp;i++) {
//         for (int j=0;j<temp;j++) F[temp*i+j]/=temp_norm;
//     }
//     for (int i=0;i<temp;i++) F[temp*i+i]+=1;
//     for (int i=0;i<n;i++) {
//         for (int j=0;j<n;j++) {
//             if (i<k) {
//                 if (i==j) out[n*i+j]=1.0;
//                 else out[n*i+j]=0.0;
//             }
//             else {
//                 if (j<k) out[n*i+j]=0.0;
//                 else out[n*i+j]=F[temp*(i-k)+j-k];
//             }
//         }
//     }
//     free(temp1);
//     free(temp2);
//     free(F);
//     free(v);
//     return 0;
// }

// /*Forms H matrix required in heisenberg's algorithm*/
// int H_form(double *a,int k,double *out,int n) {
//     int temp=n-k-1;
//     double *v=(double *)malloc(sizeof(double)*(temp));
//     for (int i=k+1;i<n;i++) v[i-k-1]=a[n*i+k];
//     double temp_norm=norm_sq(v,temp);
//     v[0]=v[0]+my_sign(v[0])*sqrt(temp_norm);
//     temp_norm=norm_sq(v,temp);
//     if (my_abs(temp_norm)<=ZERO) return -1;
//     double *F=(double *)malloc(sizeof(double)*temp*temp);
//     double *temp1=(double *)malloc(sizeof(double)*(temp));
//     double *temp2=(double *)malloc(sizeof(double)*temp);
//     for (int i=0;i<temp;i++) temp1[i]=(-2)*v[i];
//     transpose(v,temp,1,temp2);
//     matrix_multiply(temp1,temp,1,temp2,1,temp,F);
//     for (int i=0;i<temp;i++) {
//         for (int j=0;j<temp;j++) F[temp*i+j]/=temp_norm;
//     }
//     for (int i=0;i<temp;i++) F[temp*i+i]+=1;
//     for (int i=0;i<n;i++) {
//         for (int j=0;j<n;j++) {
//             if (i<=k) {
//                 if (i==j) out[n*i+j]=1.0;
//                 else out[n*i+j]=0.0;
//             }
//             else {
//                 if (j<=k) out[n*i+j]=0.0;
//                 else out[n*i+j]=F[temp*(i-k-1)+j-k-1];
//             }
//         }
//     }
//     free(temp1);
//     free(temp2);
//     free(F);
//     free(v);
//     return 0;
// }

// /*Equate array rhs to lhs*/
// int equate_array(double *lhs,double *rhs,int size) {
//     for (int i=0;i<size;i++) lhs[i]=rhs[i];
//     return 0;
// }

// /*return 1 if a is upper diagonal matrix or lower diagonal matrix
// else return -1*/
// int check(double *a,int n) {
//     int count_low=0;
//     int count_up=0;
//     for (int i=0;i<n;i++) {
//         for (int j=0;j<n;j++) {
//             if (i>j && my_abs(a[n*i+j])<=ZERO) count_low+=1;
//             else if (i<j && my_abs(a[n*i+j])<=ZERO) count_up+=1;
//         }
//     }
//     int temp=n*(n-1)/2;
//     if (count_low==temp || count_up==temp) return 1;
//     return -1; 
// }

// /*does QR decomposition of a of size nxn and stores Q in matrix Q
// return 0 if succeded
// else return -1*/
// int householder(double *a,int n,double *Q) {
//     double *Q0=(double *)malloc(sizeof(double)*n*n);
//     double *ac=(double *)malloc(sizeof(double)*n*n);
//     equate_array(ac,a,n*n);
//     my_eye(Q0,n);
//     for (int k=0;k<n;k++) {
//         double *temp=(double *)malloc(sizeof(double)*n*n);
//         double *temp2=(double *)malloc(sizeof(double)*n*n);
//         if (Q_form(ac,k,temp,n)==-1) return -1;
//         matrix_multiply(temp,n,n,ac,n,n,temp2);
//         equate_array(ac,temp2,n*n);
//         double *temp1=(double *)malloc(sizeof(double)*n*n);
//         transpose(temp,n,n,temp1);
//         matrix_multiply(Q0,n,n,temp1,n,n,Q);
//         equate_array(Q0,Q,n*n);
//         free(temp1);
//         free(temp);
//         free(temp2);
//     }
//     free(Q0);
//     free(ac);
//     return 0;
// }

// /*stores eigen values of a of size nxn in eig_arr array using householder's method
// return number of iterations if succeded
// else return -1*/
// int house_eigenvalues(double *a,int n,double *eig_arr) {
//     double *ac0=(double *)malloc(sizeof(double)*n*n);
//     double *ac=(double *)malloc(sizeof(double)*n*n);
//     equate_array(ac0,a,n*n);
//     int it=0;
//     while (check(ac0,n)!=1) {
//         it+=1;
//         double *Q_ac=(double *)malloc(sizeof(double)*n*n);
//         if (householder(ac0,n,Q_ac)==-1) return -1;
//         double *Q_acT=(double *)malloc(sizeof(double)*n*n);
//         transpose(Q_ac,n,n,Q_acT);
//         double *temp1=(double *)malloc(sizeof(double)*n*n);
//         matrix_multiply(Q_acT,n,n,ac0,n,n,temp1);
//         matrix_multiply(temp1,n,n,Q_ac,n,n,ac);
//         equate_array(ac0,ac,n*n);
//         free(temp1);
//         free(Q_acT);
//         free(Q_ac);
//         if (it==MAX_IT) return -1;
//     }
//     for (int i=0;i<n;i++) eig_arr[i]=ac0[n*i+i]; 
//     free(ac0);
//     free(ac);
//     return it;
// }

// /*Makes out as heisenberg matrix corresponding to matrix a of size nxn
// return 0 if possible
// else return -1*/
// int heisenberg(double *a,int n, double *out) {
//     double *a0=(double *)malloc(sizeof(double)*n*n);
//     equate_array(a0,a,n*n);
//     for (int i=0;i<n-2;i++) {
//         double *H=(double *)malloc(sizeof(double)*n*n);
//         double *temp1=(double *)malloc(sizeof(double)*n*n);
//         double *temp2=(double *)malloc(sizeof(double)*n*n);
//         if (H_form(a0,i,H,n)==-1) return -1;
//         transpose(H,n,n,temp1);
//         matrix_multiply(temp1,n,n,a0,n,n,temp2);
//         matrix_multiply(temp2,n,n,H,n,n,out);
//         equate_array(a0,out,n*n);
//         free(H);
//         free(temp1);
//         free(temp2);
//     }
//     free(a0);
//     return 0;
// }

int main(void) {
    int n;
    printf("Enter the number of rows/columns of matrix:");
    scanf("%d",&n);
    double *a_demo=(double *)malloc(sizeof(double)*n*n);
    double *a=(double *)malloc(sizeof(double)*n*n);
    double* eig=(double *)malloc(sizeof(double)*n);
    printf("Enter the matrix:\n");
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) scanf("%lf",&a_demo[n*i+j]);
    }
    double time_spent=0.0;
    clock_t begin=clock();
    int hes=heisenberg(a_demo,n,a);
    if (hes==-1) {
        int it=egValue_householder(a_demo,n,eig);
        if (it==-1) {
            printf("Complex Eigen Values\n");
            return -1;
        }
        print_matrix(eig,n,1);
        printf("Iterations:%d\n",it);
    }
    else {
        printf("a:\n");
        print_matrix(a,n,n);
        int it=egValue_householder(a,n,eig);
        if (it==-1) {
            printf("Complex Eigen Values\n");
            return -1;
        }
        print_matrix(eig,n,1);
        printf("Iterations:%d\n",it);
    }
    free(a_demo);
    free(a);
    free(eig);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}