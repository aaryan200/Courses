#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ZERO 1.0e-16
#define EPS 1.0e-10
#define MAX_IT 5.0e6

/*return the absolute value of variable x*/
double my_abs(double x) {
    double out=(x>0) ? x:(-1)*x;
    return out;
}

/*Prints the matrix a with rows=row and columns=col*/
int print_matrix(double *a,int row,int col) {
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) printf("%lf ",a[col*i+j]);
        printf("\n");
    }
    printf("\n");
    return 0;
}

/*Equate array rhs to lhs*/
int equate_array(double *lhs,double *rhs,int size) {
    for (int i=0;i<size;i++) lhs[i]=rhs[i];
    return 0;
}

/* Input - matrix m1 and its dimensions and matrix m2 and its dimensions and required output matrix
Output - the matrix out becomes the multiplication of matrices m1,m2*/
int matrix_multiply(double *m1,int row1,int col1, double *m2,int row2,int col2, double *out) {
    if (col1==row2) {
        double *temp=(double *)malloc(sizeof(double)*row1*col2);
        int i,j,k;
        for (i=0;i<row1;i++) {
            for (j=0;j<col2;j++) {
                temp[col2*i+j]=0.0;
                for (k=0;k<col1;k++) {
                    temp[col2*i+j]+=m1[col1*i+k]*m2[col2*k+j];
                }
            }
        }
        for (i=0;i<row1;i++) {
            for (j=0;j<col2;j++) out[col2*i+j]=temp[col2*i+j];
        }
        free(temp);
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

/*makes a as an identity matrix of size nxn*/
int eye(double *a,int n) {
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i==j) a[n*i+j]=1.0;
            else a[n*i+j]=0.0;
        }
    }
    return 0;
}

/*makes out matrix as transpose of a*/
int transpose(double *a,int row,int col,double *out) {
    for (int i=0;i<col;i++) {
        for (int j=0;j<row;j++) out[row*i+j]=a[col*j+i];
    }
    return 0;
}

/*return 1 if a>0
return 0 if a=0
else return -1*/
int sign(double a) {
    if (a>0) return 1;
    else if (a<0) return -1;
    return 0;
}

/*return square of n dimensional vector r */
double norm_sq(double *r,int n) {
    double out=0.0;
    for (int i=0;i<n;i++) out+=r[i]*r[i];
    return out;
}

/*Adds m times of jth column of A in ith column of A*/
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

/*Swaps ith row of A with jth column of A of size x,y*/
int swap_rows(double *A,int i,int j,int x,int y,double *out) {
    // i-> j, j-> i
    int row,col;
    for (row =0 ;row<x;row++) {
        if (row != i && row != j) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*row+col];
        }
        else if (row==i) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*j+col];
        }
        else if (row==j) {
            for (col=0;col<y;col++) out[y*row+col]=A[y*i+col];
        }
    }
    return 0;
}

/*Returns the augmented matrix in upper triangular form*/
int gauss_upper(double *A,double *b,int n,double *augmented_matrix) {
    int i,j,k;
    for (int col=0;col<n+1;col++) {
        if (col<n) {
            for (int row=0;row<n;row++) augmented_matrix[(n+1)*row+col]=A[n*row+col];
        }
        else {
            for (int row=0;row<n;row++) augmented_matrix[(n+1)*row+col]=b[row];
        }
    }

    for (i=0;i<n;i++) {
        if (abs(augmented_matrix[(n+1)*i+i])<=1.0e-16) {
            int x=0;
            for (int row=i+1;row<n;row++) {
                if (augmented_matrix[(n+1)*row+i]!=0) {
                    add_row(augmented_matrix,i,row,1,n+1,n,augmented_matrix);
                    break;
                }
                else x+=1;
            }
            if (x==n-1-i) continue;
            else {
                for (int row=i+1;row<n;row++) {
                    double r=augmented_matrix[(n+1)*row+i]/augmented_matrix[(n+1)*i+i];
                    for (int col=n;col>i-1;col-=1) {
                        augmented_matrix[(n+1)*row+col]-=r*augmented_matrix[(n+1)*i+col];
                    }
                }
            }
        }
        else {
            for (int row=i+1;row<n;row++) {
                double r=augmented_matrix[(n+1)*row+i]/augmented_matrix[(n+1)*i+i];
                for (int col=n;col>i-1;col-=1) {
                    augmented_matrix[(n+1)*row+col]-=r*augmented_matrix[(n+1)*i+col];
                }
            }
        }
    }
    return 0;
}

/*Returns the determinant of A of size nxn*/
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

/*Solution of ux=b where A is an upper diagonal matrix
return 0 if solution is possible (with this method) 
Else return -1*/
int back_substitution(double *u,double *b,int n,double *x) {
    int i,j,k,y=0;
    for (i=0;i<n;i++) {
        if (my_abs(u[n*i+i])<=ZERO) {
            y+=1;
            break;
        }
    }
    if (y>0) {
        return -1;
    }
    else {
        x[n-1]=b[n-1]/u[n*n-1];
        for (i=n-2;i>=0;i=i-1) {
            double sum=0;
            for (j=i+1;j<n;j++) sum+=u[n*i+j]*x[j];
            x[i]=(b[i]-sum)/u[n*i+i];
        }
    }
    return 0;
}

/*Solution of lx=b where A is a lower diagonal matrix
return 0 if solution is possible (with this method) 
Else return -1*/
int for_substitution(double *l,double *b,int n,double *x) {
    int i,j,k,y=0;
    for (i=0;i<n;i++) {
        if (my_abs(l[n*i+i])<=ZERO) {
            y+=1;
            break;
        }
    }
    if (y>0) {
        return -1;
    }
    else {
        x[0]=b[0]/l[0];
        for (i=1;i<n;i++) {
            double sum=0;
            for (j=0;j<i;j++) sum+=l[n*i+j]*x[j];
            x[i]=(b[i]-sum)/l[n*i+i];
        }
    }
    return 0;
}

/*Solves ax=b 
return 0 if solution is possible (with this method) 
Else return -1*/
int gauss_soln(double *a,double *b,int n,double *x) {
    double *aug_matrix;
    aug_matrix=(double *)malloc((n+1)*n*sizeof(double));
    gauss_upper(a,b,n,aug_matrix);
    double *Ac;
    Ac=(double *)calloc(n*n,sizeof(double));
    double *bc;
    bc=(double *)calloc(n,sizeof(double));
    for (int col=0;col<n+1;col++) {
        if (col<n) {
            for (int row=0;row<n;row++) Ac[n*row+col]=aug_matrix[(n+1)*row+col];
        }
        if (col==n) {
            for (int row=0;row<n;row++) bc[row]=aug_matrix[(n+1)*row+col];
        }
    }
    if (back_substitution(Ac,bc,n,x)==-1) return -1;
    free(aug_matrix);
    free(Ac);
    free(bc);
}

/*Solves ax=b 
return 0 if solution is possible (with this method) 
Else return -1*/
int LU_soln(double *a,int n,double *b,double *x) {
    int k,i,j;
    double *l;
    double *u;
    l=(double *)calloc(n*n,sizeof(double));
    u=(double *)calloc(n*n,sizeof(double));
    for (i=0;i<n;i++) l[n*i+i]=1.0;
    for (j=0;j<n;j++) {
        for (i=0;i<=j;i++) {
            if (i!=j) l[n*i+j]=0.0;
            double sum=a[n*i+j];
            for (k=0;k<i;k++) sum-=l[n*i+k]*u[n*k+j];
            u[n*i+j]=sum;
        }
        for (i=j+1;i<n;i++) {
            u[n*i+j]=0.0;
            if (my_abs(u[n*j+j])<=ZERO) {
                return -1;
            }
            else {
                double sum=a[n*i+j];
                for (k=0;k<j;k++) sum-=l[n*i+k]*u[n*k+j];
                l[n*i+j]=sum/u[n*j+j];
            }
        }
    }
    double *y;
    y=(double *)malloc(sizeof(double)*n);
    if (for_substitution(l,b,n,y)==-1) return -1;
    if (back_substitution(u,y,n,x)==-1) return -1;
    free(l);
    free(u);
    free(y);
    return 0;
}

/*checks whether a is diagonally dominant or not
return 0 if yes
else return -1*/
int check_diag_dom(double *a,int n) {
    int i,j,count=0;
    for (i=0;i<n;i++) {
        int x=0;
        double m=my_abs(a[n*i+i]);
        for (j=0;j<n;j++) {
            if (j!=i && m>my_abs(a[n*i+j])) x+=1;
        }
        if (x==n-1) count+=1;
    }
    if (count==n) return 1;
    return -1;
}

/*return 1 if a is upper diagonal matrix or lower diagonal matrix
else return -1*/
int check_upper_lower(double *a,int n) {
    int count_low=0;
    int count_up=0;
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i>j && my_abs(a[n*i+j])<=ZERO) count_low+=1;
            else if (i<j && my_abs(a[n*i+j])<=ZERO) count_up+=1;
        }
    }
    int temp=n*(n-1)/2;
    if (count_low==temp || count_up==temp) return 1;
    return -1; 
}

/*return the norm of residual b-Ax */
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

/*return the absolute of difference between present and last residual*/
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

/*Apply gauss-sidal method to solve system of equations ax=b
return number of iterations if solution is possible (with this method)
else return -1*/
int GS_soln(double *a,double *b,double *x,int n) {
    if (check_diag_dom(a,n)==-1) return -1;
    double *x0;
    x0=(double *)calloc(n,sizeof(double));
    double residual=res(x0,b,a,n);
    int i,j,it=0;
    double w=1.0;
    // printf("Iteration    Residual\n");
    while (residual>EPS) {
        // printf("%d    %0.10lf\n",it,residual);
        for (i=0;i<n;i++) {
            double sum=b[i];
            for (j=0;j<n;j++) {
                if (j!=i) sum-=a[n*i+j]*x[j];
            }
            if (my_abs(a[n*i+i])<=ZERO) return -1;
            x[i]=(1-w)*x0[i]+w*sum/a[n*i+i];
        }
        double temp=res1(x0,a,x,n);
        if (temp<residual) w=1.9;
        residual=temp;
        equate_array(x0,x,n);
        it+=1;
    }
    // printf("%d    %0.10lf\n",it,residual);
    free(x0);
    return it;
}

/*return the value of d0T*A*d0 where d0T is 1 x n matrix
A is n x n matrix
d0 n x 1 matrix*/
double dAd(double *dT,double *A,double *d,int n) {
    double *temp;
    temp=(double *)malloc(n*sizeof(double));
    matrix_multiply(dT,1,n,A,n,n,temp);
    double *out;
    out=(double *)malloc(sizeof(double));
    matrix_multiply(temp,1,n,d,n,1,out);
    double a;
    a=out[0];
    free(out);
    free(temp);
    return a;
}

/*Apply conjugate gradient method to solve system of equations Ax=b
return number of iterations if solution is possible (with this method)
else return -1*/
int ConjGrad_soln(double *A,double *b,double *x,int n) {
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
    // printf("Iteration    Residual\n");
    int count=0;
    while (s>EPS) {
        double t=dAd(d0,A,d0,n);
        // printf("%d    %0.10lf\n",count,s);
        if (my_abs(t)<=ZERO) break;
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
        s=norm_sq(r,n);
        free(Ad0);
        count+=1;
    }
    // printf("%d    %0.10lf\n",count,s);
    free(temp);
    free(r0);
    free(r);
    free(x0);
    free(d);
    free(d0);
    return count;
}

/*Forms kth matrix in Householder method*/
int Q_form(double *a,int k,double *out,int n) {
    int temp=n-k;
    double *v=(double *)malloc(sizeof(double)*(temp));
    for (int i=k;i<n;i++) v[i-k]=a[n*i+k];
    double temp_norm=norm_sq(v,temp);
    v[0]=v[0]+sign(v[0])*sqrt(temp_norm);
    temp_norm=norm_sq(v,temp);
    if (my_abs(temp_norm)<=ZERO) return -1;
    double *F=(double *)malloc(sizeof(double)*temp*temp);
    double *temp1=(double *)malloc(sizeof(double)*(temp));
    double *temp2=(double *)malloc(sizeof(double)*temp);
    for (int i=0;i<temp;i++) temp1[i]=(-2)*v[i];
    transpose(v,temp,1,temp2);
    matrix_multiply(temp1,temp,1,temp2,1,temp,F);
    for (int i=0;i<temp;i++) {
        for (int j=0;j<temp;j++) F[temp*i+j]/=temp_norm;
    }
    for (int i=0;i<temp;i++) F[temp*i+i]+=1;
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

/*does QR decomposition of a and stores Q in matrix Q
return 0 if succeded
else return -1*/
int QR_householder(double *a,int n,double *Q) {
    double *Q0=(double *)malloc(sizeof(double)*n*n);
    double *ac=(double *)malloc(sizeof(double)*n*n);
    equate_array(ac,a,n*n);
    eye(Q0,n);
    for (int k=0;k<n;k++) {
        double *temp=(double *)malloc(sizeof(double)*n*n);
        if (Q_form(ac,k,temp,n)==-1) return -1;
        matrix_multiply(temp,n,n,ac,n,n,ac);
        double *temp1=(double *)malloc(sizeof(double)*n*n);
        transpose(temp,n,n,temp1);
        matrix_multiply(Q0,n,n,temp1,n,n,Q0);
        free(temp1);
        free(temp);
    }
    equate_array(Q,Q0,n*n);
    free(Q0);
    free(ac);
    return 0;
}

/*stores eigen values of a in eig_arr array using householder's method
return number of iterations if succeded
else return -1*/
int egValue_householder(double *a,int n,double *eig_arr) {
    double *ac0=(double *)malloc(sizeof(double)*n*n);
    double *ac=(double *)malloc(sizeof(double)*n*n);
    equate_array(ac0,a,n*n);
    int it=0;
    while (check_upper_lower(ac0,n)!=1) {
        it+=1;
        double *Q_ac=(double *)malloc(sizeof(double)*n*n);
        if (QR_householder(ac0,n,Q_ac)==-1) return -1;
        double *Q_acT=(double *)malloc(sizeof(double)*n*n);
        transpose(Q_ac,n,n,Q_acT);
        double *temp1=(double *)malloc(sizeof(double)*n*n);
        matrix_multiply(Q_acT,n,n,ac0,n,n,temp1);
        matrix_multiply(temp1,n,n,Q_ac,n,n,ac);
        equate_array(ac0,ac,n*n);
        free(temp1);
        free(Q_acT);
        free(Q_ac);
        if (it==MAX_IT) return -1;
    }
    for (int i=0;i<n;i++) eig_arr[i]=ac0[n*i+i]; 
    free(ac0);
    free(ac);
    return it;
}

/*Forms H matrix required in heisenberg's algorithm*/
int H_form(double *a,int k,double *out,int n) {
    int temp=n-k-1;
    double *v=(double *)malloc(sizeof(double)*(temp));
    for (int i=k+1;i<n;i++) v[i-k-1]=a[n*i+k];
    double temp_norm=norm_sq(v,temp);
    v[0]=v[0]+sign(v[0])*sqrt(temp_norm);
    temp_norm=norm_sq(v,temp);
    if (my_abs(temp_norm)<=ZERO) return -1;
    double *F=(double *)malloc(sizeof(double)*temp*temp);
    double *temp1=(double *)malloc(sizeof(double)*(temp));
    double *temp2=(double *)malloc(sizeof(double)*temp);
    for (int i=0;i<temp;i++) temp1[i]=(-2)*v[i];
    transpose(v,temp,1,temp2);
    matrix_multiply(temp1,temp,1,temp2,1,temp,F);
    for (int i=0;i<temp;i++) {
        for (int j=0;j<temp;j++) F[temp*i+j]/=temp_norm;
    }
    for (int i=0;i<temp;i++) F[temp*i+i]+=1;
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i<=k) {
                if (i==j) out[n*i+j]=1.0;
                else out[n*i+j]=0.0;
            }
            else {
                if (j<=k) out[n*i+j]=0.0;
                else out[n*i+j]=F[temp*(i-k-1)+j-k-1];
            }
        }
    }
    free(temp1);
    free(temp2);
    free(F);
    free(v);
    return 0;
}

/*Makes out as heisenberg matrix corresponding to matrix a of size nxn
return 0 if possible
else return -1*/
int heisenberg(double *a,int n, double *out) {
    double *a0=(double *)malloc(sizeof(double)*n*n);
    equate_array(a0,a,n*n);
    for (int i=0;i<n-2;i++) {
        double *H=(double *)malloc(sizeof(double)*n*n);
        double *temp1=(double *)malloc(sizeof(double)*n*n);
        double *temp2=(double *)malloc(sizeof(double)*n*n);
        if (H_form(a0,i,H,n)==-1) return -1;
        transpose(H,n,n,temp1);
        matrix_multiply(temp1,n,n,a0,n,n,temp2);
        matrix_multiply(temp2,n,n,H,n,n,out);
        equate_array(a0,out,n*n);
        free(H);
        free(temp1);
        free(temp2);
    }
    free(a0);
    return 0;
}