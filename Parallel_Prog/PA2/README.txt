To execute the program, follow these steps:

1. Compile the file
    Example: g++ CLQ_co21btech11001.cpp
    or g++ NLQ_co21btech11001.cpp

2. Run the executable and provide the name of input file as command line argument
    Example: ./a.out inp_params.txt

3. File named CLQ_co21btech11001.cpp will output a file named CLQ-out.txt containing the average time taken to execute
one operation.

4. File named NLQ_co21btech11001.cpp will output a file named CLQ-out.txt containing the average time taken to execute
one operation.

5. Important: It is less probable, although possible, that the program for NLQ may come to a halt when the queue is empty
and every thread has invoked deq() method call. Therefore, it is recommended to comment out lines 108 and 123 of the file
NLQ_co21btech11001.cpp so that you will be able to see if the program has come to a halt. If the program has come to a
halt, then you can terminate the program by pressing Ctrl+C. The same is mentioned in the report.

6. Every program will output the input parameters, average enqueue time, average dequeue time, average operation time and
throughput on stdout.