Instructions to run code:

1. While compiling the pthread file, use flags -pthread -lm
   i.e., gcc Assgn2SrcPthreadCO21BTECH11001.c -pthread -lm
   
   Note: Ignore compilation warnings if any.
   
2. While compiling the openmp file, use flags -fopenmp -lm
   i.e., gcc Assgn2SrcOpenmpCO21BTECH11001.c -fopenmp -lm
   
   Note: Ignore compilation warnings if any.
   
3. Make sure to given input text file as command line argument.
   for e.g., ./a.out 25x25.txt
   
4. After successful execution of pthread code, a file named output_pthread.txt will be created in same directory as source file.
   This output file will contain the following fields:
   a. Time taken to execute in microseconds
   b. Validation of sudoku as done by the program
   c. Log, which contains the information about the rows and columns and grids which are checked by the threads.
   
5. After successful execution of openmp code, a file named output_openmp.txt will be created in same directory as source file.
   This output file will contain the following fields:
   a. Time taken to execute in microseconds
   b. Validation of sudoku as done by the program
   c. Log, which contains the information about the rows and columns and grids which are checked by the threads.
