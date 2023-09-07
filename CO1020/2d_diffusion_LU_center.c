#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ZERO 1.0e-16
#define CONST 1000

/*return the absolute value of variable x*/
double my_abs(double x) {
    double out=(x>0) ? x:(-1)*x;
    return out;
}
/*A function for calculating column number of the coefficient*/
int col_pos(int n,int i,int j) {
    return(i-1+(n-2)*(j-1));
}

/*return value of gamma corresponding to i,j position of square plate*/
double gama(double len,int i,int j,double dx,double dy) {
    double out=(i*dx-len/2)*(i*dx-len/2)+(j*dy-len/2)*(j*dy-len/2)+CONST;
    return out;
}

/*return partial derivative of gama w.r.t x at position i of square plate*/
double gamax(double len,double dx,int i) {
    return(2*i*dx-len);
}

/*return partial derivative of gama w.r.t y at position j of square plate*/
double gamay(double len,double dy,int i) {
    return(2*i*dy-len);
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
int lu(double *a,int n,double *b,double *x) {
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
    // printf("b\n");
    // for (i=0;i<dim;i++) printf(" %lf\n",b[i]);
    if (lu(a,dim,b,x)==-1) {
        free(x);
        free(b);
        free(a);
        printf("Solution NOT possible\n");
        return -1;
    }
    printf("\n");
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
    printf("X    Y    T\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            double x1=i*dx;
            double y1=j*dy;
            if (i==0) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print 1st column
            else if (i==n-1) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print last column
            else {
                if (j==0) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi0); //Print 1st element of each column
                else if (j==n-1) printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,phi1); //Print last element of each column
                else printf("%0.4lf    %0.4lf    %0.4lf\n",x1,y1,x[col_pos(n,i,j)]); //Print mid elements;
            }
        }
    }
    // }
    free(x);
    free(b);
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}