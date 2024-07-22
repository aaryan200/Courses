1. There are two programs, one for Wait-Free implementation and other for Obstruction-Free implementation.
2. To compile the program, use the following command:
    g++ WFS_co21btech11001.cpp -o wfs
    Similarly,
    g++ OBS_co21btech11001.cpp -o obs
3. To run the program, use the following command:
    ./wfs inp-params.txt
    Similarly,
    ./obs inp-params.txt
4. The program named "WFS_co21btech11001.cpp" will output a log file named "LogFile_WFS.txt".
5. The program named "OBS_co21btech11001.cpp" will output a log file named "LogFile_OBS.txt".
6. Both of the programs will output the input parameters (which are read from the input file), 
   the average operation time and the worst case time in stdout.