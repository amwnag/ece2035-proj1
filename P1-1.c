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

void				matchRow(char *, int, int *, int *);

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
   for (int currRow = 0; currRow < 64; currRow++) {
	   for (int currCol = 0; currCol < 64; currCol += 9) {
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
					// to optimize, skip entire face (some value...) if no match
					matchRow(Crowd, ind, &HatLoc, &ShirtLoc);
					// currCol += 6;
					break;
				default:
					// non-george face pixels or bg pixels
					// to optimize, full skip nonmatching face pixels
					break;
		   }
	   }
   }
   

   printf("George is located at: hat pixel %d, shirt pixel %d.\n", HatLoc, ShirtLoc);
   exit(0);
}

void matchRow(char *Crowd, int ind, int * hat, int * shirt) {

	unsigned int pixelsChunk = 0;
	unsigned int extraPixels = 0;
	void				horizontalMatch(char *, int, int *, int *);
	void 				eyesMatch(char *, int, int *, int *);
	for (int i = 0; i < 12; i++) {
		// backtracking, but what if there are two faces with side by side pixels?
		// need some way to detect that...
		// as of now, algorithm would fail in this special edge case
		if (Crowd[ind] > 0x8) {
			ind++;
			break;
		}
		
		ind--;
	}
	
	// only interested if the first pixel is blue, yellow, or white
	if (Crowd[ind] > 0x5) {
		return;
	}



	for (int scanAhead = 0; scanAhead < 12; scanAhead++) { // max length of face pixels
		if (((ind + scanAhead) % 64) >= 64){
			break; // stay on the same row
		}
		
		int pixel = Crowd[ind + scanAhead]; // be careful not to modify original arr
			
		if (scanAhead < 8) {
			pixelsChunk = pixelsChunk << 4;
			pixelsChunk = pixelsChunk | pixel;
		} else {
			extraPixels = extraPixels << 4;
			extraPixels = extraPixels | pixel;
		}
		
		// detect if the face has been left
		if ((pixelsChunk & 0xF) > 0x8) {
			return; // no match
		}
		if ((extraPixels & 0xF) > 0x8) {
			extraPixels = extraPixels >> 4; // clear off the last pixel
			break;
		}
	}

	// int pixelsChunk = Crowd[ind] | (Crowd[ind + 1] << 4) | (Crowd[ind + 2] << 8) | (Crowd[ind + 3] << 12);
	
	if (DEBUG) {
		printf("Face at %d, row %d. Current chunk is 0x%08x.", ind, ind / 64, pixelsChunk);
		printf(" Extra pixels are 0x%08x\n", extraPixels);
	}

	if (pixelsChunk == 0x12125558 && extraPixels == 0x8553) {
		horizontalMatch(Crowd, ind, hat, shirt);
	} else if (pixelsChunk == 0x35588555 && extraPixels == 0x2121) {
		horizontalMatch(Crowd, ind, hat, shirt);
	} else if (pixelsChunk == 0x55755575 && extraPixels == 0x5) {
		// pixel for middle of eyes
		eyesMatch(Crowd, ind + 4, hat, shirt);
	}
}

void eyesMatch(char *Crowd, int ind, int * hat, int * shirt) {
	if (DEBUG) {
		printf("Entered eyes match for face at %d\n", ind / 64);
	}
		
	// traverse down the column
	// varies based on the orientation
	// therefore traverse up 7 and traverse down 7
	
	int colUp = 0;
	int colDown = 0;
	
	for (int i = 1; i < 8; i++) {
		int pixelUp = Crowd[ind + (i * 64)];
		int pixelDown = Crowd[ind - (i * 64)];
		colUp = colUp << 4;
		colUp = colUp | pixelUp;
		colDown = colDown << 4;
		colDown = colDown | pixelDown;
	}
	
	
	// if george is right-side up
	// colUp contents would be red white red white x x x
	// colDown contents would be y y black black y y blue
	
	// if george is up-side down
	// colUp contents would be y y black black y y blue
	// colDown would equal right-side up colUp
	
	int seq1 = 0x2121; // would need to shift left by 12 before comparing
	int seq2 = 0x5588553;

	if (colDown == seq2) {
		if (colUp >> 12 == seq1) {
			// right side up match
			*hat = ind + (4 * 64);
			*shirt = ind - (7 * 64);
		}
	} else if (colUp == seq2) {
		if (colDown >> 12 == seq1) {
			// up side down match
			*hat = ind - (4 * 64);
			*shirt = ind + (7 * 64);
		}
	}

}

void horizontalMatch(char *Crowd, int ind, int * hat, int * shirt) {
	if (DEBUG) {
		printf("Entered horizontal match for face at %d\n", ind / 64);
	}
	// process columns where eyes would be
	int colAddr1 = ind + 4 - (4 * 64); // -252
	int colAddr2 = ind + 7 - (4 * 64); // -249
	int col1 = 0;
	int col2 = 0;
	for (int i = 0; i < 5; i++) { // only need to scan half of face
		int pixel1 = Crowd[colAddr1 + (i * 64)];
		int pixel2 = Crowd[colAddr2 + (i * 64)];
		col1 = col1 << 4;
		col1 = col1 | pixel1;
		col2 = col2 << 4;
		col2 = col2 | pixel2;
	}
	
	if (col1 != 0x55755 && col2 != 0x55755) {
		// eyes not matching
		return;
	}
	
	if (Crowd[ind] == 0x1) {
		// hat
		*hat = ind;
		*shirt = ind + 11;
	} else if (Crowd[ind] == 0x3) {
		// shirt
		*hat = ind + 11;
		*shirt = ind;
	}
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
