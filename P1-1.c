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
// can use less memory if each word stores one sequence of 4 pixels
int whiteSequences[] = {0x12125558};
int redSequences[] = {0x2111112F, 0x2211122F, 0x22127585, 0x22222222, 0x2222222F, 0x22225555, 0x255555F, 0x2F};
int blueSequences[] = {0x33333F, 0x35558555, 0x35588555}; //0x3F555857
int yellowSequences[] = {0x55585721, 0x55555555, 0x55555222, 0x555552F, 0x55555F, 0x55558555, 0x55588855, 0x555F, 0x55755575, 0x55855585};

int hatShirt[] = {0x12125558, 0x8553, 0x35588555, 0x2121, 0x2211122F, 0x33333F};

int				matchRow(char *, int);
int				verifyGeorge(char *, int, int *, int *);

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
   
   // strategy: scan every 11th row
   // Will be able to always find a face pixel (if exists)
   for (int currRow = 0; currRow < 64; currRow += 10) {
	   for (int currCol = 0; currCol < 64; currCol++) {
		   if (HatLoc != 0 && ShirtLoc != 0) { // has been assigned
			   break;
		   }
		   int ind = currRow * 64 + currCol;
		   switch (Crowd[ind]) {
				case 1:
				case 2:
				case 3:
				case 5:
					// begin scanning, since there is a face that is possibly george
					// to optimize, skip by something if no match
					currCol += verifyGeorge(Crowd, ind, &HatLoc, &ShirtLoc) - 1;
					break;
				default:
					// non-george face pixels or bg pixels
					break;
		   }
	   }
   }
   

   printf("George is located at: hat pixel %d, shirt pixel %d.\n", HatLoc, ShirtLoc);
   exit(0);
}

int verifyGeorge(char * Crowd, int ind, int * hat, int * shirt) {

	int firstRow = matchRow(Crowd, ind);
	if (firstRow) {
		return firstRow;
	}
	
	for (int rowsUp = 1; rowsUp < 12; rowsUp++) {
		Crowd[
	}
	// now scan entirety of the match
	// traverse up and down rows
	
	// *hat =
	// *shirt =
	return 1;
	
}

int matchRow(char *Crowd, int ind) {

	unsigned int pixelsChunk = 0;
	unsigned int extraPixels = 0;
	
	int scanAhead;
	for (scanAhead = 0; scanAhead < 12; scanAhead++) { // max length of face pixels
		if (((ind + scanAhead) % 64) >= 64){
			break; // stay on the same row
		}
		
		int pixel = Crowd[ind + scanAhead]; // be careful not to modify original arr
		if (pixel == 0x9 || pixel == 0xa || pixel == 0xb) {
			pixel = 0xF;
		}
			
		if (scanAhead < 8) {
			pixelsChunk = pixelsChunk << 4;
			pixelsChunk = pixelsChunk | pixel;
		} else {
			extraPixels = extraPixels << 4;
			extraPixels = extraPixels | pixel;
		}
		
		// detect if the face has been left
		if ((pixelsChunk & 0xF) == 0xF) {
			break;
		}
		if ((extraPixels & 0xF) == 0xF) {
			break;
		}
		
	}
	

	// int pixelsChunk = Crowd[ind] | (Crowd[ind + 1] << 4) | (Crowd[ind + 2] << 8) | (Crowd[ind + 3] << 12);
	
	if (DEBUG) {
		printf("Face at %d. Current chunk is 0x%08x. ", ind / 64, pixelsChunk);
		if (extraPixels != 0) {
			printf("Wowza some extra pixels 0x%08x", extraPixels);
		}
		printf("\n");
	}

	
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
	
	int i = 0;
	int curr = *(startMatch + i);
	while (i < 10) { // make sure first pixel is the same somehow...
		// well now above doesn't work anymore hmph
		if (pixelsChunk == curr) {
			if (DEBUG) {
				// printf("---> Wow this may be George. Let's do a more thorough scan now\n");
			}
			return 0;
		}
		
		// update
		curr = *(startMatch + ++i);
	}
	
	return scanAhead;
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
