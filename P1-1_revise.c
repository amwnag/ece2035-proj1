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
	} else if (Crowd[ind] == 0x5) {
		// yellow, confirmed not wearing shades
		// left eye is green 
		if (Crowd[ind + 2] != 0x7) {
			return;
		}
		// now all tests pass for eyes
		eyesMatch(Crowd, ind + 4, hat, shirt);
	} else if (Crowd[ind] == 0x3) {
		// blue
		// yellow face, smile, last 3 hat pixels
		if (Crowd[ind + 7] != 0x5) {
			return;
		}
		if (Crowd[ind + 3] != 0x8 || Crowd[ind + 4] != 0x8) {
			return;
		}
		if (Crowd[ind + 10] != 0x2 || Crowd[ind + 11] != 0x1) {
			return;
		}
		horizontalMatch(Crowd, ind, hat, shirt);
	} else if (Crowd[ind] == 0x1) {
		// white
		
		// hat color
		if (Crowd[ind + 1] != 0x2) {
			return;
		}
		if (Crowd[ind + 4] != 0x5) {
			return;
		}
		if (Crowd[ind + 7] != 0x8 || Crowd[ind + 8] != 0x8) {
			return;
		}
		if (Crowd[ind + 11] != 0x3) {
			return;
		}
		horizontalMatch(Crowd, ind, hat, shirt);
	}


}

void eyesMatch(char *Crowd, int ind, int * hat, int * shirt) {
	if (DEBUG) {
		printf("Entered eyes match for face at %d\n", ind / 64);
	}
		
	// traverse down the column
	// varies based on the orientation
	// therefore traverse up 7 and traverse down 7
	
	// check row above to determine orientation

	if (Crowd[ind - 64] == 0x2) {
		// rightside up
		if (DEBUG) {
			printf("George is right-side up %d\n", ind / 64);
		}
		ind -= 4*64;
		if (Crowd[ind] != 0x1 || Crowd[ind + 1*64] != 0x2) {
			return;
		}
		if (Crowd[ind + 4*64] != 0x5) {
			return;
		}
		if (Crowd[ind + 7*64] != 0x8 || Crowd[ind + 8*64] != 0x8) {
			return;
		}
		if (Crowd[ind + 11*64] != 0x3) {
			return;
		}
		*hat = ind;
		*shirt = ind + (11 * 64);
	} else {
		// upside down
		ind -= 7*64;
		if (Crowd[ind + 7*64] != 0x5) {
			return;
		}
		if (Crowd[ind + 3*64] != 0x8 || Crowd[ind + 4*64] != 0x8) {
			return;
		}
		if (Crowd[ind + 10*64] != 0x2 || Crowd[ind + 11*64] != 0x1) {
			return;
		}
		*hat = ind + (11 * 64);
		*shirt = ind;
		
	}

}

void horizontalMatch(char *Crowd, int ind, int * hat, int * shirt) {
	if (DEBUG) {
		printf("Entered horizontal match for face at %d\n", ind / 64);
	}
	// process columns where eyes would be
	// need to check only 1 pixel for eyes (middle section takes care of shades)
	// travel right by 4 pixels. if it's not yellow, then travel an additional 3
	
	if (Crowd[ind + 4] == 0x5) {
		// starting from hat
		if (Crowd[ind + 4 + 64 * 2] != 0x7) {
			return;
		}
		*hat = ind;
		*shirt = ind + 11;
	} else {
		// starting from shirt
		if (Crowd[ind + 7 + 64 * 2] != 0x7) {
			return;
		}
		
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
