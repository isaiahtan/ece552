#include <stdio.h>

int main(void)
{
	int i = 0;
	int sum = 0, temp = 0;

	// no-op that helps us read assembly code faster.
	asm("nop");

	/*
	The assembly output (with -O2 flag) of the following while loop looks like this:

	#NO_APP
	  li	$4,0x00010000		# 65536
	  ori	$4,$4,0x869f
	$L16:
	  addu	$5,$5,5		# temp = temp + 5
	  addu	$3,$3,1   	# i = i + 1 being run here
	  addu	$16,$5,5  	# sum = temp + 5 (RAW hazard with 1 stall)
	  slt	$2,$4,$3	# RAW hazard with 1 stall (unintentional)
	  beq	$2,$0,$L16 	# RAW hazard with 2 stalls (unintentional)
	#APP
	  nop
	#NO_APP
	*/

	while(i < 100000)
	{
		// Create a RAW hazard.
		temp = temp + 5;
		sum = temp + 5;

		i = i + 1;
	}


	// no-op that helps us read assembly code faster.
	asm("nop");

	printf("temp is %d\n", temp);
	printf("sum is %d\n", sum);
	return 0;
}

