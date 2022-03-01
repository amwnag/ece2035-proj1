//=================================================================
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*    
Please fill in the following
 Student Name: Amanda Wang
 Date: 02/24/2022

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


// global variables
int whiteSequences[] = {};
int redSequences[] = {};
int blueSequences[] = {};
int yellowSequences[] = {};

int main(int argc, char *argv[]) {
   int	             CrowdInts[1024];
   // This allows you to access the pixels (individual bytes)
   // as byte array accesses (e.g., Crowd[25] gives pixel 25):
   char *Crowd = (char *)CrowdInts;
   int	             NumInts, HatLoc=0, ShirtLoc=0;
   int               Load_Mem(char *, int *);
   int				matchGeorge(char *, int);

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
   
   // strategy: scan every 11th row
   // Will be able to always find a face pixel (if exists)
   for (int currRow = 0; currRow < 64; currRow += 11) {
	   for (int currCol = 0; currCol < 64; currCol++) {
		   int ind = currRow * 64 + currCol;
		   switch (Crowd[ind]) {
				case 1:
				case 2:
				case 3:
				case 5:
					// begin scanning, since there is a face that is possibly george
					// skip 3 if no match
					matchGeorge(Crowd, ind);
					currCol += 2;
					break;
				case 9:
				case 10:
				case 11:
					// bg pixels
					break;
				default:
					// non-george face pixels
					// skip by offsets of 3 until finding a bg pixel 
					// or until have performed maxiumum three skips of 3
					currCol += 2;
					break;
		   }
	   }
   }
   

   printf("George is located at: hat pixel %d, shirt pixel %d.\n", HatLoc, ShirtLoc);
   exit(0);
}

int matchGeorge(char *Crowd, int ind) {
	
	
	// scan 3 pixels ahead then compare (3 bytes, which fits in a word)
	// guaranteed that 3rd pixel will not be out of bounds for a face
	// chose to put first pixel at ind + 1 as lsb
	int pixelsChunk = Crowd[ind] | (Crowd[ind + 1] << 8) | (Crowd[ind + 2] << 16) | (Crowd[ind + 3] << 24);
	
	if (DEBUG) {
		printf("Helper function matchGeorge called. Current chunk of pixels is 0x%08x\n", pixelsChunk);
	}
	// int matchArr[]; // note syntax is different from java... array size needs to be specified...
	// or maybe instead use a pointer
	// terminate loop as soon as lsb doesn't match
	int* startMatch;
	switch (Crowd[ind]) {
		case 1:
			startMatch = &(whiteSequences[0]); // want the base address
			break;			
		case 2:
			startMatch = &(redSequences[0]);
			break;	
		case 3:
			startMatch = &(blueSequences[0]);
			break;	
		case 5:
			startMatch = &(yellowSequences[0]);
			break;	
		default:
			break;
		
	}
	
	// can process all bg pixels to be the same color
	
	
	// pointer arithmetic...
	// compare last byte... mask
	int lastByte = 0xFF;
	int i = 0;
	int curr = *(startMatch + i);
	while ((curr & lastByte) == Crowd[ind]) { // little endian byte access
		if (pixelsChunk == curr) {
			// wow this may be george
			// a little tricky with bg 
			// if there is a match... call another helper function to branch out
		}
		
		// update
		curr = *(startMatch + ++i);
	}
	
	return 0;

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
