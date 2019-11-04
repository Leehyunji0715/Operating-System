///////////////////////////////////////////////////////////////////
// Student name: Instructor
// Course: COSC3503
// Assignment: #13 - Client-Server FIFOs
// Source Code File Name: server-program.c
// Program's Purpose: Implements the server software that will accept a request
//          from a client via a server FIFO and return an answer to
//          the client via a FIFO set up by the client
// Program's Limitations: See the #define constants
// Assumptions: The client program uses the FIFO communication protocol that
//              is expected by this server program
// Development Computer: Dell 
// Operating System: CYGWIN on Microsoft Windows
// Compiler: gcc
// Build Direction: gcc server-program.c
// Program's Operational Status: All requirements have been satisfied
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE  512
#define NAME_SIZE    100
#define SERVER_FIFO_NAME "server.fifo"
#define SHUTDOWN_PHRASE "SHUTDOWN"
#define FALSE 0
#define TRUE 1

// Prototypes
void performServerDuties(void);


// *******************************************************
int main(int argc, char *argv[])
{
int status;

fprintf(stderr, "\nSERVER: The server is running\n");

status = mkfifo(SERVER_FIFO_NAME, (S_IRUSR | S_IWUSR));
if (errno == EEXIST)
   fprintf(stderr, "\nSERVER: An existing server FIFO has been detected\n");
else if (status == -1)
   {
   perror("SERVER: Error creating the server FIFO");
   return 1;
   } // End if
else
   fprintf(stderr, "\nSERVER: The server FIFO has been created\n");

performServerDuties();

fprintf(stderr, "\nSERVER: The server has terminated\n");

return 0;
} // End main


// ********************************************************************
void performServerDuties(void)
{
   int nbrBytesRead;
   int nbrBytesWritten;
   char buffer[BUFFER_SIZE];
   char clientFifoName[NAME_SIZE];
   int serverFifoId;
   int clientFifoId;
   time_t currentTime;
   int remainingInService = TRUE;

while (remainingInService)
   {
   fprintf(stderr, "\nSERVER: ========================================\n");

   fprintf(stderr, "\nSERVER: Open the server FIFO in read mode\n");

   serverFifoId = open(SERVER_FIFO_NAME, O_RDONLY);
   if (serverFifoId == -1)
      {
      perror("SERVER: Error opening server FIFO in read mode");
      return;
      } // End if

   fprintf(stderr, "\nSERVER: Wait for a request from a client\n");

   nbrBytesRead = read(serverFifoId, buffer, BUFFER_SIZE);
   buffer[nbrBytesRead] = '\0';

   if (strncmp(buffer, SHUTDOWN_PHRASE, BUFFER_SIZE) == 0)
      {
      fprintf(stderr, "\nSERVER: Received shutdown request from a client\n",
              buffer);
      remainingInService = FALSE;
      continue;
      } // End if

   fprintf(stderr, "\nSERVER: Received time request from client# %s\n", buffer);

   sprintf(clientFifoName, "client-%s.fifo", buffer);

   fprintf(stderr, "\nSERVER: Open the client FIFO named %s in write mode\n", 
           clientFifoName);

   clientFifoId = open(clientFifoName, O_WRONLY);
   if (clientFifoId == -1)
      {
      perror("SERVER: Error opening client FIFO in write mode");
      continue;
      } // End if

   fprintf(stderr, "\nSERVER: Send response to the client\n");

   currentTime = time(NULL);

   strcpy(buffer, ctime(&currentTime));

   nbrBytesWritten = write(clientFifoId, buffer, strlen(buffer));

   fprintf(stderr, "\nSERVER: Close client FIFO\n");
   close(clientFifoId);

   fprintf(stderr, "\nSERVER: Close server FIFO\n");
   close(serverFifoId);

   } // End while

fprintf(stderr, "\nSERVER: Close server FIFO\n");
close(serverFifoId);

} // End performServerDuties

