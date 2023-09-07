/*This program takes the dimension of a matrix, say A, as input in 1st line and dimension of a matrix, say B, as input in 2nd line and print the matrix AB (if possible) followed by time taken to run the code*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

int main(void) {
    int m,n,s,t,i,j;
    printf("Enter the dimension of 1st matrix:");
    scanf("%d %d",&m,&n);
    printf("Enter the dimension of 2nd matrix:");
    scanf("%d %d",&s,&t);
    double time_spent=0.0;  
    clock_t begin=clock();
    if (n==s) {
        double *m1,*m2;
        m1=(double *)malloc(sizeof(double)*m*n);
        m2=(double *)malloc(sizeof(double)*s*t);
        double *out;
        out=(double *)malloc(sizeof(double)*m*t);
        printf("1st matrix:\n");
        for (i=0;i<m;i++) {
            for (j=0;j<n;j++) {
                scanf("%lf",&m1[n*i+j]);
                //printf("%f ",m1[n*i+j]);
            }
            //printf("\n");
        }
        printf("\n");
        printf("2nd matrix:\n");
        for (i=0;i<s;i++) {
            for (j=0;j<t;j++) {
                scanf("%lf",&m2[t*i+j]);
                //printf("%f ",m2[t*i+j]);
            }
            //printf("\n");
        }
        printf("\n");
        printf("Resultant matrix:\n");
        matrix_multiply(m1,m,n,m2,s,t,m1);
        for (i=0;i<m;i++) {
            for (j=0;j<t;j++) {
                printf("%lf ",m1[t*i+j]);
            }
            printf("\n");
        }
        printf("\n");
        free(m1);
        free(m2);
        free(out);
        clock_t end=clock();
        time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
        //printf("Time taken: %lfs\n",time_spent);
        return 0;
    }
    else {
        printf("Multiplication is not possible\n");
        clock_t end=clock();
        time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time taken: %lfs\n",time_spent);
        return 0;
    }
    return 0;
}