///////////////////////////////////////////////////////////////////
// Student name: Hyunji Lee
// Course: COSC 3503 - Operating Systems
// Assignment: Assignment 4
// File name: Lee4.c
// Program's Purpose: Read input file that contains 2 dimensional integers,
//                    print sum, average, and maximum value per row.
//
// Limitations: The program only reads and displays MAX_COUNT of integers
// 
// Development Computer: Mac book
// Operating System: Mac OS
// Integrated Development Environment (IDE): Wordpad
// Compiler: Gnu gcc 
// Program's Operational Status: Fulfills all requirements
/////////////////////////////////////////////////////////////////// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_SIZE 200
#define TRUE 1
#define FALSE 0

FILE * inputFile;

int readTableSize(FILE *inputFile, int *rowCountPtr, int *colCountPtr);
void processTableContents(FILE *inputFile, int rowCount, int columnCount);
void writeReportHeading(void);
void readTableRow(FILE *inputFile, int nbrOfEntries, int rowEntries[]);
void writeReportRow(int rowNbr, int nbrOfEntries, int rowEntries[]);


int main (int argc, char * argv []) {
    int * rowCountPtr = malloc(sizeof(int));
    int * columnCountPtr = malloc(sizeof(int));
    char fileName [MAX_SIZE];

    if(argc == 2){
        strncpy(fileName, argv[1], MAX_SIZE);
    }
    else{
        printf("The number of command line arguments is incorrect (Give 1 file name)\n");
        return 0;
    }
    
    inputFile = fopen(fileName, "r");

    if(inputFile == NULL){
        fprintf(stderr, "Error: Could not open the file\n");
        return 0;
    }

    if ( readTableSize(inputFile, rowCountPtr, columnCountPtr) == TRUE ) {
        processTableContents(inputFile, *rowCountPtr, *columnCountPtr);
    }
    else {
        fprintf(stderr, "Error: Row and/or column values are out of range\n");
        return 0;
    }

    fclose(inputFile);
    return 0;
}


int readTableSize(FILE *inputFile, int *rowCountPtr, int *colCountPtr){

    int rowCount = 0;
    int colCount = 0;
    fscanf(inputFile, "%d %d", &rowCount, &colCount);

    if(rowCount >= 1 && rowCount <=7 && colCount >=2 && colCount <=10){
        *rowCountPtr = rowCount;
        *colCountPtr = colCount;
        return TRUE;
    }
    else
        return FALSE;
}

void processTableContents(FILE *inputFile, int rowCount, int columnCount) {
    int rowEntries [rowCount][columnCount];
    writeReportHeading();

    for(int i = 0 ; i < rowCount ; i++){
            readTableRow ( inputFile, columnCount, rowEntries[i] );
            writeReportRow ( i+1, columnCount , rowEntries[i] );
    }


}

void writeReportHeading(void){
    printf("Row  Sum  Avg  Max\n");
    printf("___________________\n");
}
void readTableRow(FILE *inputFile, int nbrOfEntries, int rowEntries[]){

    for (int i = 0; i < nbrOfEntries; i++){
        fscanf(inputFile, "%d", &rowEntries[i]);
    }
}

void writeReportRow(int rowNbr, int nbrOfEntries, int rowEntries[]){
    int sum = 0;
    int max = -999;
    float avg = 0;

    for(int i = 0; i < nbrOfEntries ; i++){
        sum += rowEntries[i];
        if(max < rowEntries[i])
            max = rowEntries[i];
    }
    avg = (float) sum / nbrOfEntries;

    printf("%2d  %3d   %-5.2f  %-4d\n", rowNbr, sum, avg, max);
}