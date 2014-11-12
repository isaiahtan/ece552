#include <stdio.h>

int main(int argc, char *argv[]) {

	// NLP is 64*4 blks in cache, 64B per blk

	int array_size = 64 * 64; // test 64 blks
	char array [array_size];
	int i, j;

	int upper_bound = atoi(argv[1]);

	for (j = 0; j < upper_bound; j++){
		for (i = 0; i < array_size; i+=64){ // skip over blk
			array[i] ++;
		}
	}

	// prefetcher should have only 8 (cold, one for each blk) misses for data L1 cache.

	return 0;
}
