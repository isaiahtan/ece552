#include <stdio.h>

int main(int argc, char *argv[]) {

	// stride is 64*4 blks in cache, 64B per blk
	// 16 RPT entries
	int next_access = atoi(argv[1]);

	int array_size = 100000 * next_access;
	char array [array_size];
	int i;

	// results should get near perfect miss rate.
	// no matter the stride size
	for (i = 0; i < array_size; i+=next_access){ // skip over blk
		array[i] ++; // stride size of next_access
	}

	return 0;
}
