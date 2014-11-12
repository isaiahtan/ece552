#include <stdio.h>

int main(int argc, char *argv[]) {

	// NLP is 64 blks in cache, 64 items per blk

	int array_size = 1000000;
	char array [array_size];
	int i, j;

	int next_access = atoi(argv[1]);

	// have extremely low miss rate (only due to other stack variables) if next_access is within this/next blk size
	// Eg next_access = 10, miss rate = 0.78%

	// much larger miss rate when skipping over prefetched blk (where next_access >= blk * 2)
	// Eg next_access = 128, miss rate = 9.87%

	// note results are skewed due to other stack variables, set up, etc.

	for (i = 0; i < array_size; i+=next_access){ // skip over blk
		array[i] ++;
	}
	return 0;
}
