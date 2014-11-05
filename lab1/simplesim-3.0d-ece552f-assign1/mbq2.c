#include <stdio.h>

int main(int argc, char *argv[]) {

	int i = 0;
	int input;
	int sum = 0;

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}


	input = atoi(argv[1]);
	asm("nop");

	while (i < 100000) {
		sum += input + 5;
		i += 1;
	}

	asm("nop");

	printf("input: %d", input);
	printf("sum: %d", sum);

	return 0;
}
