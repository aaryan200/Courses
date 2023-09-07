#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*return the absolute value of variable x*/
double my_abs(double x);

/*Prints the matrix a with rows=row and columns=col*/
int print_matrix(double *a,int row,int col);

/*Equate array rhs to lhs*/
int equate_array(double *lhs,double *rhs,int size);

/* Input - matrix m1 and its dimensions and matrix m2 and its dimensions and required output matrix
Output - the matrix out becomes the multiplication of matrices m1,m2*/
int matrix_multiply(double *m1,int row1,int col1, double *m2,int row2,int col2, double *out);

/*out =a +bm*/
int matrix_add(double *a,double *b,int row, int col,double *out,double m);

/*makes a as an identity matrix of size nxn*/
int eye(double *a,int n);

/*makes out matrix as transpose of a*/
int transpose(double *a,int row,int col,double *out);

/*return 1 if a>0
return 0 if a=0
else return -1*/
int sign(double a);

/*return square of n dimensional vector r */
double norm_sq(double *r,int n);

/*Adds m times of jth column of A in ith column of A*/
int add_col(double *A,int i,int j,int m,int x,int y,double *out);

/*Adds m times of jth row of A in ith row of A of size x,y*/
int add_row(double *A,int i,int j,int m,int x,int y,double *out);

/*Swaps ith row of A with jth column of A of size x,y*/
int swap_rows(double *A,int i,int j,int x,int y,double *out);

/*Returns the augmented matrix in upper triangular form*/
int gauss_upper(double *A,double *b,int n,double *augmented_matrix);

/*Returns the determinant of A of size nxn*/
double det(double *A,int n);

/*Solution of ux=b where A is an upper diagonal matrix
return 0 if solution is possible (with this method) 
Else return -1*/
int back_substitution(double *u,double *b,int n,double *x);

/*Solution of lx=b where A is a lower diagonal matrix
return 0 if solution is possible (with this method) 
Else return -1*/
int for_substitution(double *l,double *b,int n,double *x);

/*Solves ax=b 
return 0 if solution is possible (with this method) 
Else return -1*/
int gauss_soln(double *a,double *b,int n,double *x);

/*Solves ax=b 
return 0 if solution is possible (with this method) 
Else return -1*/
int LU_soln(double *a,int n,double *b,double *x);

/*checks whether a is diagonally dominant or not
return 0 if yes
else return -1*/
int check_diag_dom(double *a,int n);

/*return 1 if a is upper diagonal matrix or lower diagonal matrix
else return -1*/
int check_upper_lower(double *a,int n);

/*return the norm of residual b-Ax */
double res(double *x,double *b,double *A,int n);

/*return the absolute of difference between present and last residual*/
double res1(double *x0,double *A,double *x,int n);

/*Apply gauss-sidal method to solve system of equations ax=b
return number of iterations if solution is possible (with this method)
else return -1*/
int GS_soln(double *a,double *b,double *x,int n);

/*return the value of d0T*A*d0 where d0T is 1 x n matrix
A is n x n matrix
d0 n x 1 matrix*/
double dAd(double *dT,double *A,double *d,int n);

/*Apply conjugate gradient method to solve system of equations Ax=b
return number of iterations if solution is possible (with this method)
else return -1*/
int ConjGrad_soln(double *A,double *b,double *x,int n);

/*Forms kth matrix in Householder method*/
int Q_form(double *a,int k,double *out,int n);

/*does QR decomposition of a and stores Q in matrix Q
return 0 if succeded
else return -1*/
int QR_householder(double *a,int n,double *Q);

/*stores eigen values of a in eig_arr array using householder's method
return number of iterations if succeded
else return -1*/
int egValue_householder(double *a,int n,double *eig_arr);

/*Forms H matrix required in heisenberg's algorithm*/
int H_form(double *a,int k,double *out,int n);

/*Makes out as heisenberg matrix corresponding to matrix a of size nxn
return 0 if possible
else return -1*/
int heisenberg(double *a,int n, double *out);