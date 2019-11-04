///////////////////////////////////////////////////////////////////
// Student name: Hyunji Lee
// Course: COSC3503
// Assignment: #13 - Client-Server FIFOs
// Source Code File name: Lee13.c
// Program's Purpose: Implements the client software that will send a request
//          to the server via a server FIFO and read the answer sent
//          back by the server via a FIFO set up by the client
// Program's Limitations: See the #define constants
// Assumptions: The server program uses the FIFO communication protocol that is
//              expected by this client program
// Development Computer:  mac book pro
// Operating System: mac OS 
// Compiler: gcc
// Program's Operational Status:
///////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE  512
#define NAME_SIZE    100 
#define SERVER_FIFO_NAME "server.fifo"
#define SHUTDOWN_PHRASE "SHUTDOWN"
#define MAX_OPTION_SIZE 3
#define MAX_SIZE 100
#define FALSE 0
#define TRUE 1

// Prototypes
void performClientDuties(char *clientFifoName, int shutdownRequested);
int checkCommand (int argc, char *argv[]);
void makeFIFOFileName (char * originalFile, char *sPID);


// *******************************************************
int main(int argc, char *argv[])
{
    char clientFileName[MAX_SIZE] = "client-";
    char sPID[MAX_SIZE];
    int status = 0;
    int request = FALSE;

    request = checkCommand(argc, argv);
    if(request == -1){
        return 1;
    }

    sprintf(sPID, "%d", getpid());
    fprintf(stdout, "CLIENT: PID# %s is running\n", sPID);

    makeFIFOFileName(clientFileName, sPID);
    status = mkfifo(clientFileName, (S_IRUSR | S_IWUSR));
    if (errno == EEXIST)
        fprintf(stderr, "\nCLIENT: An existing client FIFO has been detected\n");
    else if (status == -1){
        perror("CLIENT: Error creating the client FIFO\n");
        return 1;
    }
    else
        fprintf(stderr, "\nCLIENT: The client FIFO named %s has been created\n", clientFileName);

    //printf("Request : %d\n",request);
    performClientDuties(clientFileName, request);

    fprintf(stderr, "\nCLIENT: PID# %s has terminated\n\n", sPID);
    

    return 0;
} // End main


// ********************************************************************
void performClientDuties(char *clientFifoName,int KindOfRequest)
{
    int status = 0;
    int pid = getpid();
    int serverFifoId;
    int clientFifoId;
    int nbrBytesRead;
    int nbrBytesWritten;

    char * sPID = malloc(MAX_SIZE+1);;
    char buffer[BUFFER_SIZE];
    time_t currentTime;

    snprintf(sPID, MAX_SIZE, "%d", pid);
    fprintf(stderr, "\nCLIENT: ========================================\n");
    
    fprintf(stderr, "\nCLIENT: Open the server FIFO in write mode\n");
    serverFifoId = open(SERVER_FIFO_NAME, O_WRONLY);

    if (serverFifoId == -1){
        perror("CLIENT: Error opening server FIFO in write mode");
        return;
    } // End if

    if(KindOfRequest == 0){ //Shutdown Request

        fprintf(stderr, "\nCLIENT: Send shutdown request to the server\n");
        write(serverFifoId, SHUTDOWN_PHRASE, MAX_SIZE);
            
        fprintf(stderr, "\nCLIENT: Unlink client FIFO\n");
        status = unlink(clientFifoName);

        fprintf(stderr, "\nCLIENT: Close server FIFO\n");
        close(serverFifoId);
    }

    else{ //Time Request

        fprintf(stderr, "\nCLIENT: Send time request to the server\n");
        write(serverFifoId, sPID, MAX_SIZE);

        fprintf(stderr, "\nCLIENT: Open the client FIFO in read mode\n");
        clientFifoId = open(clientFifoName, O_RDONLY);
        if (clientFifoId == -1){
            perror("CLIENT: Error opening server FIFO in read mode");
            return;
        } // End if

        fprintf(stderr, "\nCLIENT: Wait for a response from the server\n");
        nbrBytesRead = read(clientFifoId, buffer, BUFFER_SIZE);
        buffer[nbrBytesRead] = '\0';

        fprintf(stderr, "\nCLIENT: Received response from server: %s", buffer);

        fprintf(stderr, "\nCLIENT: Close client FIFO\n");
        close(clientFifoId);

        fprintf(stderr, "\nCLIENT: Unlink client FIFO\n");
        status = unlink(clientFifoName);

        fprintf(stderr, "\nCLIENT: Close server FIFO\n");
        close(serverFifoId);
    }
} // End performClientDuties


int checkCommand (int argc, char *argv[]){
    if(argc == 2){
        if(strcmp(argv[1], "-s") != 0){
            fprintf(stderr, "\n\nUsage: a.exe [-s]\n\n");
            fprintf(stderr, "   -s    shut down the server\n\n");
            return -1; // terminate program
        }
        else
            return 0; // send shutdown request
    }
    else if (argc == 1)
        return 1; // send time request

    return -1;
}

void makeFIFOFileName (char * originalFile, char *sPID){
    strncat(originalFile, sPID, MAX_SIZE);
    strncat(originalFile, ".fifo", MAX_SIZE);
}