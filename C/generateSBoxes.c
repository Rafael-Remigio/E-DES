#include <stdint.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

// Swap Function to help with the Shufling algorithm
void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}


// Fisher Yates Algorithm
// It allows us to shuffle with a deterministic vector
// It also has linear time complexity, and O(1) space complexity.
void fisherYates ( uint8_t *array , int* vector )
{
	int i , j ;
	int swapper = 0;
	for ( i = 0; i < 256 ; i ++) {
		// Get next value to be swapped, Fisher Yates algorithm
		swapper = vector[i] % (256 -i);
		// Swap last value with value on another position
		swap(&array[swapper], &array[256-i]);
	}
}

// Generate vector from Key
// This vector will be used to shuffle the S-Boxes in a deterministic way
int *generateShuffleVector(char *key)
{
	// Key length is 32 bytes
	int key_length = 32;
	// Declare key to be incremented
	char key_with_index[34];
	// Hash value on each iteration
	char* hash;

	// Allocate memory
	hash = malloc(SHA256_DIGEST_LENGTH * sizeof(char));


	for (int i = 0; i < 4 ; i ++){


		sprintf(key_with_index, "%.32s%d", key, i);
		printf("%.33s\n", key_with_index);
		SHA256(key_with_index, 33, hash);

		printf("Hash: %.32s\n",hash);
	}
}




int main(int argc, char const *argv[])
{

	char key[32] = "aaaaaaaaabbbbbbbbbbcccccdddddddd";

	printf("%.32s\n", key);


	generateShuffleVector(key);
}