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
#define SIZE 64

int main()
{
    FILE *ptr = fopen("input.txt", "r");
    /*
    input.txt is:
    200 100
    300 400
    */
    if (ptr == NULL)
    {
        printf("No such file");
        return 0;
    }
    int n, k;
    while (fscanf(ptr, "%d %d", &n, &k) == 2)
    {
        printf("%d %d\n", n, k);
        for (int i = 0; i < 2; i++)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                printf("Child process %d\n", i + 1);
                exit(0);
            }
            else
                wait(NULL);
        }
    }

    return 0;
}
