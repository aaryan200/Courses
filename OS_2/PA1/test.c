#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>

int main()
{
    srand((unsigned int)time(NULL));

    int a;
    scanf("%d",&a);
    double time_spent=0.0;
    clock_t begin=clock();
    for (int i=0;i<10;i++)
        printf("%f\n", ((float)rand()/(float)(RAND_MAX)) * (float)a);
    clock_t end=clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime: %lf \xC2\xB5s\n",time_spent);
    return 0;
}