///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC 3503 - Operating Systems
// Assignment: #6 - Unbuffered Input and Output
// File name: commentRemover.h
// Program's Purpose: Provides the function prototypes for the student6.c file  
// Limitations: None known 
// Development Computer: Dell 
// Operating System: Windows 7 running CYGWIN
// Integrated Development Environment (IDE): wordpad
// Compiler: Gnu C
// Program's Operational Status: Fulfills all requirements
///////////////////////////////////////////////////////////////////

#ifndef COMMENT_REMOVER
#define COMMENT_REMOVER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Function Prototypes
void performSequentialProcessing(int argc, char *argv[]);
void performParallelProcessing(int argc, char *argv[]);

#endif

