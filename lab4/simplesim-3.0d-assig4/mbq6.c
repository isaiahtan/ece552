#include <stdio.h>

int main(int argc, char *argv[]) {

	// stride is 64*4 blks in cache, 64B per blk
	// 128 RPT entries, 
	
	// history with max of 2^8 entries
	
	// compiled with no optimizations '-O0'
	
	int array_size = 10000000;
	char array [array_size];
	int i, j;

	
	// rerun entire pattern 1000 times
	for (j = 0; j < 1000; j++){ // multiple loops to resimulate sudo random. will not be captured by stride, but by history.
		int index = 73; // random prime number
		
		for (i = 0; i < 100; i++){ // use 100 accesses to create access pattern
			array[index] ++; // mem access
			index = (index*37 + i*11)%array_size; // next index is sudo random
		}
	}
	return 0;
}
