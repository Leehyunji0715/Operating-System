///////////////////////////////////////////////////////////////////
// Student name: 
// Course: COSC 3503 - Operating Systems
// Assignment: #3 - Purposes of HTML Tags
// Source Code File name: 
// Program's Purpose: Finds and prints a list of the HTML tags found in
//                    a file and the purpose of certain tags
// Program's Limitations: Assumes that the input file is a
//                        properly-formatted HTML file
// Development Computer: 
// Operating System: 
// Compiler: 
// Program's Operational Status: 
/////////////////////////////////////////////////////////////////// 

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define TRUE  1
#define FALSE 0
#define LEFT_ANGLE_BRACKET '<'
#define RIGHT_ANGLE_BRACKET '>'
#define SPACE ' '
#define NULL_BYTE '\0'
#define MAX_LENGTH 100


// Function Prototypes
int detectTags(void);
void displayPurpose(char *tag);


// #############################################################
int main(void)
{

int tagCount = 0;

printf("\n*** START OF HTML TAGS ***\n\n");

tagCount = detectTags();

printf("\nTOTAL NUMBER OF HTML TAGS FOUND: %d \n", tagCount);

return 0;
} // End main


// #############################################################
int detectTags(void)
{
    char identifier[MAX_LENGTH];
    int index = 0;
    int count = 0;
    char symbol;

    while (!feof(stdin)) {

        while( symbol != LEFT_ANGLE_BRACKET && !feof(stdin) ){
            symbol = getchar();
        }

        if ( symbol == LEFT_ANGLE_BRACKET ){
            index = 0;

            while ( symbol != RIGHT_ANGLE_BRACKET) 
         {
            assert(index < MAX_LENGTH);
            symbol = toupper(symbol);

            identifier[index] = symbol;
            index++;
            symbol = getchar();
         } // End while

        identifier[index] = symbol;
        index++;

        assert(index < MAX_LENGTH);
        identifier[index] = NULL_BYTE;

        displayPurpose(identifier);

        count++;
      } // End if
    }
    return count;
} // End detectTags


// #############################################################
void displayPurpose(char *tag)
{
    if( strncmp(tag, "<HTML>", MAX_LENGTH) == 0)
        printf("%-25s: Start of HTML file\n", tag);
    else if ( strncmp (tag, "<HEAD>", MAX_LENGTH ) == 0 )
        printf("%-25s: Start of HTML header\n", tag);
    else if ( strncmp (tag, "<TITLE>", MAX_LENGTH ) == 0 )
        printf("%-25s: Start of document title\n", tag);
    else if ( strncmp (tag, "</TITLE>", MAX_LENGTH ) == 0 )
        printf("%-25s: End of document title\n", tag);
    else if ( strncmp (tag, "</HEAD>", MAX_LENGTH ) == 0 )
        printf("%-25s: End of HTML header\n", tag);
    else if ( strncmp (tag, "<UL>", MAX_LENGTH ) == 0 )
        printf("%-25s: Start of unnumbered list\n", tag);
    else if ( strncmp (tag, "<P>", MAX_LENGTH ) == 0 )
        printf("%-25s: Start of a new paragraph\n", tag);
    else if ( strncmp (tag, "</UL>", MAX_LENGTH ) == 0 )
        printf("%-25s: End of unnumbered list\n", tag);
    else if ( strncmp (tag, "</HTML>", MAX_LENGTH ) == 0 )
        printf("%-25s: End of HTML file\n", tag);
    else
        printf("%-25s: Unknown HTML tag\n", tag);
    
} // End displayPurpose
