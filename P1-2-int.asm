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
# 02/12 Looping through pixels to find one w/ color $4              (example)
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
			addi $4, $0, 0		# set up counter for outer loop iterating through rows
OuterLoop: 	addi $5, $0, 0		# set up counter for inner loop iterating through columns
InnerLoop:	add  $7, $4, $5		# calculate effective address of pixel, $4 + $5
			lbu	 $8, Array($7)	# access the color in the array
			slti $6, $8, 9 		# color value is face, call match function
			beq  $6, $0, NoMatch
			addi $29, $29, -12 	# store inner and outer loop values in stack
			sw $31, 8($29)		# preserve return addr
			sw $4, 4($29)		# push outer loop counter
			sw $5, 0($29)		# push inner loop counter
			jal MatchRow		# recall jal stores return addr in $31
			lw $31, 8($29)
			lw $4, 4($29)
			lw $5, 0($29)
			addi $29, $29, 12	# adjust SP up
			bne $2, $0, ReportLoc # if $2 is non-zero
NoMatch: 	addi $5, $5, 9		# update inner loop counter (incrementing by 9)
			slti $6, $5, 64		# check inner loop exit
			bne  $6, $0, InnerLoop 
			addi $4, $4, 64		# update statement for outer loop, row is 64 pixels
			slti $6, $4, 4096		# check outer loop exit condition, total 64 rows, i less than 64
			bne  $6, $0, OuterLoop 
			
ReportLoc: 	swi	571			# submit answer and check
			# oracle returns correct answer in $3
			jr	$31			# return to caller

MatchRow: 	addi $7, $7, -1 # use effective address and decrement until bg pixel
			lbu	 $8, Array($7)	# access the color in the array
			slti $6, $8, 9	# if it is 9, 10, 11 --> stop decrementing, branch
			beq $6, $0, CheckFirst
			j MatchRow
CheckFirst:	addi $7, $7, 1 # increment by 1, checking the first pixel
			lbu $8, Array($7) 
			
			slti $6, $8, 6 # if greater than 5, return
			beq $6, $0, MatchRowEnd
			addi $11, $7, 0 		# save copy of $7
			
			addi $4, $0, 0	# initialize loop for scanning pixels
			addi $9, $0, 0	# initialize first part of seq
			addi $10, $0, 0	# initialize second part of seq
ScanFirst: 	lbu $8, Array($7) 		# get pixel at the current ind, may be redundant for 1st iteration

			slti $6, $8, 9 			# return if pixel is bg
			beq  $6, $0, MatchRowEnd 
			sll $9, $9, 4 		# shift 
			or $9, $9, $8 	# use $9 for running word representing first 8 pixels
			addi $7, $7, 1 		# increment the addr
			addi $4, $4, 4 		# update, scaled for shifting
			slti $6, $4, 32 		# exit condition, 4*8
			bne $6, $0, ScanFirst 	# loop back
			addi $4, $0, 0 			# reset loop counter
ScanSecond: lbu $8, Array($7) 		# get pixel at the current ind
			slti $6, $8, 9 			# exit loop if pixel is bg
			beq  $6, $0, Comparison # note: if no extra pixels, exit function
			sll	 $10, $10, 4
			or $10, $10, $8 			# remaining pixels
			addi $7, $7, 1
			addi $4, $4, 4 # update, scaled for shifting
			slti $6, $4, 16 # exit condition, 4*4
			bne $6, $0, ScanSecond # loop back
Comparison:	lw $4, RowHat1($0)		# match with 3 possible sequences
			bne $9, $4, Compare2
			lw $5, RowHat2($0)
			bne $10, $5, MatchRowEnd
			j MatchMid 		# compare hat funct
Compare2:	lw $4, RowShirt1($0)
			bne $9, $4, Compare3
			lw $5, RowShirt2($0)
			bne $10, $5, MatchRowEnd
			j MatchMid 		# compare shirt funct
Compare3:	lw $4, RowEyes1($0)
			bne $9, $4, MatchRowEnd
			lw $5, RowEyes2($0)
			beq $10, $5, MatchEyes # compare eyes funct
MatchRowEnd: 	jr $31
			

			
MatchEyes:	addi $4, $0, 64		# init values used in matching, counter
			addi $9, $0, 0		# colUp
			addi $10, $0, 0		# colDown
			addi $7, $11, 4 		# traverse to middle of row
ColScan1:	sll	$9, $9, 4
			sll $10, $10, 4
			sub $6, $7, $4			# traverse up arr
			lbu $8, Array($6) 		# get pixel at the current ind 
			# check if out of bounds
			or $9, $9, $8 	
			add $6, $7, $4			# traverse down arr
			lbu $8, Array($6)
			or $10, $10, $8
			addi $4, $4, 64		# update
			slti $6, $4, 512		# exit, 8 * 64
			bne $6, $0, ColScan1
			lw $4, RowShirt2($0) # now compare, 0x2121
			lw $5, Seq2($0)
			
			bne $10, $5, UpsideDown
			srl $9, $9, 12
			bne $9, $4, MatchRowEnd
			addi $2, $7, -256
			sll $2, $2, 16
			addi $6, $7, 448
			or $2, $2, $6

UpsideDown: bne $9, $5, MatchRowEnd
			srl $10, $10, 12
			bne $10, $4, MatchRowEnd
			addi $2, $7, 256
			sll $2, $2, 16
			addi $6, $7, -448
			or $2, $2, $6
			
			j MatchRowEnd
			

MatchMid:	addi $4, $0, 0		# init values used in matching
			addi $9, $0, 0
			addi $10, $0, 0
			addi $7, $11, -252		# set starting addr
ColScan2:	sll	$9, $9, 4
			sll $10, $10, 4
			add $6, $7, $4			# traverse down col
			lbu $8, Array($6) 		# get pixel at the current ind
			or $9, $9, $8 	
			addi $6, $6, 3
			lbu $8, Array($6)
			or $10, $10, $8
			addi $4, $4, 64		# update
			slti $6, $4, 320		# exit, 5 * 64
			bne $6, $0, ColScan2
			lw $6, RowEyes1($0)
			srl $6, $6, 12
			beq $6, $9, Good1	# eyes located at left ind
			beq $6, $10, Good2 		# eyes located right
			j MatchRowEnd
			
Good1:		addi $2, $11, 0
			sll $2, $2, 16
			addi $6, $11, 11
			or $2, $2, $6
			j MatchRowEnd
Good2:		addi $6, $11, 11
			addi $2, $6, 0
			sll $2, $2, 16
			or $2, $2, $11
			j MatchRowEnd