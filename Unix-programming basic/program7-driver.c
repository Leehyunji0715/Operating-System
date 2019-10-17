///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503 - Operating Systems
// Assignment: #7 - Unbuffered Input and Output
// Source Code File name: program7-driver.c
// Program's Purpose: Parses the command line and calls either the sequential
// processing function or the parallel processing function depending on the
// command line input  
// Program's Limitations: It assumes the input files are text files and that the 
//    operating system allows a program to have as many child processes
//    running as the number of files entered on the command line 
// Development Computer: Dell 
// Operating System: Windows 7 running CYGWIN
// Compiler: Gnu C
// Program's Operational Status: Fulfills all requirements
///////////////////////////////////////////////////////////////////

#include "commentRemover.h"

#define COMPARE_SIZE 3
#define GENERAL_BUFFER_SIZE 100

// Function Prototypes
void displayUsageMessage(void);


// ################################################################
int main(int argc, char *argv[])
{
int count;
char buffer[GENERAL_BUFFER_SIZE];

if (argc < 3)
   {
   displayUsageMessage();
   return 1;
   } // End if
else if (strncmp(argv[1], "-s", COMPARE_SIZE) == 0)
   {
   performSequentialProcessing(argc, argv);
   } // End else if
else if (strncmp(argv[1], "-p", COMPARE_SIZE) == 0)
   {
   performParallelProcessing(argc, argv);
   } // End else if
else
   {
   displayUsageMessage();
   return 1;
   } // End if

snprintf(buffer, GENERAL_BUFFER_SIZE, "\nFinished removing comments from %d files\n", argc - 2);
write(STDERR_FILENO, buffer, strlen(buffer));
return 0;
} // End main


// ################################################################
void displayUsageMessage(void)
{
char buffer[GENERAL_BUFFER_SIZE];

snprintf(buffer, GENERAL_BUFFER_SIZE, "\nUsage: a.out {-s | -p} <file_name> [file_name ...]\n");
write(STDERR_FILENO, buffer, strlen(buffer));
} // End displayUsageMessage


