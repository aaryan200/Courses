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

// Size of sudoku, size of a subgrid, number of threads
int N, n, k;

// 2D array of sudoku
int **sudoku;

// Array of thread identifier of different threads
pthread_t *threads;

/*A structure to pass arguments to a thread*/
struct thread_args
{
    int size;       // Total number of rows and columns and subgrids that the thread is going to check
    int *start_row; // Array to store starting row of row or column or subgrid
    int *start_col; // Array to store starting column of row or column or subgrid
    int *end_row;   // Array to store ending row of row or column or subgrid
    int *end_col;   // Array to store ending column of row or column or subgrid
    bool *check;    // Array to store whether row or column or subgrid is valid or invalid
};

/* Runner function of thread */
void *runner(void *args)
{
    struct thread_args *ptr = (struct thread_args *)args;

    // arr[i] stores frequency of element i+1 in row or column or subgrid
    int *arr;
    arr = (int *)calloc(N, sizeof(int));
    for (int i = 0; i < ptr->size; i++)
    {
        memset(arr, 0, N * sizeof(int));
        bool temp_check = true;
        for (int row = ptr->start_row[i]; row <= ptr->end_row[i]; row++)
        {
            for (int col = ptr->start_col[i]; col <= ptr->end_col[i]; col++)
            {
                if (sudoku[row][col] >= 1 && sudoku[row][col] <= N)
                    arr[sudoku[row][col] - 1]++;
                else
                    temp_check = false;
            }
        }
        for (int j = 0; j < N; j++)
        {
            // If frequency of any element is not 1, then the row or column or subgrid is invalid
            if (arr[j] != 1)
            {
                temp_check = false;
            }
        }
        ptr->check[i] = temp_check;
    }
    free(arr);
}

/* Please give name of input file in command line argument */
int main(int argc, char **argv)
{
    /* read input file */
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("%s: File doesn't exist\n", argv[1]);
        return 0;
    }
    // Output file to be generated by main thread
    FILE *out_file = fopen("output_pthread.txt", "w");

    // Number of threads, size of sudoku
    if (fscanf(input, "%d %d", &k, &N) != EOF)
    {
        // 3N is the total number of rows and columns and subgrids in sudoku
        int mod = (3 * N) % k, div = (3 * N) / k;
        n = sqrt(N);

        sudoku = (int **)malloc(N * sizeof(int *));
        for (int i = 0; i < N; i++)
            sudoku[i] = (int *)malloc(N * sizeof(int));

        // Take sudoku as input from sudoku
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                fscanf(input, "%d", &sudoku[i][j]);
        }

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
            int it = 0;
            arguments[i].size = size;
            arguments[i].start_row = (int *)malloc((size) * sizeof(int));
            arguments[i].start_col = (int *)malloc((size) * sizeof(int));
            arguments[i].end_row = (int *)malloc((size) * sizeof(int));
            arguments[i].end_col = (int *)malloc((size) * sizeof(int));
            arguments[i].check = (bool *)malloc((size) * sizeof(bool));

            while (it < size && count_rows < N)
            {
                arguments[i].start_row[it] = count_rows;
                arguments[i].start_col[it] = 0;
                arguments[i].end_row[it] = count_rows;
                arguments[i].end_col[it] = N - 1;
                arguments[i].check[it] = false;
                it++;
                count_rows++;
            }

            while (it < size && count_cols < N)
            {
                arguments[i].start_row[it] = 0;
                arguments[i].start_col[it] = count_cols;
                arguments[i].end_row[it] = N - 1;
                arguments[i].end_col[it] = count_cols;
                arguments[i].check[it] = false;
                it++;
                count_cols++;
            }

            while (it < size && count_grids < N)
            {
                int temp = count_grids / n;
                arguments[i].start_row[it] = temp * n;
                arguments[i].start_col[it] = (count_grids % n) * n;
                arguments[i].end_row[it] = arguments[i].start_row[it] + n - 1;
                arguments[i].end_col[it] = arguments[i].start_col[it] + n - 1;
                arguments[i].check[it] = false;
                it++;
                count_grids++;
            }
        }

        // Track of time
        double time_taken = 0.0;
        struct timeval start, end;

        // Start timer.
        gettimeofday(&start, NULL);

        // Create the threads
        for (int i = 0; i < k; i++)
            pthread_create(&threads[i], &attr, runner, &arguments[i]);

        // Join threads
        for (int i = 0; i < k; i++)
            pthread_join(threads[i], NULL);

        // Total time spent in microseconds
        gettimeofday(&end, NULL);
        time_taken = (end.tv_sec - start.tv_sec) * M;
        time_taken = (time_taken + (end.tv_usec - start.tv_usec));

        printf("Time taken: %.4lf\n", time_taken);

        bool final_check = true;

        // Checking the output of all threads and printing log in output file
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < arguments[i].size; j++)
            {
                if (arguments[i].start_row[j] == arguments[i].end_row[j])
                {
                    fprintf(out_file, "Thread %d checks row %d and is ", i + 1, arguments[i].start_row[j] + 1);
                }
                else if (arguments[i].start_col[j] == arguments[i].end_col[j])
                {
                    fprintf(out_file, "Thread %d checks column %d and is ", i + 1, arguments[i].start_col[j] + 1);
                }
                else
                {
                    int quot = arguments[i].start_row[j] / n;
                    int rem = arguments[i].start_col[j] / n;
                    fprintf(out_file, "Thread %d checks grid %d and is ", i + 1, n * quot + rem + 1);
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
        {
            fprintf(out_file, "Sudoku is valid\n");
            printf("Sudoku is valid\n");
        }
        else
        {
            fprintf(out_file, "Sudoku is invalid\n");
            printf("Sudoku is invalid\n");
        }

        // Print time taken in output file
        fprintf(out_file, "The total time taken is %.1lf \xC2\xB5s", time_taken);

        fclose(input);
        fclose(out_file);
        for (int i = 0; i < N; i++)
            free(sudoku[i]);
        free(sudoku);
        for (int i = 0; i < k; i++)
        {
            free(arguments[i].start_row);
            free(arguments[i].start_col);
            free(arguments[i].end_row);
            free(arguments[i].end_col);
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