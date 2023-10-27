#include <stdint.h>
#include <stdio.h>



// Swap Function to help with the Shufling algorithm
void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}


// Fisher Yates Algorithm
// It allows us to shuffle with a deterministic vector
// It also has linear time complexity, and O(1) space complexity.
void fisherYates ( uint8_t ∗ array , int* vector )
{
	int i , j ;
	int swapper = 0;
	for ( i = 0; i < 256 ; i ++) {
		// Get next value to be swapped, Fisher Yates algorithm
		swapper = vector[i] % (256 -i);
		// Swap last value with value on another position
		swap(array[swapper] , array[256-i]);
	}
}

// Generate vector from Key
// This vector will be used to shuffle the S-Boxes in a deterministic way
void generateShuffleVector()
{

}

