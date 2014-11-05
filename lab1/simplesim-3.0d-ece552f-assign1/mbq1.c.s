	.file	1 "mbq1.c"

 # GNU C 2.7.2.3 [AL 1.1, MM 40, tma 0.1] SimpleScalar running sstrix compiled by GNU C

 # Cc1 defaults:
 # -mgas -mgpOPT

 # Cc1 arguments (-G value = 8, Cpu = default, ISA = 1):
 # -quiet -dumpbase -O2 -o

gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"temp is %d\n\000"
	.align	2
$LC1:
	.ascii	"sum is %d\n\000"
	.text
	.align	2
	.globl	main

	.extern	stdin, 4
	.extern	stdout, 4

	.text

	.loc	1 4
	.ent	main
main:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	__main
	move	$3,$0
	move	$5,$0
 #APP
	nop
 #NO_APP
	li	$4,0x00010000		# 65536
	ori	$4,$4,0x869f
$L16:
	addu	$5,$5,5
	addu	$3,$3,1
	addu	$16,$5,5
	slt	$2,$4,$3
	beq	$2,$0,$L16
 #APP
	nop
 #NO_APP
	la	$4,$LC0
	jal	printf
	la	$4,$LC1
	move	$5,$16
	jal	printf
	move	$2,$0
	lw	$31,20($sp)
	lw	$16,16($sp)
	addu	$sp,$sp,24
	j	$31
	.end	main
