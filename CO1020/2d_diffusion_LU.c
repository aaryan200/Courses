#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int col_pos(int n,int i,int j) {
    return(i-1+(n-2)*(j-1));
}


/*Returns the solution of ux=b where A is an upper diagonal matrix*/
int back_substitution(double *u,double *b,int n,double *x) {
    int i,j,k,y=0;
    for (i=0;i<n;i++) {
        if (abs(u[n*i+i])<=1.0e-16) {
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

/*Returns the solution of lx=b where A is a lower diagonal matrix*/
int for_substitution(double *l,double *b,int n,double *x) {
    int i,j,k,y=0;
    for (i=0;i<n;i++) {
        if (abs(l[n*i+i])<=1.0e-16) {
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
            if (abs(u[n*j+j])<=1.0e-12) {
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
    if (lu(a,dim,b,x)==-1) {
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
    free(x);
    free(b);
    free(a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken: %lfs\n",time_spent);
    return 0;
}