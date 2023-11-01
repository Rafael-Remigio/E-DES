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


// ------------------- Helper functions to hexlify the char output ------------

int a2v(char c) {
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
    if ((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
    else return 0;
}

char v2a(int c) {
    const char hex[] = "0123456789abcdef";
    return hex[c];
}

char *hexlify(char *bstr) {
    char *hstr=malloc((strlen(bstr)*2)+1);
    bzero(hstr,(strlen(bstr)*2)+1);
    char *phstr=hstr;
    for(int i=0; i<strlen(bstr);i++) {
        *phstr++ =v2a((bstr[i]>>4)&0x0F);
        *phstr++ =v2a((bstr[i])&0x0F);
    }
    *phstr++ ='\0';
    return hstr;
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
		char *hash_hex = hexlify(hash);
		printf("Hash: %.64s\n",hash_hex);

		// Turn the char values from the hex_hash into integers. Sum them 4 by 4 to get larger values
		for (int c = 0; c < 64; c +=  4) {

			// Using absulutes to not have to deal with negative values
			int value = abs((int) hash_hex[c]) + abs((int) hash_hex[c+1]) + abs((int) hash_hex[c+2]) +abs((int) hash_hex[c+3]) ;
			printf("Ord: %d, ", value);


			// Add this value to the vector
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
	printf("\n");
}