#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <math.h>
#define N 100
#define THRESHOLD 1.0e-5

/*Linked list nodes for storing perfect numbers by threads*/
struct node
{
    int number;
    struct node *next;
};

/*Insert at top of list*/
void Insert(struct node **ptrhead, int num)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->number = num;
    temp->next = NULL;
    if (*ptrhead != NULL) temp->next = *ptrhead;
    *ptrhead = temp;
    return;
}

/*Deallocates the memory allocated to a list*/
void freeList(struct node **head)
{
    struct node *temp = *head;
    while (temp != NULL)
    {
        struct node *pr = temp;
        temp = temp->next;
        free(pr);
    }
    return;
}

/*Struct created for passing all the parameters for thread in a single struct*/
struct data
{
    int *start;         // Number from beginning where thread will start processing
    int *end;           // Number from end where thread will start processing
    int max_numbers;    // Maximum numbers to be processed by the thread
    bool last;          // True if the thread is last thread, false otherwise
    FILE **file;        // Log file created by thread
    struct node **head; // Head of list where thread will store the numbers which are identified as perfect numbers.
};

/*Return true if num is a perfect number
else return false*/
bool check_perfect(int num)
{
    if (num <= 1) return false;
    float exact_sqrt = sqrt(num);
    int sqrt_of_num = floor(exact_sqrt);
    int sum_of_factors = 1; // 1 is a trivial factor
    for (int i = 2; i <= sqrt_of_num; i++)
    {
        if (num % i == 0) sum_of_factors += (i + num / i);
    }
    /*Case of perfect squares*/
    if (exact_sqrt - sqrt_of_num <= THRESHOLD) {
        sum_of_factors -= sqrt_of_num; // Because the square root will be added twice in the for loop
    }
    if (sum_of_factors == num) return true;
    return false;
}

/*Runner function of thread*/
void *runner(void *param)
{
    struct data *ptr = (struct data *)param;
    int maxNums = ptr->max_numbers;
    int count = 0; // Counting numbers processed by the thread
    bool last = ptr->last;
    /*If the thread is not last thread, it has to process "maxNums" number of numbers*/
    if (!last)
    {
        while (count < maxNums)
        {
            if (count % 2 == 0)
            {
                if (check_perfect(*(ptr->start)))
                {
                    // Printing in log file created by the thread
                    fprintf(*(ptr->file), "%d: Is a perfect number\n", *(ptr->start));
                    // Insert the number in list of perfect numbers identified by the thread
                    Insert(ptr->head, *(ptr->start));
                }
                else
                {
                    fprintf(*(ptr->file), "%d: Not a perfect number\n", *(ptr->start));
                }
                (*(ptr->start))++;
            }
            else
            {
                if (check_perfect(*(ptr->end)))
                {
                    fprintf(*(ptr->file), "%d: Is a perfect number\n", *(ptr->end));
                    Insert(ptr->head, *(ptr->end));
                }
                else
                {
                    fprintf(*(ptr->file), "%d: Not a perfect number\n", *(ptr->end));
                }
                (*(ptr->end))--;
            }
            count++;
        }
    }
    /*If thread is last thread, it will process all numbers form "start" to "end"*/
    else
    {
        while (*(ptr->start) <= (*ptr->end))
        {
            if (count % 2 == 0)
            {
                if (check_perfect(*(ptr->start)))
                {
                    // Printing in log file created by the thread
                    fprintf(*(ptr->file), "%d: Is a perfect number\n", *(ptr->start));
                    // Insert the number in list of perfect numbers identified by the thread
                    Insert(ptr->head, *(ptr->start));
                }
                else
                {
                    fprintf(*(ptr->file), "%d: Not a perfect number\n", *(ptr->start));
                }
                (*(ptr->start))++;
            }
            else
            {
                if (check_perfect(*(ptr->end)))
                {
                    fprintf(*(ptr->file), "%d: Is a perfect number\n", *(ptr->end));
                    Insert(ptr->head, *(ptr->end));
                }
                else
                {
                    fprintf(*(ptr->file), "%d: Not a perfect number\n", *(ptr->end));
                }
                (*(ptr->end))--;
            }
            count++;
        }
    }
    pthread_exit(0);
}

/*Please give name of input file in command line argument*/
int main(int argc, char **argv)
{
    /*read inputs n and k from input.txt*/
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("%s: File doesn't exist\n", argv[1]);
        return 0;
    }
    int n, k;
    int ct = 0;          // Counts the number of inputs from input file
    char main_name[N];   // Name of log file of main thread
    char thread_name[N]; // Name of log file of threads other than main thread
    char directory[N];   // Name of directory created for different inputs
    while (fscanf(input, "%d %d", &n, &k) != EOF)
    {
        ct++;
        int max_nums = n / k; // Maximum numbers that will be assigned to a thread

        sprintf(directory, "Input_%d", ct);
        if (mkdir(directory, 0777) == -1)
        {
            fclose(input);
            printf("Please remove the directory %s and run again\n", directory);
            return 0;
        }

        sprintf(main_name, "%s/Main.log", directory);
        FILE *main_file = fopen(main_name, "a+"); // Log file created by main thread
        FILE **files;                             // Array of log file created by threads other than main thread
        files = (FILE **)malloc(k * sizeof(FILE *));

        struct node **arrayOfPerfectNumbers; // Array where threads will store the numbers which are identified as perfect
        arrayOfPerfectNumbers = (struct node **)malloc(sizeof(struct node *) * k);

        for (int i = 0; i < k; i++) arrayOfPerfectNumbers[i] = NULL;

        pthread_t threads[k]; // Array of thread identifier of different threads
        pthread_attr_t attr;
        pthread_attr_init(&attr); // Set the default attributes of threads

        int num_from_start = 1;   // Starting number assigned to a thread
        int num_from_end = n;     // Number from end assigned to a process

        for (int i = 0; i < k; i++)
        {
            struct data params; // Parameters to be passed to ith thread
            if (i == k - 1) params.last = true;
            else params.last = false;

            params.start = &num_from_start;
            params.end = &num_from_end;
            params.max_numbers = max_nums;

            sprintf(thread_name, "%s/Thread_%d.log", directory, i + 1);
            files[i] = fopen(thread_name, "a+");
            params.file = (&files[i]);                           // ith file allocated to ith process
            params.head = (&arrayOfPerfectNumbers[i]);           // ith list allocated to ith process

            pthread_create(&threads[i], &attr, runner, &params); /*Create the thread*/
            pthread_join(threads[i], NULL);                      /*Wait for thread to exit*/
        }

        for (int i = 0; i < k; i++)
        {
            // Printing in log file of main thread
            fprintf(main_file, "Thread%d: ", i + 1);
            struct node *temp = arrayOfPerfectNumbers[i];

            while (temp != NULL)
            {
                fprintf(main_file, "%d ", temp->number);
                temp = temp->next;
            }
            
            fprintf(main_file, "\n");
        }
        printf("Created directory %s for input n = %d, k = %d\n", directory, n, k);

        fclose(main_file);
        for (int i = 0; i < k; i++) fclose(files[i]);

        for (int i = 0; i < k; i++) freeList(&arrayOfPerfectNumbers[i]);
        free(files);
        free(arrayOfPerfectNumbers);
    }

    fclose(input);
    return 0;
}