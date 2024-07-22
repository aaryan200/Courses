1. There are two programs, one for CLH lock implementation and other for MCS lock implementation.
2. To compile the program, use the following command:
    g++ -std=c++20 CLH-co21btech11001.cpp -o clh
    Similarly,
    g++ -std=c++20 MCS-co21btech11001.cpp -o mcs
3. To run the program, use the following command:
    ./clh inp-params.txt
    Similarly,
    ./mcs inp-params.txt
4. The program named "CLH-co21btech11001.cpp" will output a log file named "CLH-out.txt".
5. The program named "MCS-co21btech11001.cpp" will output a log file named "MCS-out.txt".
6. Both of the programs will output the input parameters (which are read from the input file), 
   the average critical section entry time and throughput in stdout.