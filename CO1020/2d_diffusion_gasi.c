#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ZERO 1.0e-16
#define EPS 1.0e-10
#define CONST 1000

/*return the absolute value of variable x*/
double my_abs(double x) {
    double out=(x>0) ? x:(-1)*x;
    return out;
}

/*return value of gamma corresponding to i,j position of square plate*/
double gama(double len,int i,int j,double dx,double dy) {
    double out=(i*dx-len/2)*(i*dx-len/2)+(j*dy-len/2)*(j*dy-len/2)+CONST;
    return out;
}

/*A function for calculating column number of the coefficient*/
int col_pos(int n,int i,int j) {
    return(i-1+(n-2)*(j-1));
}

/*return partial derivative of gama w.r.t x at position i of square plate*/
double gamax(double len,double dx,int i) {
    return(2*i*dx-len);
}

/*return partial derivative of gama w.r.t y at position j of square plate*/
double gamay(double len,double dy,int i) {
    return(2*i*dy-len);
}

/* Input - matrix m1 and its dimensions and matrix m2 and its dimensions and required output matrix
Output - the matrix out becomes the multiplication of matrices m1,m2*/
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

/*Equate array rhs to lhs*/
int equate_array(double *lhs,double *rhs,int size) {
    for (int i=0;i<size;i++) lhs[i]=rhs[i];
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
int ga_si(double *a,double *b,double *x,int n) {
    if (check_diag_dom(a,n)==-1) return -1;
    double *x0;
    x0=(double *)calloc(n,sizeof(double));
    double residual=res(x0,b,a,n);
    int i,j,it=0;
    double w=1.0;
    printf("Iteration    Residual\n");
    while (residual>EPS) {
        printf("%d    %0.10lf\n",it,residual);
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
    printf("%d    %0.10lf\n",it,residual);
    free(x0);
    return it;
}

int main(void) {
    int n,i,j,k;
    double len;
    //printf("Enter the length 1 side of square plate:");
    scanf("%lf",&len);
    //printf("Enter the number of nodes on 1 side of square plate:");
    scanf("%d",&n);
    double phi0,phi1;
    //printf("Enter the value at 3 sides and 1 other side respectively:");
    scanf("%lf %lf",&phi0,&phi1);
    double time_spent=0.0;
    clock_t begin=clock();
    double dx=len/(n-1);
    double dy=len/(n-1);
    int dim=(n-2)*(n-2);  //dimension of required matrix
    double *a;
    a=(double *)calloc(dim*dim,sizeof(double));
    double *b;
    b=(double *)calloc(dim,sizeof(double));
    for (i=1;i<=n-2;i++) {
        for (j=1;j<=n-2;j++) {
            int row=col_pos(n,i,j);
            double tempx=(-1)*(gamax(len,dx,i))/(2*dx);
            double tempy=(-1)*(gamay(len,dy,j))/(2*dy);
            double tempgama=gama(len,i,j,dx,dy);
            double temp2x=tempgama/((dx*dx));
            double temp2y=tempgama/((dy*dy));
            if (i==1) {
                if (j==1) {
                    b[row]-=(phi0)*(tempx+temp2x);
                    b[row]-=(phi0)*(temp2y+tempy);
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                }
                else if (j==n-2) {
                    b[row]-=(phi0)*(tempx+temp2x);
                    b[row]-=(phi1)*(temp2y-tempy);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                }
                else {
                    b[row]-=(phi0)*(tempx+temp2x);
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                }
            }
            else if (i==n-2) {
                if (j==1) {
                    b[row]-=(phi0)*(temp2x-tempx);
                    b[row]-=(phi0)*(temp2y+tempy);
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                }
                else if (j==n-2) {
                    b[row]-=(phi0)*(temp2x-tempx);
                    b[row]-=(phi1)*(temp2y-tempy);
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                }
                else {
                    b[row]-=(phi0)*(temp2x-tempx);
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                }
            }
            else {
                if (j==1) {
                    b[row]-=(phi0)*(temp2y+tempy);
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                }
                else if (j==n-2) {
                    b[row]-=(phi1)*(temp2y-tempy);
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                }
                else {
                    a[dim*row+col_pos(n,i,j)]=(-2)*(temp2x+temp2y);
                    a[dim*row+col_pos(n,i+1,j)]=temp2x-tempx;
                    a[dim*row+col_pos(n,i-1,j)]=temp2x+tempx;
                    a[dim*row+col_pos(n,i,j-1)]=temp2y+tempy;
                    a[dim*row+col_pos(n,i,j+1)]=temp2y-tempy;
                }
            }
        }
    }
    double *x;  // The matrix of required variables
    x=(double *)calloc(dim,sizeof(double));
    // for (i=0;i<dim;i++) {
    //     for (j=0;j<dim;j++) printf("%lf ",a[dim*i+j]);
    //     printf("\n");
    // }
    // printf("b:\n");
    // for (i=0;i<dim;i++) printf(" %lf\n",b[i]);
    int it=ga_si(a,b,x,dim);
    if (it==-1) {
        free(x);
        free(b);
        free(a);
        printf("Solution NOT possible\n");
        return -1;
    }
    //printf("\n");
    //for (i=0;i<dim;i++) printf("%lf\n",x[i]);
    // printf("The distribution on plate is following:\n\n");
    // for (i=0;i<n;i++) {
    //     for (j=0;j<n;j++) {
    //         if (i==0) printf("%lf ",phi1); // Printing top side
    //         else if (i==n-1) printf("%lf ",phi0); // Printing bottom side
    //         else {
    //             if (j==0) printf("%lf ",phi0); // Printing left side
    //             else if (j==n-1) printf("%lf ",phi0); // Printing right side
    //             else printf("%lf ",x[dim-(i*(n-2))+j-1]); // Printing required values
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("X    Y    T\n");
    // for (i=0;i<n;i++) {
    //     for (j=0;j<n;j++) {
    //         double x1=i*dx;
    //         double y1=j*dy;
    //         if (i==0) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print 1st column
    //         else if (i==n-1) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print last column
    //         else {
    //             if (j==0) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print 1st element of each column
    //             else if (j==n-1) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi1); //Print last element of each column
    //             else printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,x[col_pos(n,i,j)]); //Print mid elements;
    //         }
    //     }
    // }
    // printf("\nNumber of iterations:%d",it);
    free(x);
    free(b);
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    //printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}