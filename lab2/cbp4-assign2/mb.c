#include <stdio.h>

int main(int argc, char *argv[]) {

    int i, n;

    if (argc != 2) {
        printf("specify max loop");
        return 0;
    }
    n = atoi(argv[1]);

    int a = 0, b = 0;

    /* Assembly of the for-loop, compiled with no flag.
#NO_APP
	movl	$0, -4(%rbp)
	jmp	.L4             // Start for-loop
.L7:
	movl	-4(%rbp), %ecx
	movl	$1431655766, %edx
	movl	%ecx, %eax
	imull	%edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	testl	%edx, %edx
	jne	.L5             // Branch: if(i%3==0)
	addl	$1, -8(%rbp)
.L5:
	movl	-4(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	%edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	testl	%edx, %edx
	jne	.L6             // Branch: if (i%5==0)
	addl	$1, -12(%rbp)
.L6:
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-16(%rbp), %eax
	jl	.L7             // Branch: for-loop
#APP
# 77 "mb.c" 1
	nop
# 0 "" 2
#NO_APP

     */

    asm("nop");
    for (i = 0; i < n; i++) {
        if (i%3 == 0) {
            a ++;
        }

        if (i%5 == 0) {
            b ++;
        }
    }
    asm("nop");

    printf("%d%d%d%d", i, a, b, n);

    return 0;
}
