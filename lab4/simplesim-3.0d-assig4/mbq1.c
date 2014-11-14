#include <stdio.h>

int main(int argc, char *argv[]) {

	// NLP is 64 sets in cache, 2-way, 64 items per blk

	// compiled with no optimizations '-O0'
	
	int next_access = atoi(argv[1]);
	
	int array_size = 100000 * next_access; // run loop 100000 times
	char array [array_size];
	int i, j;

	// have extremely low miss rate (only due to other stack variables) if next_access is within this/next blk size
	// Eg next_access = 10, miss rate = 0.0%

	// much larger miss rate when skipping over prefetched blk (where next_access >= blk * 2)
	// Eg next_access = 128, miss rate = 9.96%

	// note results are skewed due to other stack variables, set up, etc.

	for (i = 0; i < array_size; i+=next_access){ // skip over B in strides
		array[i] ++;
	}
	return 0;
}
