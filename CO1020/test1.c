#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int col_pos(int n,int i,int j) {
    return(i-1+(n-2)*(j-1));
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

int ga_si(double *a,double *b,double *x,int n,double tol) {
    if (check_diag_dom(a,n)==-1) return -1;
    double *x0;
    x0=(double *)calloc(n,sizeof(double));
    double residual=res(x0,b,a,n);
    int i,j,it=0;
    double w=1.0;
    while (residual>tol) {
        for (i=0;i<n;i++) {
            double sum=b[i];
            for (j=0;j<n;j++) {
                if (j!=i) sum-=a[n*i+j]*x[j];
            }
            if (abs(a[n*i+i])<=1.0e-12) return -1;
            x[i]=(1-w)*x0[i]+w*sum/a[n*i+i];
        }
        double temp=res1(x0,a,x,n);
        if (temp<residual) w=1.9;
        residual=temp;
        equate_array(x0,x,n);
        it+=1;
    }
    free(x0);
    return it;
}

int main(void) {
    int n,i,j,k;
    printf("Enter the number of nodes on 1 side of square plate:");
    scanf("%d",&n);
    double phi0,phi1;
    printf("Enter the value at 3 sides and 1 other side respectively:");
    scanf("%lf %lf",&phi0,&phi1);
    double time_spent=0.0;
    clock_t begin=clock();
    int dim=(n-2)*(n-2);
    double *a;
    a=(double *)calloc(dim*dim,sizeof(double));
    double *b;
    b=(double *)calloc(dim,sizeof(double));
    for (i=1;i<=n-2;i++) {
        for (j=1;j<=n-2;j++) {
            if (i==1) {
                if (j==1) {
                    int row=col_pos(n,i,j);
                    b[row]=(-2)*phi0;
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
                else if (j==n-2) {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi0*dim)-(phi1*(n-2+dim+1));
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                }
                else {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi0*j*j);
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
            }
            else if (i==n-2) {
                if (j==1) {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi0*dim)-(phi0*(dim+n-1));
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
                else if (j==n-2) {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi1*(n-2+(2*dim)))-(phi0*(n-2+(2*dim)));
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                }
                else {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi0*(n-2+dim+j*j));
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
            }
            else {
                if (j==1) {
                    int row=col_pos(n,i,j);
                    b[row]=((-1)*phi0*i*i);
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
                else if (j==n-2) {
                    int row=col_pos(n,i,j);
                    b[row]=(-1)*phi1*(n-2+i*i+dim);
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                }
                else {
                    int row=col_pos(n,i,j);
                    a[dim*row+row]=(-4)*(i*i+j*j);
                    a[dim*row+col_pos(n,i+1,j)]=i+i*i+j*j;
                    a[dim*row+col_pos(n,i-1,j)]=i*i+j*j-i;
                    a[dim*row+col_pos(n,i,j-1)]=i*i+j*j-j;
                    a[dim*row+col_pos(n,i,j+1)]=j+i*i+j*j;
                }
            }
        }
    }
    // for (i=0;i<dim;i++) {
    //     for (j=0;j<dim;j++) printf("%lf ",a[dim*i+j]);
    //     printf("\n");
    // }
    // printf("b\n");
    // for (i=0;i<dim;i++) printf(" %lf\n",b[i]);
    double *x;
    x=(double *)calloc(dim,sizeof(double));
    int it=ga_si(a,b,x,dim,1.0e-10);
    if (it==-1) {
        printf("Solution NOT possible\n");
        return -1;
    }
    printf("\n");
    //for (i=0;i<dim;i++) printf("%lf\n",x[i]);
    printf("The distribution on plate is following:\n\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            if (i==0) printf("%lf ",phi1);
            else if (i==n-1) printf("%lf ",phi0);
            else {
                if (j==0) printf("%lf ",phi0);
                else if (j==n-1) printf("%lf ",phi0);
                else printf("%lf ",x[dim-(i*(n-2))+j-1]);
            }
        }
        printf("\n");
    }
    printf("\nNumber of iterations:%d",it);
    free(x);
    free(b);
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}