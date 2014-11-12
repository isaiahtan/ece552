#include <stdio.h>

int main(int argc, char *argv[]) {

	// stride is 64*4 blks in cache, 64B per blk
	// 16 RPT entries

	int array_size = 100000;
	char array [array_size];
	int i, j;

	int upper_bound = atoi(argv[1]);


	for (j = 0; j < upper_bound; j++){
		for (i = 0; i < array_size/2; i+=64){ // skip over blk
			// test 2 different strides (2 RPT entries)
			array[i] ++;
			array[i + array_size/2] ++;
		}
	}



	return 0;
}
