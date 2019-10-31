///////////////////////////////////////////////////////////////////
// Student name: Hyunji Lee
// Course: COSC 3503
// Assignment: #12 - Files and Directories
// Source Code File Name: Lee12.c
// Program's Purpose: Lists status information for all entries in a directory 
// Program's Limitations: 
// Development Computer:  mac book pro
// Operating System: mac OS
// Compiler: gcc
// Build Instructions: gcc Lee12.c
// Program's Operational Status: 
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>


// Constants
#define MAX_SIZE 100



// Function Prototypes
void printType(const struct stat *buf);
void printFileSize(const struct stat *buf);
void printAccess (const struct stat *buf);
void printStatusChanged (const struct stat *buf);



// *******************************************************
int main(int argc, char *argv[])
{
    DIR * dp = NULL;
    struct dirent * entry = NULL;
    struct stat statusBuffer;
    char * pathPtr = NULL;
    char * fName = NULL;
    char buffer[MAX_SIZE];

    int status = 0;
    int inFile = 0;
    int checkUnknown = 0;

    //current directory 
    if(argc == 1){
        status = chdir(".");
        if (status == -1){
            printf("Error changing current working directory: No such file or directory\n");
            return 1;
        }
    }
    else if (argc == 2){
        status = chdir(argv[1]);
        if (status == -1){
            printf("Error changing current working directory: No such file or directory\n");
            return 1;
        }
    }
    else if (argc > 2){
        printf("Usage: a.out [directory_path] \n");
        return 1;
    }

    pathPtr = (char *) getcwd(buffer, MAX_SIZE);
    printf("\nContents of: %s\n", buffer);
    printf("\n");
    printf("Type                     Size   Access      Status changed    Name\n");
    printf("----                     ----   ------      --------------    ----\n");
    
    if( (dp = opendir(buffer)) == NULL){
        printf("Fail to open the directory : %s\n", *dp);
        return 1;
    }

    while((entry = readdir(dp))!= NULL){
        fName = entry->d_name;

        inFile = open(fName, O_RDONLY);
        if(inFile == -1){
            printf("Fail to open \"%s\" file \n", fName);
            return 1;
        }

        status = fstat(inFile, &statusBuffer);
        if(status == -1){
            printf("Fail to get %s file status\n", fName);
            return 1;
        }
        printType (&statusBuffer);
        printFileSize (&statusBuffer);
        printAccess(&statusBuffer);
        printStatusChanged (&statusBuffer);
        printf("   %s\n", fName);

    }
    
    closedir(dp);
    return 0;
} // End main


void printType(const struct stat *buf){
    if (S_ISBLK(buf->st_mode))
        printf("%-26s", "Block special file");
    else if(S_ISDIR(buf->st_mode))
        printf("%-26s", "Directory");
    else if (S_ISCHR(buf->st_mode))
        printf("%-26s", "Character special");
    else if (S_ISFIFO(buf->st_mode))
        printf("%-26s", "Pipe or FIFO");
    else if (S_ISLNK(buf->st_mode))
        printf("%-26s", "Symbolic link");
    else if (S_ISREG(buf->st_mode))
        printf("%-26s", "Regular file");
    else if (S_ISSOCK(buf->st_mode))
        printf("%-26s", "Socket");
    else
        printf("%-26s", "Unknown type");
}

void printFileSize(const struct stat *buf){
    printf("%-6lld", buf->st_size);
}

void printAccess (const struct stat *buf){
    char access [MAX_SIZE];
    int idx = 0;
    for(int i = 0; i < 3; i++){
        if(buf->st_mode & (S_IREAD >> i*3))
            access[idx++] = 'r';
        else
            access[idx++] = '-';

        if(buf->st_mode & (S_IWRITE >> i*3))
            access[idx++] = 'w';
        else
            access[idx++] = '-';

        if(buf->st_mode & (S_IEXEC >> i*3))
            access[idx++] = 'x';
        else
            access[idx++] = '-';
    }
    access[idx] = '\0';
    printf("%-12s", access);
}

void printStatusChanged (const struct stat *buf){
    char * date = ctime(&(buf->st_mtime));
    for(int i = 4; i<19; i++)
        printf("%c", date[i]);
}
