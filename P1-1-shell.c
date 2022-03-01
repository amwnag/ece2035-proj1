//=================================================================
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*    
Please fill in the following
 Student Name:
 Date:

ECE 2035 Project 1-1

This is the only file that should be modified for the C implementation
of Project 1.

Do not include any additional libraries.
------------------------------------------

This program finds an exact match of George's face which may be rotated
in a crowd of tumbling (rotated) faces. It reports the location of his
hat and shirt as pixel positions.  The pixels are specified as linear
offsets from the crowd image base.*/

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1 // RESET THIS TO 0 BEFORE SUBMITTING YOUR CODE

int main(int argc, char *argv[]) {
   int	             CrowdInts[1024];
   // This allows you to access the pixels (individual bytes)
   // as byte array accesses (e.g., Crowd[25] gives pixel 25):
   char *Crowd = (char *)CrowdInts;
   int	             NumInts, HatLoc=0, ShirtLoc=0;
   int               Load_Mem(char *, int *);

   if (argc != 2) {
     printf("usage: ./P1-1 testcase_file\n");
     exit(1);
   }
   NumInts = Load_Mem(argv[1], CrowdInts);
   if (NumInts != 1024) {
      printf("valuefiles must contain 1024 entries\n");
      exit(1);
   }
   if (DEBUG){
     printf("Crowd[0] is Pixel 0: 0x%02x\n", Crowd[0]);
     printf("Crowd[107] is Pixel 107: 0x%02x\n", Crowd[107]);
   }

   /* your code goes here. */

   printf("George is located at: hat pixel %d, shirt pixel %d.\n", HatLoc, ShirtLoc);
   exit(0);
}

/* This routine loads in up to 1024 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 1024; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}
