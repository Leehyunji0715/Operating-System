///////////////////////////////////////////////////////////////////
// Student name: Hyunji Lee
// Course: COSC 3503 - Operating Systems
// Assignment: Demonstration
// Source Code File Name: Lee5.c
// Program's Purpose: Showing ebvironment variable. There are 3 options to show.
//                      If user put more than 2 entries, the program show the proper input and is terminated
//                      If user put only a.out (execute file), get the environment variable's name and print the value out.
//                      If user put -a option, the program shows every environemnt variable
//                      If user put a.out and environment variable's name, the program shows the result if the variable exists or not.
// Program's Limitations: argc should be 1 or 2
// Development Computer: Mac book 
// Operating System: Mac OS
// Compiler: Gnu gcc
// Program's Operational Status: All requirements have been met
///////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_NUMBER 100

void showbByWho(void);
void ifFailAtexit(int status);

extern char ** environ;

int main(int argc, char * argv[]){
    int status = 0;
    char enviroName [MAX_NUMBER];
    char * enviroValue;

    if (argc == 1) {
        printf("%%%s\n", argv[0]);
        printf("Enter the name of an environment variable: ");
        scanf("%s", enviroName);

        enviroValue = (char *) getenv(enviroName);
        printf("\n");

        if(enviroValue == NULL){
            printf("%s is not an existing environment variable name\n", enviroName);
            status = atexit(showbByWho);
        }
        else {
            printf("%s : %s\n", enviroName, enviroValue);
            status = atexit(showbByWho);
        }

        ifFailAtexit(status);
    }
    else if (argc == 2){

        if( !strncmp(argv[1], "-a", MAX_NUMBER) ){
            for(int i = 0 ; environ[i] != NULL; i++){
                printf("%s\n",environ[i]);
            }
        }
        else {

            strncpy(enviroName, argv[1], MAX_NUMBER);
            printf("%%%s\n",argv[0]);
            enviroValue = (char *) getenv(enviroName);
            printf("\n");

            if(enviroValue == NULL)
                printf("%s is not an existing environment variable name\n", enviroName);
            else 
                printf("%s : %s\n", enviroName, enviroValue);  
        }
        status = atexit(showbByWho);
        ifFailAtexit (status);
    }
    else if(argc > 2){
        printf("Usage:\n");
        printf("a.out\n");
        printf("a.out <Environment_variable_name>\n");
        printf("a.out -a\n");
        exit(0);
    }
}

void showbByWho(void) {
    printf("**************************\n");
    printf("Environment Variable Display Program (by Hyunji Lee)\n");
}

void ifFailAtexit(int status){
    if(status != 0){
        fprintf(stderr, "Error: Failed to install exit handler\n");
        exit(1);
    }
}