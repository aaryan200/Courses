/*
C++ program to check if a sudoku is valid using mulithreading with pthreads
Author: Aaryan
Roll number: CO21BTECH11001
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#define M 1.0e6

int N, n, k;
int **sudoku;
pthread_t *threads;

/*A structure to pass arguments to a thread*/
struct thread_args
{
    int **nums;
    int *count;
    bool *check;
    int size;
    char *type;
};

/*Runner function of thread*/
void *runner(void *args)
{
    struct thread_args *ptr = (struct thread_args *)args;

    int *arr;
    arr = (int *)calloc(N, sizeof(int));
    for (int i = 0; i < ptr->size; i++)
    {
        memset(arr, 0, N * sizeof(int));
        bool temp_check = true;
        for (int j = 0; j < N; j++)
            arr[ptr->nums[i][j] - 1]++;

        for (int j = 0; j < N; j++)
        {
            if (arr[j] != 1)
            {
                temp_check = false;
                // break;
            }
        }
        ptr->check[i] = temp_check;
    }
    free(arr);
}

/*Please give name of input file in command line argument*/
int main(int argc, char **argv)
{
    /*read input file*/
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("%s: File doesn't exist\n", argv[1]);
        return 0;
    }
    // Output file to be generated by main thread
    FILE *out_file = fopen("output_pthread.txt", "w");

    // Number of points, number of threads
    if (fscanf(input, "%d %d", &k, &N) != EOF)
    {
        int mod = (3 * N) % k, div = (3 * N) / k;
        n = sqrt(N);

        sudoku = (int **)malloc(N * sizeof(int *));
        for (int i = 0; i < N; i++)
            sudoku[i] = (int *)malloc(N * sizeof(int));

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                fscanf(input, "%d", &sudoku[i][j]);
        }

        // Array of thread identifier of different threads
        threads = (pthread_t *)calloc(k, sizeof(pthread_t));
        pthread_attr_t attr;
        // Set the default attributes of threads
        pthread_attr_init(&attr);

        // Array of arguments to be passed to threads
        struct thread_args arguments[k];

        int count_rows = 0, count_cols = 0, count_grids = 0;

        for (int i = 0; i < k; i++)
        {
            int extra = 0;
            if (mod > 0)
            {
                extra++;
                mod--;
            }
            int size = div + extra;
            arguments[i].size = size;

            arguments[i].nums = (int **)malloc((size) * sizeof(int *));
            for (int j = 0; j < size; j++)
                arguments[i].nums[j] = (int *)malloc(N * sizeof(int));
            // arguments[i].start_row = (int*)malloc((size)*sizeof(int));
            // arguments[i].start_col = (int*)malloc((size)*sizeof(int));
            // arguments[i].end_row = (int*)malloc((size)*sizeof(int));
            arguments[i].count = (int *)malloc((size) * sizeof(int));
            arguments[i].check = (bool *)malloc((size) * sizeof(bool));
            arguments[i].type = (char *)malloc((size) * sizeof(char));

            int it = 0;
            while (it < size && count_rows < N)
            {
                for (int col = 0;col<N;col++) {
                    arguments[i].nums[it][col] = sudoku[count_rows][col];
                }
                arguments[i].check[it] = false;
                arguments[i].type[it] = 'r';
                arguments[i].count[it] = count_rows;
                it++;
                count_rows++;
            }
            while (it < size && count_cols < N)
            {
                for (int row = 0;row<N;row++) {
                    arguments[i].nums[it][row] = sudoku[row][count_cols];
                }
                arguments[i].check[it] = false;
                arguments[i].type[it] = 'c';
                arguments[i].count[it] = count_cols;
                it++;
                count_cols++;
            }
            while (it < size && count_grids < N)
            {
                int temp = count_grids / n;
                int start_row = temp * n;
                int start_col = (count_grids % n) * n;
                int ct=0;
                for (int row = start_row; row < start_row+n; row++)
                {
                    for (int col = start_col; col < start_col+n; col++)
                    {
                        arguments[i].nums[it][ct] = sudoku[row][col];
                        ct++;
                    }
                }
                arguments[i].check[it] = false;
                arguments[i].type[it] = 'g';
                arguments[i].count[it] = count_grids;
                it++;
                count_grids++;
            }
        }

        // Track of time
        double time_taken = 0.0;
        // clock_t begin = clock();
        struct timeval start, end;
        // start timer.
        gettimeofday(&start, NULL);

        // Create the threads
        for (int i = 0; i < k; i++)
            pthread_create(&threads[i], &attr, runner, &arguments[i]);

        // Join threads
        for (int i = 0; i < k; i++)
            pthread_join(threads[i], NULL);

        // clock_t end = clock();
        // // Total time spent in microseconds
        gettimeofday(&end, NULL);
        time_taken = (end.tv_sec - start.tv_sec) * M;
        time_taken = (time_taken + (end.tv_usec - start.tv_usec));
        printf("%.4lf\n", time_taken);
        // time_taken += (double)(end - begin) * (M) / CLOCKS_PER_SEC;

        bool final_check = true;

        // Print log in output file
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < arguments[i].size; j++)
            {
                if (arguments[i].type[j] == 'r')
                {
                    fprintf(out_file, "Thread %d checks row %d and is ", i + 1, arguments[i].count[j] + 1);
                }
                else if (arguments[i].type[j] == 'c')
                {
                    fprintf(out_file, "Thread %d checks column %d and is ", i + 1, arguments[i].count[j] + 1);
                }
                else
                {
                    fprintf(out_file, "Thread %d checks grid %d and is ", i + 1, arguments[i].count[j] + 1);
                }
                if (arguments[i].check[j])
                    fprintf(out_file, "valid\n");
                else
                {
                    fprintf(out_file, "invalid\n");
                    final_check = false;
                }
            }
        }
        fprintf(out_file, "\n");
        if (final_check)
            fprintf(out_file, "Sudoku is valid\n");
        else
            fprintf(out_file, "Sudoku is invalid\n");

        // Print time taken in output file
        fprintf(out_file, "The total time taken is %.1lf \xC2\xB5s", time_taken);

        fclose(input);
        fclose(out_file);
        for (int i = 0; i < N; i++) free(sudoku[i]);
        free(sudoku);
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < arguments[i].size; j++) free(arguments[i].nums[j]);
            free(arguments[i].nums);
            free(arguments[i].count);
            free(arguments[i].type);
            free(arguments[i].check);
        }
        free(threads);
    }
    else
    {
        printf("No input written in the file\n");
    }
    return 0;
}