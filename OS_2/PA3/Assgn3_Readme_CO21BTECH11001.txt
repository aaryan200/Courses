Instructions to run code:

1. Compile the code using g++ <filename>.cpp

2. Pass the name of input file as command line argument.
   For example, ./a.out inp_params.txt

3. After successful execution of the code, a file named output_<ProgName>.txt will be created in the same directory. Where <ProgName> is the one of the following: tas, cas, casBounded.
    This file contains the details of the times when a thread request to enter the critical section, the time when it actually enters the critical section and the time when it leaves the critical section.