///////////////////////////////////////////////////////////////////
// Student Name: Hyunji Lee
// Course: COSC 3503 - Operating Systems
// Assignment: #1 - Using a Command Shell
// Source Code File Name: Lee2.cpp
// Program's Purpose: get a file name from command line, and read the (html)file 
//                    and print out the content of the file and the number of tags
// Program's Limitations: 
// Development Computer: Mac book
// Operating System: Mac OS
// Compiler: Gnu g++
// Program's Operational Status:  
/////////////////////////////////////////////////////////////////// 

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int printTags(int argc, string fileName);

int main(int argc, char * argv[])
{
  string fileName = argv[1];
  int countTag = 0;
  
  countTag = printTags(argc, fileName);

  cout << '\n';
  cout << "TOTAL NUMBER OF HTML TAGS FOUND: " << countTag << '\n';

  return 0;
} // End main


int printTags(int argc, string fileName ){

  int countTag = 0;
  char readC = ' ';

  if(argc > 1){
  }
  else {
    printf("No file to read. Try again.\n");
    return 0;
  }

  fstream fin( fileName , fstream::in );
  
  cout << "*** START OF HTML TAGS ***";
  cout << "\n\n";
  
  while (fin >> readC) {
    cout << readC;

    if(readC == '>'){
      cout << '\n';
      countTag++;
    }
  }

  return countTag;

}