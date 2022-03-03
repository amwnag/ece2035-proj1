#=================================================================
# Copyright 2022 Georgia Tech.  All rights reserved.
# The materials provided by the instructor in this course are for
# the use of the students currently enrolled in the course.
# Copyrighted course materials may not be further disseminated.
# This file must not be made publicly available anywhere.
# =================================================================
#     F i n d  T u m b l i n g G e o r g e  i n  a  C r o w d
#
# P1-2
# Student Name: Amanda Wang
# Date: 3/3/2022
#
# This routine finds an exact match of George's face which may be
# rotated in a crowd of tumbling (rotated) faces.
#
#===========================================================================
# CHANGE LOG: brief description of changes made from P1-2-shell.asm
# to this version of code.
# Date  Modification
# 02/12 Looping through pixels to find one w/ color $3              (example)
# 02/15 Reduced avg DI by only looking at pixels starting at row 10 (example)
#===========================================================================

.data
Array:  .alloc	1024

.text

FindGeorge:	addi	$1, $0, Array		# point to array base
		swi	570			# generate tumbling crowd

# ***************************************************************************
	# SWI 552 is useful for debugging. Remove calls to it in
	# final program submission.
    #	addi    $2, $0, 160             # Example: mark the 160th pixel
	#	swi	552			

# ***************************************************************************
			addi $3, $0, 0		# set up counter for outer loop iterating through rows
OuterLoop: 	addi $4, $0, 0		# set up counter for inner loop iterating through columns
InnerLoop:	add  $2, $3, $4		# calculate effective address of pixel, $2 + $3
			swi 552
			addi $4, $4, 1		# update inner loop counter
			slti $5, $4, 64		# check inner loop exit
			bne  $5, $0, InnerLoop 
			addi $3, $3, 64		# update statement for outer loop, row is 64 pixels
			slti $5, $3, 4096		# check outer loop exit condition, total 64 rows, i < 64
			bne  $5, $0, OuterLoop 
		


		lui     $2, 300             # TEMP: guess the 300th pixel for hat
	    ori     $2, $2, 1004        # TEMP: and 1004th pixel for shirt
	
		swi	571			# submit answer and check
		# oracle returns correct answer in $3

		jr	$31			# return to caller
