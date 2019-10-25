///////////////////////////////////////////////////////////////////
// Student name: Hyunji Kee
// Course: COSC 3503
// Assignment: #9 - POSIX Signals
// Source Code File Name: Lee10.c
// Program's Purpose: Analyze FCFS, SJF and Round robin scheduling algorithms, and show the results of them each.
//                    (Time quantum), Average waiting time and Average turn-around time
// Assumptions: Every process starts at 0
// Program's Limitations: None known
// 
// Development Computer: mac book pro 
// Operating System: mac book pro
// Compiler: Gnu gcc
// Build Directions: gcc program10-driver.c Lee10.c
//                     
// Program's Operational Status: Fulfills the requirements
///////////////////////////////////////////////////////////////////
#include "scheduling-algorithms.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Function Prototypes
void sortRows(processDataType processes[], const int processCount);
void scheduleFCFS(processDataType processes[], int processCount);
void scheduleSJF(processDataType processes[], int processCount);
void scheduleRoundRobin(processDataType processes[], int processCount, int quantum);



void sortProcesses (processDataType processes[], int processCount)
{
    processDataType temp;
    int i;
    int j;

    for (i = processCount - 2; i >= 0; i--)
    {
    for (j = 0; j <= i; j++)
        {
        if (processes[j].totalTime > processes[j + 1].totalTime){
            temp = processes[j + 1];
            processes[j + 1] = processes[j];
            processes[j] = temp;
            } // End if
        } // End j
    } // End for
}

void addWaitTime(processDataType processes[], int nbrOfProcess, int addedIndex,int addedTime){
    for(int i = 0 ; i<nbrOfProcess ;i++){
        if(i == addedIndex)
            continue;
        if(processes[i].isFinished == FALSE){
            processes[i].waitSum += addedTime;
        }
    }
}


// Function 
void scheduleFCFS(processDataType processes[], int processCount){
    int startTime = 0;
    int stopTime = 0;
    int runTime = 0;
    int totalWaitTime = 0;
    int totalTurnAroundTime = 0;

    double avgWaitTime = 0;
    double avgTurnAroundTime = 0;

    printf("Process#    Start Time    Stop Time    Running Time\n");
    printf("--------    ----------    ---------    ------------\n");

    for(int i=0 ; i<processCount; i++){
        startTime = stopTime; // get previous stop Time
        runTime = processes[i].totalTime; // execute the whole time
        stopTime += runTime;
        totalWaitTime += startTime;
        totalTurnAroundTime += stopTime;
        printf("  %3d       %5d      %8d      %8d\n", processes[i].processNbr, startTime, stopTime, runTime);
    }

    avgWaitTime = (double) totalWaitTime / processCount;
    avgTurnAroundTime = (double) totalTurnAroundTime / processCount;
    printf("\n    Average waiting time:   %4.2f msec\n", avgWaitTime);
    printf("Average turn-around time:   %4.2f msec\n", avgTurnAroundTime);
}

void scheduleSJF(processDataType processes[], int processCount){
    //copy processes
    processDataType cProcesses [processCount];
    for(int i = 0; i<processCount ; i++){
        memcpy(cProcesses + i, processes + i, sizeof(processes[i]));
    }

    //sort copied processes
    sortProcesses(cProcesses, processCount);
    //call FCFS
    scheduleFCFS(cProcesses, processCount);
}

void scheduleRoundRobin(processDataType processes[], int processCount, int quantum){
    int i = 0;
    int checkFinish = 0;
    int startTime = 0;
    int stopTime = 0;
    int runTime = 0;
    int totalWaitTime = 0;
    int totalTurnAroundTime = 0;

    double avgWaitTime = 0;
    double avgTurnAroundTime = 0;

    //copy processes
    processDataType cProcesses [processCount];
    for(int i = 0; i<processCount ; i++){
        memcpy(cProcesses + i, processes + i, sizeof(processes[i]));
    }
    //print out
    printf("Process#    Start Time    Stop Time    Running Time\n");
    printf("--------    ----------    ---------    ------------\n");
    
    while(1){
        startTime = stopTime;

        if(processes[i].remainingTime <= quantum){
            runTime = processes[i].remainingTime;
            processes[i].remainingTime = 0;
            processes[i].stopTime = stopTime + runTime;
        }
        else{
            runTime = quantum;
            processes[i].remainingTime -= runTime;
        }

        stopTime += runTime;
        addWaitTime(processes, processCount, i, runTime);
        if(processes[i].remainingTime == 0)
            processes[i].isFinished = TRUE;
        printf("  %3d       %5d      %8d      %8d\n", processes[i].processNbr, startTime, stopTime, runTime);
        

        checkFinish = 0; //if  processes' remaining Time is 0, then finish ++
        //choosing next index
        while(1){
            i++;
            if(i == processCount)
                i = 0;
            if(processes[i].remainingTime == 0){
                checkFinish++;
                if(checkFinish == processCount)
                    break;
            }
            else
                break;            
        }
        if(checkFinish == processCount)
            break;
    }

    for(int i=0 ; i<processCount ;i++){
        totalWaitTime += processes[i].waitSum;
        totalTurnAroundTime += processes[i].stopTime;
    }

    avgWaitTime = (double)totalWaitTime / processCount;
    avgTurnAroundTime = (double)totalTurnAroundTime / processCount;

    printf("\n            Time quantum:   %5d msec\n",quantum);
    printf("    Average waiting time:   %4.2f msec\n", avgWaitTime);
    printf("Average turn-around time:   %4.2f msec\n", avgTurnAroundTime);
}

