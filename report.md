# E-DES

## Introduction

The primary objective of this project is to develop an upgraded iteration of the Data Encryption Standard (DES), named E-DES.
We seek to address two key shortcomings of DES: the algorithm's inherent software performance inefficiencies and the limited key size. To achieve these objectives, we will retain the fundamental cipher architecture of DES while departing from its traditional reliance on permutations, expansion and key mixing. Instead, we will utilize a 256-bit key to generate key-dependent S-Boxes and perform only substitution on each Feistel iteration. Another concern with DES, and it's use of static S-Boxes, was the existance of a possible backdoor. 

In order to test the performance of our approach, E-DES is implemented in two diferent programming languages, a high-level language and a low-level language. The programming languages selected were Python and C respectivly. Encryption and decryption is possible using both DES and E-DES in the tools provided in both languages. These tools take a textual password as an argument and use it to generate the 256 bit key for E-DES and the 56 bit key for DES. The only cipher mode implemented is ECB, and the block size is 64 bits.

## Cipher Architecture

### Feistel Networks

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

```
void feistelRounds(uint8_t data[8],bool isEncrypting, uint8_t SBOXES[16][256]){
    // Slices from 8 bytes of data, 4 bytes each
    uint8_t slice1[4], slice2[4]; 
    (...)
    if (isEncrypting){
        for( int i = 0 ; i < 16; i ++ ){
            feistel(slice1,slice2,SBOXES[i]);
        }
    }
    // If it is decryption do the rounds the other way
    else {
        for( int i = 16 - 1 ; i >= 0; i -- ){
            feistel(slice2,slice1,SBOXES[i]);
        }
    }
    (...)
}
```

### S-Boxes

The 16 S-Boxes used in the feistel network must be diferent and pseudo randomly generated. The 16 S-Boxes don't need all to contain the same set of values, but accross the 16 S-Boxes the 4096 values must be be equally distributed. What this means is that for example one S-Box may have 3 zeros and another 5 eights but in the totality the S-Boxes must have exactly 16 zeros and 16 eights.

The 4098 byte sized values are shuffled deterministically using a shuffling vector of the same size. The 256 bit key is used to generate this vector.

**Key generation**
Key generated from password using SHA256

**Shufling Vector from key**
To generate the 4098 **pseudo-random** values from the 256 bit key, we used SHA-256 hash function. The key is concatenated with integer values from 0 to 255 and hashed. From each 256 bit hash value we generate 16 integers and add them to the vector. The ideia was to create something similar to Hash_DRBG. Hash_DRBG requires reseeding in order to avoid cycles or the discovery of the seed, something we did not implement given the small ammount of hashes used and the fact that the values generated are not reflected directly on the ciphertext.

```
// Key length is 32 bytes
int seed_length = 32;

// Declare seed to be concatenated with index
// seed_length + 3 (integer char) + 1 (null value to end string) = 34
char seed_with_index[seed_length + 4];

// Hash value on each iteration
char* hash;

for (int i = 0; i < 256 ; i ++){

		// Concatente char array with index
		sprintf(seed_with_index, "%.32s%.3d", seed, i);

		// Generate digest from Key and index
		// HASH( "KEY+IDX" )
		SHA256(seed_with_index, 35, hash);

```

**Fisher Yates**
The deterministic shuffling algorithm used is the [Fisher–Yates shuffle](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle). Given a list and vector of random integers with the same size, we move throught the list backwards and swap betweent the current element of the list and the remainder of the corresponding element of the vector divided by the index of the element of the list.

```
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
	int i  ;
	int swapper = 0;
	int size =  256 * 16;
	for ( i = 0; i < size; i ++) {
		// Get next value to be swapped, Fisher Yates algorithm
		swapper = vector[i] % ( size -i);
		// Swap last value with value on another position
		swap(&array[swapper], &array[size-i-1]);
	}
}
```


### Padding


```
// Remove the padding
uint8_t padding = clearText[7]
for( int i = 0 ; i < 8 - padding; i ++ ){
    printf("%c",clearText[i]);
}

// Add padding to last block
int padding_value = 8 - input_index;
// PKCS#7 padding
for (int i = 7 ; i >= input_index ; i--){
    data[i] = (uint8_t) padding_value;
}
```

## Implementation

Implementing this is Pyhon3 proved to be challenging, and incredibly ineficient compared to the normal DES, on C however implementation was much easier. Another problem I ran to was language compatability with stdout and Pyhton3's ridiculous encoding.

## Conclusion

E-DES 