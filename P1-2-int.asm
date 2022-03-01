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
# Student Name:
# Date:
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
	# TEMP: remove the lines between **********
                addi    $2, $0, 160             # TEMP: mark the 160th pixel
		swi	552			# TEMP: with this swi
                addi    $2, $0, 161             # TEMP: mark the 161th pixel
		swi	552			# TEMP: with this swi
# ***************************************************************************
	
	# your code goes here
	# pack hat/shirt locations into $2 and report your answer
# ***************************************************************************
	# TEMP: replace these lines to pack locations properly and report answer

		lui     $2, 300                 # TEMP: guess the 300th and
	        ori     $2, $2, 1004            # TEMP: and 1004th pixels
# ***************************************************************************
	
		swi	571			# submit answer and check
		# oracle returns correct answer in $3

		jr	$31			# return to caller
