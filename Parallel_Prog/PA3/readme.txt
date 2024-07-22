1. There are two programs, one for custom implementation and other for inbuilt implementation.
2. To compile the program, use the following command:
    g++ MRMW_custom_co21btech11001.cpp -o MRMW_custom
    or
    g++ MRMW_atomic_co21btech11001.cpp -o MRMW_atomic
3. To run the program, use the following command:
    ./MRMW_custom inp-params.txt
    or
    ./MRMW_atomic inp-params.txt
4. The program named "MRMW_custom_co21btech11001.cpp" will output a log file named "LogFile_MRMW.txt".
5. The program named "MRMW_atomic_co21btech11001.cpp" will output a log file named "LogFile_atomic.txt".
6. Both of the programs will output the input parameters (which are read from the input file) and 
   the average operation time in stdout.