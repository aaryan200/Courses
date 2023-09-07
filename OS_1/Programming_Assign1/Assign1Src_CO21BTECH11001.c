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
    int num_lines = 1; // number of lines in input.txt
    if (input == NULL)
    {
        printf("No input file\n");
        return 0;
    }
    int n, k;
    int ct = 0;         // counts the number of inputs
    char main_name[N];  // Name of log file of main process
    char child_name[N]; // Name of log file of child process
    char directory[N];  // Name of directory
    while (ct < num_lines && fscanf(input, "%d %d", &n, &k) == 2)
    {
        ct++;
        printf("%d\n", ct);
        sprintf(directory, "Input_%d", ct);
        if (mkdir(directory, 0777) == -1)
        {
            printf("%s directory already created\n", directory);
            return 0;
        }
        sprintf(main_name, "%s/Main_process.log", directory);
        int divide = n / k; // Maximum numbers that will be assigned to a chlid process
        int *num_from_start = (int *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        // Starting number assigned to a process
        int *num_from_end = (int *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        // Number from end assigned to a process
        *num_from_start = 1;
        *num_from_end = n;
        FILE **files; // Array of file pointers, Total files generated: k+1
        files = (FILE **)malloc(sizeof(FILE *) * (k + 1));
        for (int i = 0; i < k + 1; i++)
        {
            if (i == 0)
            {
                files[0] = fopen(main_name, "a+");
            }
            else
            {
                sprintf(child_name, "%s/Child_process_%d.log", directory, i);
                files[i] = fopen(child_name, "a+");
            }
        }
        // Creating the child processes
        for (int i = 0; i < k; i++)
        {
            if (fork() == 0)
            {
                fprintf(files[0], "P%d: ", i + 1); // Printing in log file of main process
                if (i != k - 1)
                {
                    /*All processes except the last will be assigned a maximum of (divide) numbers*/
                    int count = 0;
                    while (count < divide)
                    {
                        if (count % 2 == 0)
                        {
                            if (check_perfect(*num_from_start))
                            {
                                fprintf(files[0], "%d ", *num_from_start);
                                fprintf(files[i + 1], "%d: Is a perfect number\n", *num_from_start);
                                // Printing in log file of child process
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
                    /*Rest of numbers will be assigned to last process.*/
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
        // Parent process will wait until child process terminates.
        for (int i = 0; i < k; i++)
            wait(NULL);

        // Close all files
        for (int i = 0; i < k + 1; i++)
        {
            fclose(files[i]);
        }
        free(files);
    }
    fclose(input);
    return 0;
}