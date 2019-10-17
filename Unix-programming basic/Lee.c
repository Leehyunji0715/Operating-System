///////////////////////////////////////////////////////////////////
// Student name: Hyunji Lee
// Course: COSC 3503 - Operating Systems
// Assignment: #7 - Unbuffered Input and Output
// Source Code File name: Lee.c
// Program's Purpose: Removes all of the single-line comments from a C, C++, or
//    Java text file and produces an output file with "nc" appended to the front
//    of the file name.  The program obtains the option "-s" or "-p" from the
//    command line followed by one or more file names. Each file on the 
//    command line is processed by the program either sequentially or in
//    parallel.  For the parallel approach, a separate forked process
//    performs the comment removal steps and a single parent process waits
//    for all children to complete  
// Program's Limitations: It assumes the input files are text files and that the 
//    operating system allows a program to have as many child processes
//    running as the number of files entered on the command line 
// Development Computer:  mac book pro
// Operating System: mac OS
// Compiler: gcc
// Program's Operational Status: 
///////////////////////////////////////////////////////////////////

#include "commentRemover.h"

#define GENERAL_BUFFER_SIZE 100
#define WRITE_BUFFER_SIZE 1
#define SLASH '/'
#define NEW_LINE '\n'
#define FALSE 0
#define TRUE  1

// Function Prototypes
static void processInputFile(const char *inputFileName);
static void removeComments(int inputFile, int outputFile);


// ################################################################
void performSequentialProcessing(int argc, char *argv[])
{
    for(int i = 0; i < argc - 2; i++){ //except for -p or -s
        processInputFile (argv[i+2]);
    }

} // End performSequentialProcessing


// ################################################################
void performParallelProcessing(int argc, char *argv[])
{
    int i = 0;
    pid_t pid;

    while(1){
        if(i == argc-2) // 
            break;

        pid = fork();

        if(pid == -1){
            printf("Fork failed!\n");
            exit(1);
        }
        else if (pid == 0){
            processInputFile (argv[i+2]);
            exit(0);
        }
        else {
            i++;
            wait(NULL);
        }
    }

} // End performParallelProcessing


// ################################################################
static void processInputFile(const char *inputFileName)
{
    char outputFileName [100] = "nc-";
    strcat(outputFileName, inputFileName);
    int inputFD;
    int outputFD;
    int closeFD1, closeFD2;

    inputFD = open(inputFileName, O_RDONLY);
    if(inputFD == -1){
        perror("Fail to read input file's name \n");
        exit(1);
    }

    outputFD = open(outputFileName, O_WRONLY | O_CREAT, S_IRUSR |  S_IWUSR);
    if(outputFD == -1){
        perror("Fail to read output file's name \n");
        exit(1);
    }
    printf("(%d) Reading from %s and writing to %s\n", getpid(), inputFileName, outputFileName);
    removeComments(inputFD, outputFD);
    closeFD1 = close(inputFD);
    closeFD2 = close(outputFD);

    if(closeFD1 == -1 || closeFD2 == -1){
        perror("Fail to close file");
        exit(1);
    }

} // End processInputFile


// ################################################################
static void removeComments(int inputFile, int outputFile)
{
    char buffer [100];
    int nbBytesRead;
    int nbrBytesWritten;
    
    while(1){
        nbBytesRead = read(inputFile, buffer,  GENERAL_BUFFER_SIZE);
        if(nbBytesRead == -1){
            printf("Fail to read\n");
            exit(1);
        }
        else if(nbBytesRead == 0){ // nothing to read
            break;
        }
        else{ // read 
            for(int i = 0; i < 99; i++){
                if(buffer[i] == SLASH && buffer[i+1] == SLASH){ // detect comment  
                    while(1){
                        i++;
                        if(buffer[i] == NEW_LINE)
                            break;
                        else if (i == 99)
                            break;
                    }
                }
                if(i == 99)
                    break;
                nbrBytesWritten = write (outputFile, &buffer[i], WRITE_BUFFER_SIZE);
            }
        }
    }


} // End removeComments
