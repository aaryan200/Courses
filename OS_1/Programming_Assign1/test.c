/*Aaryan , CO21BTECH11001*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#define M 10
#define N 100
#define SIZE 64

/*Return true if num is a perfect number
else return false*/
bool check_perfect(int num)
{
    if (num <= 1)
        return false;
    int sum_of_factors = 1; // 1 is a trivial factor
    for (int i = 2; i < num; i++)
    {
        if (num % i == 0)
            sum_of_factors += i;
    }
    if (sum_of_factors == num)
        return true;
    return false;
}

int main()
{
    /*read inputs n and k from input.txt*/
    FILE *input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("No input file\n");
        return 0;
    }
    char n_inp[M];
    char k_inp[M];
    int ct = 0;
    char main_name[N];
    char child_name[N];
    char input_number[N];
    while (ct<10 && fscanf(input, "%s %s", n_inp, k_inp) == 2)
    {
        ct++;
        sprintf(input_number, "Input%d", ct);
        if (mkdir(input_number, 0777) == -1)
        {
            printf("%s directory already created\n", input_number);
            return 0;
        }
        sprintf(main_name, "%s/Main_process.log", input_number);
        int n = atoi(n_inp), k = atoi(k_inp);
        int divide = n / k; 
        printf("ct=%d n=%d k=%d divide=%d\n", ct, n, k,divide);
        // int fd = shm_open(name,O_CREAT | O_RDWR,0666);
        // ftruncate(fd,SIZE);
        int *num_from_start = (int*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        int *num_from_end = (int*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *num_from_start = 1;
        *num_from_end = n;
        FILE **files;
        files = (FILE **)malloc(sizeof(FILE *) * (k + 1));
        for (int i = 0; i < k + 1; i++)
        {
            if (i == 0)
            {
                files[0] = fopen(main_name, "a+");
            }
            else
            {
                sprintf(child_name, "%s/Child_process_%d.log", input_number, i);
                files[i] = fopen(child_name, "a+");
            }
        }
        for (int i = 0; i < k; i++)
        {
            if (fork() == 0)
            {
                printf("start=%d end=%d\n",*num_from_start,*num_from_end);
                fprintf(files[0], "P%d: ", i + 1);
                if (i != k - 1)
                {
                    int count = 0;
                    while (count < divide)
                    {
                        if (count % 2 == 0)
                        {
                            if (check_perfect(*num_from_start))
                            {
                                fprintf(files[0], "%d ", *num_from_start);
                                fprintf(files[i + 1], "%d: Is a perfect number\n", *num_from_start);
                            }
                            else
                            {
                                fprintf(files[i + 1], "%d: Not a perfect number\n", *num_from_start);
                            }
                            (*num_from_start)++;
                        }
                        else
                        {
                            if (check_perfect(*num_from_end))
                            {
                                fprintf(files[0], "%d ", *num_from_end);
                                fprintf(files[i + 1], "%d: Is a perfect number\n", *num_from_end);
                            }
                            else
                            {
                                fprintf(files[i + 1], "%d: Not a perfect number\n", *num_from_end);
                            }
                            (*num_from_end)--;
                        }
                        count++;
                    }
                }
                else
                {
                    int count = 0;
                    while (*num_from_start < *num_from_end)
                    {
                        if (count % 2 == 0)
                        {
                            if (check_perfect(*num_from_start))
                            {
                                fprintf(files[0], "%d ", *num_from_start);
                                fprintf(files[i + 1], "%d: Is a perfect number\n", *num_from_start);
                            }
                            else
                            {
                                fprintf(files[i + 1], "%d: Not a perfect number\n", *num_from_start);
                            }
                            (*num_from_start)++;
                        }
                        else
                        {
                            if (check_perfect(*num_from_end))
                            {
                                fprintf(files[0], "%d ", *num_from_end);
                                fprintf(files[i + 1], "%d: Is a perfect number\n", *num_from_end);
                            }
                            else
                            {
                                fprintf(files[i + 1], "%d: Not a perfect number\n", *num_from_end);
                            }
                            (*num_from_end)--;
                        }
                        count++;
                    }
                }
                fprintf(files[0], "\n");
                exit(0);
            }
        }
        for (int i = 0; i < k; i++)
            wait(NULL);
    }

    return 0;
}