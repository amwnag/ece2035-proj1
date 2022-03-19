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
# 03/04 Can detect George if he is upright
# 03/07 Can detect George for all orientations, however DI is not good
# 03/17 Replaced the matching function (immediately break after no match)
# 03/18 Can once again detect George in all orientations
# 03/19 Optimized and reduced memory read errors by looping through cols 1-55

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
			addi $2, $0, 0 		# initialize $2 to 0, indicating george not found
			addi $4, $0, 0		# set up counter for outer loop iterating through rows
OuterLoop: 	addi $5, $0, 0		# set up counter for inner loop iterating through columns
InnerLoop:	add  $7, $4, $5		# calculate effective address of pixel, $4 + $5
			lbu	 $8, Array($7)	# access the color in the array
			slti $6, $8, 9 		# color value is face, call match function
			beq  $6, $0, NoMatch
			addi $29, $29, -12 	# store inner and outer loop values in stack
			sw $31, 8($29)		# preserve return addr
			jal MatchRow		# recall jal stores return addr in $31
			lw $31, 8($29)
			addi $29, $29, 12	# adjust SP up
			bne $2, $0, ReportLoc # if $2 is non-zero
NoMatch: 	addi $5, $5, 9		# update inner loop counter (incrementing by 9)
			slti $6, $5, 55		# check inner loop exit, indicies 55 and after cannot match
			bne  $6, $0, InnerLoop 
			addi $4, $4, 64		# update statement for outer loop, row is 64 pixels
			slti $6, $4, 4096		# check outer loop exit condition, total 64 rows, i less than 64
			bne  $6, $0, OuterLoop 
			
ReportLoc: 	swi	571			# submit answer and check
			# oracle returns correct answer in $3
			jr	$31			# return to caller

########
MatchRow: 	addi $7, $7, -1 # use effective address and decrement until bg pixel
			lbu	 $8, Array($7)	# access the color in the array
			slti $6, $8, 9	# if it is 9, 10, 11 --> stop decrementing, branch
			beq $6, $0, CheckFirst
			j MatchRow		# loop back to decrement more
			
CheckFirst:	addi $7, $7, 1 # increment by 1, checking the first pixel
			lbu $8, Array($7) 
			
			slti $6, $8, 6 # if greater than 5, return
			beq $6, $0, MatchRowEnd
			
			addi $6, $0, 0x5 # yellow
			beq $8, $6, MatchY # if match yellow, jump to yellow matching
			addi $6, $0, 0x3 # blue
			beq $8, $6, MatchB
			addi $6, $0, 0x1 # white
			beq $8, $6, MatchW
			j MatchRowEnd	# no match
			
MatchY:		addi $6, $7, 2 # eye, using $6 for temp value
			lbu $8, Array($6)
			addi $6, $0, 0x7 # store desired value in $6
			bne $8, $6, MatchRowEnd
			addi $6, $7, 3 # check not shades
			lbu $8, Array($6)
			addi $6, $0, 0x5 # store desired value in $6
			bne $8, $6, MatchRowEnd
			j MatchEyes
			
MatchB:		addi $6, $7, 3 		# smile
			lbu $8, Array($6)
			addi $6, $0, 0x8 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 4 		# smile2
			lbu $8, Array($6)
			addi $6, $0, 0x8 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 7  # face, maybe can implement in a loop somehow 
			lbu $8, Array($6)
			addi $6, $0, 0x5 # desired value
			bne $8, $6, MatchRowEnd
			addi $6, $7, 10 		# hat
			lbu $8, Array($6)
			addi $6, $0, 0x2 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 11 		# hat2
			lbu $8, Array($6)
			addi $6, $0, 0x1 
			bne $8, $6, MatchRowEnd
			j MatchShirt
			
MatchW:		addi $6, $7, 1 		# hat color
			lbu $8, Array($6)
			addi $6, $0, 0x2 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 4 		# face
			lbu $8, Array($6)
			addi $6, $0, 0x5 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 7 		# smile 1
			lbu $8, Array($6)
			addi $6, $0, 0x8 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 8 		# smile 2
			lbu $8, Array($6)
			addi $6, $0, 0x8 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 11 		# shirt
			lbu $8, Array($6)
			addi $6, $0, 0x3 
			bne $8, $6, MatchRowEnd
			j MatchHat

MatchRowEnd: jr $31
			
########
			
MatchEyes:	addi $7, $7, 4		# move to ind of center
			addi $6, $7, -64 	# calculate addr of pixel above

			lbu $8, Array($6)
			addi $6, $0, 0x2
			bne $8, $6, UpsideDown # see if it's red or not
			
			addi $7, $7, -256	# start at top of head

			lbu $8, Array($7)
			addi $6, $0, 0x1 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 64 		# 1 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x2
			bne $8, $6, MatchRowEnd
			addi $6, $7, 448 		# 7 * 64

			lbu $8, Array($6)
			addi $6, $0, 0x8
			bne $8, $6, MatchRowEnd
			addi $6, $7, 512 		# 8 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x8
			bne $8, $6, MatchRowEnd
			addi $6, $7, 704 		# 11 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x3
			bne $8, $6, MatchRowEnd
			add $2, $0, $7 	# match, top of head
			sll $2, $2, 16
			addi $6, $7, 704 # shirt ind
			or $2, $2, $6
			j MatchRowEnd
			
UpsideDown:	addi $7, $7, -448	# at shirt
			lbu $8, Array($7)
			addi $6, $0, 0x3 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 192 		# 3 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x8
			bne $8, $6, MatchRowEnd
			addi $6, $7, 256 		# 4 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x8 
			bne $8, $6, MatchRowEnd
			addi $6, $7, 640 		# 10 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x2
			bne $8, $6, MatchRowEnd
			addi $6, $7, 704 		# 11 * 64
			lbu $8, Array($6)
			addi $6, $0, 0x1
			bne $8, $6, MatchRowEnd
			addi $2, $7, 704 	# hat ind into register 2
			sll $2, $2, 16
			or $2, $2, $7
			j MatchRowEnd

########
MatchHat: 	addi $6, $7, 132 		# 4 + 64 * 2  
			lbu $8, Array($6)
			addi $6, $0, 0x7
			bne $8, $6, MatchRowEnd
			add $2, $0, $7 	# match!
			sll $2, $2, 16
			addi $6, $7, 11 # shirt ind
			or $2, $2, $6
			j MatchRowEnd
########

MatchShirt: addi $6, $7, 135 		# 7 + 64 * 2
			lbu $8, Array($6)
			addi $6, $0, 0x7
			bne $8, $6, MatchRowEnd
			addi $2, $7, 11 # hat ind into register 2
			sll $2, $2, 16
			or $2, $2, $7
			j MatchRowEnd
