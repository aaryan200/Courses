#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define EPS 1.0e-12

int col_pos(int n,int i,int j) {
    return(i-1+(n-2)*(j-1));
}

double gamax(double len,double dx,int i) {
    return(2*i*dx-len);
}

double gamay(double len,double dy,int i) {
    return(2*i*dy-len);
}

double gama(double len,int i,int j,double dx,double dy) {
    double out=(i*dx-len/2)*(i*dx-len/2)+(j*dy-len/2)*(j*dy-len/2)+len;
    return out;
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

int equate_array(double *lhs,double *rhs,int size) {
    for (int i=0;i<size;i++) lhs[i]=rhs[i];
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

double norm(double *r,int n) {
    double out=0;
    for (int i=0;i<n;i++) out+=r[i]*r[i];
    return out;
}

/*r -> r0-s*A*d*/
int eq_3(double *r,double *r0,double *A,double *d,double s,int n) {
    double *temp=(double *)calloc(n,sizeof(double));
    matrix_multiply(A,n,n,d,n,1,temp);
    for (int i=0;i<n;i++) r[i]=r0[i]-s*temp[i];
    free(temp);
    return 0;
}

/*dT*A*d*/
double dAd(double *A,double *d0,int n) {
    double *temp;
    temp=(double *)malloc(n*sizeof(double));
    matrix_multiply(d0,1,n,A,n,n,temp);
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
    if (check_diag_dom(A,n)==-1){
        return -1;
    }
    double *r0,*r,*x0,*d;
    r0=(double *)malloc(n*sizeof(double));
    r=(double *)malloc(n*sizeof(double));
    x0=(double *)calloc(n,sizeof(double));
    d=(double *)malloc(n*sizeof(double));
    eq_3(r0,b,A,x0,1,n);
    equate_array(d,r0,n);
    double no_r0=norm(r0,n);
    int i,count=0;
    while (no_r0>EPS) {
        double temp=dAd(A,d,n);
        if (abs(temp)<=1.0e-15) break;
        double s=no_r0/temp;
        for (i=0;i<n;i++) x[i]=x0[i]+s*d[i];
        eq_3(r,r0,A,d,s,n);
        double no_r=norm(r,n);
        if (no_r<=EPS) break;
        double c=no_r/no_r0;
        for (i=0;i<n;i++) d[i]=r[i]+c*d[i];
        equate_array(r0,r,n);
        equate_array(x0,x,n);
        no_r0=norm(r0,n);
        count+=1;
    }
    free(r0);
    free(r);
    free(x0);
    free(d);
    return count;
}

int main(void) {
    int n,i,j,k;
    double len;
    printf("Enter the length 1 side of square plate:");
    scanf("%lf",&len);
    printf("Enter the number of nodes on 1 side of square plate:");
    scanf("%d",&n);
    double phi0,phi1;
    printf("Enter the value at 3 sides and 1 other side respectively:");
    scanf("%lf %lf",&phi0,&phi1);
    double time_spent=0.0;
    clock_t begin=clock();
    double dx=len/(n-1);
    double dy=len/(n-1);
    int dim=(n-2)*(n-2);
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
    double *x;
    x=(double *)calloc(dim,sizeof(double));
    // for (i=0;i<dim;i++) {
    //     for (j=0;j<dim;j++) printf("%lf ",a[dim*i+j]);
    //     printf("\n");
    // }
    // printf("b:\n");
    // for (i=0;i<dim;i++) printf(" %lf\n",b[i]);
    int it=conj_grad(a,b,x,dim);
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