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
RowHat1: .word 0x12125558
RowHat2: .word 0x8553
RowShirt1: .word 0x35588555
RowShirt2: .word 0x2121 # also use in EyesMatch for Seq1
RowEyes1: .word 0x55755575 # can use within MidMatch after shifting right by 12
RowEyes2: .word 0x5
Seq2: .word 0x5588553

.text

FindGeorge:	addi	$1, $0, Array		# point to array base
		swi	570			# generate tumbling crowd

# ***************************************************************************
	# SWI 552 is useful for debugging. Remove calls to it in
	# final program submission.
    #	addi    $2, $0, 160             # Example: mark the 160th pixel
	#	swi	552			

# ***************************************************************************
			addi $2, $0, 0 		# initialize $2 to 0, indicating george not found
			addi $3, $0, 0		# set up counter for outer loop iterating through rows
OuterLoop: 	addi $4, $0, 0		# set up counter for inner loop iterating through columns
InnerLoop:	add  $6, $3, $4		# calculate effective address of pixel, $3 + $4
			lbu	 $7, Array($6)	# access the color in the array
			slti $5, $7, 9 		# color value is face, call match function
			beq  $5, $0, NoMatch
			addi $29, $29, -12 	# store inner and outer loop values in stack
			sw $31, 8($29)		# preserve return addr
			sw $3, 4($29)		# push outer loop counter
			sw $4, 0($29)		# push inner loop counter
			jal MatchRow		# recall jal stores return addr in $31
			lw $31, 8($29)
			lw $3, 4($29)
			lw $4, 0($29)
			addi $29, $29, 12	# adjust SP up
			bne $2, $0, ReportLoc # if $2 is non-zero
NoMatch: 	addi $4, $4, 9		# update inner loop counter (incrementing by 9)
			slti $5, $4, 64		# check inner loop exit
			bne  $5, $0, InnerLoop 
			addi $3, $3, 64		# update statement for outer loop, row is 64 pixels
			slti $5, $3, 4096		# check outer loop exit condition, total 64 rows, i less than 64
			bne  $5, $0, OuterLoop 
			
ReportLoc: 	swi	571			# submit answer and check
			# oracle returns correct answer in $3
			jr	$31			# return to caller

MatchRow: 	addi $6, $6, -1 # use effective address and decrement until bg pixel
			lbu	 $7, Array($6)	# access the color in the array
			slti $5, $7, 9	# if it is 9, 10, 11 --> stop decrementing, branch
			beq $5, $0, CheckFirst
			j MatchRow
CheckFirst:	addi $6, $6, 1 # increment by 1, checking the first pixel
			add $2, $0, $6
			swi	552	# SWI to debug, should see it beginning of face
			lbu $7, Array($6) 
			slti $5, $7, 6 # if greater than 5, return
			beq $5, $0, MatchRowEnd
			addi $3, $0, 0	# initialize loop for scanning pixels
			addi $8, $0, 0	# initialize first part of seq
			addi $9, $0, 0	# initialize second part of seq
ScanFirst: 	lbu $7, Array($6) 		# get pixel at the current ind, may be redundant for 1st iteration
			slti $5, $7, 9 			# return if pixel is bg
			beq  $5, $0, MatchRowEnd 
			sllv $7, $7, $3 		# shift pixel based on iteration
			or $8, $8, $7 	# use $8 for running word representing first 8 pixels
			addi $6, $6, 1 		# increment the addr
			addi $3, $3, 4 		# update, scaled for shifting
			slti $5, $3, 32 		# exit condition, 4*8
			bne $5, $0, ScanFirst 	# loop back
			addi $3, $0, 0 			# reset loop counter
ScanSecond: lbu $7, Array($6) 		# get pixel at the current ind
			slti $5, $7, 9 			# exit loop if pixel is bg
			beq  $5, $0, Comparison # note: if no extra pixels, exit function
			sllv $7, $7, $3
			or $9, $9, $7 			# remaining pixels
			addi $6, $6, 1
			addi $3, $3, 4 # update, scaled for shifting
			slti $5, $3, 48 # exit condition, 4*12
			bne $5, $0, ScanSecond # loop back
Comparison: lw $3, RowHat1($0)		# match with 3 possible sequences
			bne $8, $3, Compare2
			lw $4, RowHat2($0)
			bne $9, $4, MatchRowEnd
			j MatchMid 		# compare hat funct
Compare2:	lw $3, RowShirt1($0)
			bne $8, $3, Compare3
			lw $4, RowShirt2($0)
			bne $9, $4, MatchRowEnd
			j MatchMid 		# compare shirt funct
Compare3:	lw $3, RowEyes1($0)
			bne $8, $3, MatchRowEnd
			lw $4, RowEyes2($0)
			beq $9, $4, MatchEyes # compare eyes funct	
MatchRowEnd: jr $31
			
			
MatchEyes:	addi $6, $6, 4 		# traverse to middle of array
			addi $3, $0, 0		# init loop counter
			j MatchRowEnd

MatchMid:	addi $3, $0, 0		# init loop counter
			
			j MatchRowEnd