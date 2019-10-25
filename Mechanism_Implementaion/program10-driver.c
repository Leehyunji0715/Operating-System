////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503
// Assignment: #10 - CPU Scheduling Algorithms
// Source Code File Name: program10-driver.c
// Program's Purpose: Serves as the driver for a program that analyzes the use
//          of three CPU scheduling algorithms: FCFS, SJF, and RR  
// Assumptions: The user understands the purpose of the analysis and how to
//              interpret the output report
// Program's Limitations: The program only handles up to 100 processes,
//    but this value can be adjusted by changing the #define constant 
// 
// Development Computer: Dell 
// Operating System: Windows 7 running CYGWIN
// Compiler: gcc
// Build Directions: program9-driver.c student9.c
// Program's Operational Status: Satisfies all of the requirements
////////////////////////////////////////////////////////////////

#include "scheduling-algorithms.h"
#include <time.h>
#include <signal.h>
#include <string.h>

// Constants
#define USER_CHOICE    "-user"
#define RANDOM_CHOICE  "-random"
#define MAX_LENGTH     10

// Function Prototypes
void parseCommandLine(int argc, char *argv[], int *nbrOfProcessesPtr, int *timeQuantumPtr,
                      int *maxRunningTimePtr, int *randomizeDesiredPtr);
void displayUsageMessage(void);
void readProcessTimes(processDataType processes[], int processCounter);
void randomizeProcessTimes(processDataType processes[], int processCounter, int maxTime);


// #############################################################
int main(int argc, char *argv[])
{
processDataType processTable[MAX_PROCESSES];
int nbrOfProcesses;
int timeQuantum;
int maxRunningTime;
int randomizeDesired = FALSE;
int i;
time_t analysisStartTime;

analysisStartTime = time(NULL);

parseCommandLine(argc, argv, &nbrOfProcesses, &timeQuantum, &maxRunningTime, 
                 &randomizeDesired);

if (randomizeDesired)
   randomizeProcessTimes(processTable, nbrOfProcesses, maxRunningTime);
else  
   readProcessTimes(processTable, nbrOfProcesses);

printf("\nThis analysis report was generated on %s", ctime(&analysisStartTime));

printf("\n** START OF ANALYSIS REPORT **\n");

printf("\n\n** First Come, First Served (FCFS) Scheduling **\n");
scheduleFCFS(processTable, nbrOfProcesses);

printf("\n\n** Shortest Job First (SJF) Scheduling **\n");
scheduleSJF(processTable, nbrOfProcesses);

printf("\n\n** Round Robin (RR) Scheduling **\n");
scheduleRoundRobin(processTable, nbrOfProcesses, timeQuantum);

printf("\n** END OF ANALYSIS REPORT **\n\n");

return 0;
} // End main


// #############################################################
void parseCommandLine(int argc, char *argv[], int *nbrOfProcessesPtr, int *timeQuantumPtr,
                      int *maxRunningTimePtr, int *randomizeDesiredPtr)
{
if ( (argc <= 3) || (argc >= 6) )
   {
   displayUsageMessage();
   exit(1);
   } // End if
else if ( (argc == 4) && (strncmp(argv[3], USER_CHOICE, MAX_LENGTH) != 0))
   {
   displayUsageMessage();
   exit(1);
   } // End if
else if ( (argc == 5) && (strncmp(argv[3], RANDOM_CHOICE, MAX_LENGTH) != 0))
   {
   displayUsageMessage();
   exit(1);
   } // End if

*nbrOfProcessesPtr = atoi(argv[1]);
if (*nbrOfProcessesPtr < 0)
   {
   fprintf(stderr, "\nCommand line error: Number of processes must be greater than zero\n");
   exit(1);
   } // End if

*timeQuantumPtr = atoi(argv[2]);
if (*timeQuantumPtr <= 0)
   {
   fprintf(stderr, "\nCommand line error: Time quantum must be greater than zero\n");
   exit(1);
   } // End if

if (strncmp(argv[3], USER_CHOICE, MAX_LENGTH) == 0)
   *randomizeDesiredPtr = FALSE;
else if (strncmp(argv[3], RANDOM_CHOICE, MAX_LENGTH) == 0)
   {
   *randomizeDesiredPtr = TRUE;
   *maxRunningTimePtr = atoi(argv[4]);
   if (*maxRunningTimePtr <= 0)
      {
      fprintf(stderr, "\nCommand line error: Max running time must be greater than 0\n");
      exit(1);
      } // End if
   } // End else if
else
   {
   fprintf(stderr, "\nCommand line error: Source of run times must be %s ro %s\n", USER_CHOICE, RANDOM_CHOICE);
   exit(1);
   } // End else 

printf("\n** Analysis Options **\n\n");
printf("   Total number of processes to analyze      : %d\n", *nbrOfProcessesPtr);
printf("   Quantum time used in round robin analysis : %d msec\n", *timeQuantumPtr);
if (*randomizeDesiredPtr)
   {
   printf("   Randomly generate process running times   : Yes\n");
   printf("   Maximum total running time of a process   : %d msec\n", *maxRunningTimePtr);
   } // End if
else
   {
   printf("   Randomly generate process running times   : No\n");
   printf("   Maximum total running time of a process   : N/A\n");
   } // End else

} // End parseCommandLine


// #############################################################
void displayUsageMessage(void)
{
fprintf(stderr, "\nUsage: a.out <#processes> <time quantum> -user\n");
fprintf(stderr, "\nUsage: a.out <#processes> <time quantum> -random <max runtime>\n\n");
fprintf(stderr, "       <#processes>         : Positive integer\n");
fprintf(stderr, "       <time quantum>       : Positive integer\n");
fprintf(stderr, "       <max runtime>        : Positive integer\n");
} // End displayUsageMessage


// #############################################################
void readProcessTimes(processDataType processes[], int processCounter)
{
int i;
int jobTime;

printf("\nEnter a run time for each of the %d processes\n\n", processCounter);
for (i = 0; i < processCounter; i++)
   {
   do
      {
      printf("   Process #%d: ", i);
      scanf("%d", &jobTime);
      if (jobTime <= 0)
         printf("   Error: Run time value must be greater than zero\n");
      }
   while (jobTime <= 0);

   // Initialize the fields of the process record
   processes[i].processNbr = i; // "Read only"; used by FCFS, SJF, and Round Robin
   processes[i].totalTime = jobTime; // "Read only"; used by FCFS, SJF, and Round Robin
   processes[i].remainingTime = jobTime; // "Read and write"; used only by Round Robin
   processes[i].stopTime = 0; // "Read and write"; used only by Round Robin
   processes[i].waitSum = 0; // "Read and write"; used only by Round Robin
   processes[i].isFinished = FALSE; // "Read and write"; used only by Round Robin
   } // End for

} // End readProcessTimes 


// #############################################################
void randomizeProcessTimes(processDataType processes[], int processCounter, int maxTime)
{
int i;
int nbrOfProcesses;
int jobTime;

printf("\nGenerating running times for %d processes . . .\n", processCounter);

srand(time(NULL)); // Initialize seed value for random number generator

for (i = 0; i < processCounter; i++)
   {
   jobTime = (rand() % maxTime) + 1;

   // Initialize the fields of the process record
   processes[i].processNbr = i;
   processes[i].totalTime = jobTime;
   processes[i].remainingTime = jobTime;
   processes[i].stopTime = 0;
   processes[i].waitSum = 0;
   processes[i].isFinished = FALSE;
   } // End for

} // End randomizeProcessTimes 


