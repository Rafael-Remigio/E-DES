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
void generateShuffleVector(char *key,int *vector)
{
	// Key length is 32 bytes
	int key_length = 32;

	// Declare key to be concatenated with index
	// key_length + 2 (intchar) + 1 (null value to end string) = 34
	char key_with_index[key_length + 3];
	
	// Hash value on each iteration
	char* hash;

	// Allocate memory
	hash = malloc(SHA256_DIGEST_LENGTH * sizeof(char));



	int counter  = 0;
	
	for (int i = 0; i < 16 ; i ++){

		// Concatente char array with index
		sprintf(key_with_index, "%.32s%d", key, i);
		printf("String: %.34s\n", key_with_index);

		// Generate digest from Key and index
		// HASH( "kEY+IDX" )
		SHA256(key_with_index, 34, hash);

		printf("Hash: %.32s\n",hash);

		for (int c = 0; c < 32; c +=  2) {
			// Using absulutes to not have to deal with negative values
			int value = abs((int) hash[c]) + abs((int) hash[c+1]);
			printf("Ord: %d, ", value);

			vector[counter] = value;
			counter += 1;
		}
		printf("\n");
		printf("\n");

	}

	return;
}




int main(int argc, char const *argv[])
{

	char key[32] = "aaaaaaaaabbbbbbbbbbcccccdddddeef";

	printf("Initial key: %.32s\n\n", key);

	// Declare vector array
	int vector[256];

	generateShuffleVector(key, vector);

	for (int i = 0; i < 256 ; i ++){
		printf("%d, ",vector[i]);
	}
}