#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

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

/*Adds m times of jth row of A in ith row of A*/
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

/*Swaps ith row of A with jth column of A*/
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

/*Returns the solution of Ax=b where A is an upper diagonal matrix*/
int back_substitution(double *A,double *b,int n,double *x) {
    int i,j,k,y=0;
    for (i=0;i<n;i++) {
        if (abs(A[n*i+i])<=1.0e-16) {
            y+=1;
            break;
        }
    }
    if (y>0) {
        printf("Infinite Solution\n");
        return -1;
    }
    else {
        x[n-1]=b[n-1]/A[n*n-1];
        for (i=n-2;i>=0;i=i-1) {
            double sum=0;
            for (j=i+1;j<n;j++) sum+=A[n*i+j]*x[j];
            x[i]=(b[i]-sum)/A[n*i+i];
        }
    }
    return 0;
}

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

int main(void) {
    int n;
    printf("Enter the number of variables: ");
    scanf("%d",&n);
    double time_spent=0.0;  
    clock_t begin=clock();
    double *A;
    A=(double *)calloc(n*n,sizeof(double));
    int i,j;
    printf("A:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++){
            // A[n*i+j]=rand()%100;
            // printf("%lf ",A[n*i+j]);
            scanf("%lf",&A[n*i+j]);
        }
        // printf("\n");
    }
    printf("\n");
    double *b;
    b=(double *)calloc(n,sizeof(double));
    printf("b:\n");
    for (i=0;i<n;i++) {
        // b[i]=rand()%100;
        // printf("%lf\n",b[i]);
        scanf("%lf",&b[i]);
    }
    printf("\n");
    double *x;
    x=(double *)calloc(n,sizeof(double));
    // double *aug_matrix;
    // aug_matrix=(double *)malloc((n+1)*n*sizeof(double));
    // gauss_upper(A,b,n,aug_matrix);
    // double *Ac;
    // Ac=(double *)calloc(n*n,sizeof(double));
    // double *bc;
    // bc=(double *)calloc(n,sizeof(double));
    // for (int col=0;col<n+1;col++) {
    //     if (col<n) {
    //         for (int row=0;row<n;row++) Ac[n*row+col]=aug_matrix[(n+1)*row+col];
    //     }
    //     if (col==n) {
    //         for (int row=0;row<n;row++) bc[row]=aug_matrix[(n+1)*row+col];
    //     }
    // }
    // if (back_substitution(Ac,bc,n,x)!=-1) {
    //     printf("\nSolutions of Ax=b are:\n");
    //     for (int i=0;i<n;i++) printf("x%d = %lf\n",i+1,x[i]);
    //     printf("\n");
    // }
    // free(aug_matrix);
    // free(Ac);
    // free(bc);
    if (gauss_soln(A,b,n,x)==-1) return -1;
    for (int i=0;i<n;i++) printf("x%d = %lf\n",i+1,x[i]);
    free(x);
    free(b);
    free(A);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    //printf("Time taken: %lfs\n",time_spent);
    return 0;
}