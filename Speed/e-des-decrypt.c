#include <stdint.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <openssl/des.h>
#include <time.h>


uint8_t BOX[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};


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
		swap(&array[swapper], &array[size-i]);
	}
}


// ------------------- Helper functions to hexlify the char output ------------
// ------------------- I took this from a stack overflow comment ------------


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

char *hexlify(char *bstr,int str_length) {

    char *hstr=malloc((str_length*2)+1);
    bzero(hstr,(str_length*2)+1);
    char *phstr=hstr;
    for(int i=0; i<str_length;i++) {
        *phstr++ =v2a((bstr[i]>>4)&0x0F);
        *phstr++ =v2a((bstr[i])&0x0F);
    }
    *phstr++ ='\0';
    return hstr;
}

char *unhexlify(char *hstr,int str_length) {
    char *bstr=malloc((str_length/2)+1);
    bzero(bstr,(str_length/2)+1);
    char *pbstr=bstr;
    for(int i=0;i<str_length; i += 2)
        *pbstr++ =(a2v(hstr[i])<<4)+a2v(hstr[i+1]);
    *pbstr++ ='\0';
    return bstr;
}
// --------------------------------------------------------------------



void preformSBoxTransformation(uint8_t *sbox,uint8_t in [4]){

    uint8_t out[4];

    uint8_t index = in [3];
    out [0] = sbox [ index ];
    index = ( index + in [2]) ;
    out [1] = sbox [ index ];
    index = ( index + in [1]) ;
    out [2] = sbox [ index ];
    index = ( index + in [0]) ;
    out [3] = sbox [ index ];

    memcpy(in, out, 4);

    return;

}


void feistel(uint8_t *firstSlice, uint8_t *secondSlice, uint8_t sbox[256]) {

    uint8_t newFirstSlice[4];

    // Switch position between slices
    // Ri to Li+1
    memcpy(newFirstSlice, secondSlice, 4);

    // Pemutate and XOR
    preformSBoxTransformation(sbox,secondSlice);

    for ( int i = 0 ; i < 4 ; i++){
        secondSlice[i] ^= firstSlice[i];
    }

    // copy firtslice to the correct position
    memcpy(firstSlice, newFirstSlice, 4);

    return;
   
}

void feistelRounds(uint8_t data[8],bool isEncrypting, uint8_t SBOXES[16][256]){

    // Slices from 8 bytes of data, 4 bytes each
    uint8_t slice1[4], slice2[4]; 

    // Slice the arrays
    memcpy(slice1, data, 4 * sizeof(uint8_t)); 
    memcpy(slice2, data + 4, 4 * sizeof(uint8_t)); 


    //for (int i = 0 ; i < 4 ; i++){
    //    printf("%x ",slice1[i]);
    //}
    //for (int i = 0 ; i < 4 ; i++){
    //    printf("%x ",slice2[i]);
    //}
    //printf("\n");


    // If it is encryption do the rounds one way
    if (isEncrypting){
        for( int i = 0 ; i < 16; i ++ ){
            feistel(slice1,slice2,SBOXES[i]);

            //for (int i = 0 ; i < 4 ; i++){
            //    printf("%x ",slice1[i]);
            //}
            //for (int i = 0 ; i < 4 ; i++){
            //    printf("%x ",slice2[i]);
            //}
            //printf("\n");
        }
    }
    // If it is decryption do the rounds the other way
    else {
        for( int i = 16 - 1 ; i >= 0; i -- ){
            feistel(slice2,slice1,SBOXES[i]);

            //for (int i = 0 ; i < 4 ; i++){
            //    printf("%x ",slice1[i]);
            //}
            //for (int i = 0 ; i < 4 ; i++){
            //    printf("%x ",slice2[i]);
            //}
            //printf("\n");
        }
    }

    for ( int i = 0 ; i < 4; i += 1 ){
        data[i] = slice1[i];
    }
    for ( int i = 0 ; i < 4; i += 1 ){
        data[4+i] = slice2[i];
    }

}

// Generate vector from Key
// This vector will be used to shuffle the S-Boxes in a deterministic way
void generateShuffleVector(char *seed,int *vector)
{
	// Key length is 32 bytes
	int seed_length = 32;

	// Declare seed to be concatenated with index
	// seed_length + 2 (intchar) + 1 (null value to end string) = 34
	char seed_with_index[seed_length + 4];
	
	// Hash value on each iteration
	char* hash;

	// Allocate memory
	hash = malloc(SHA256_DIGEST_LENGTH * sizeof(char));



	int counter  = 0;
	
	for (int i = 0; i < 256 ; i ++){

		// Concatente char array with index
		sprintf(seed_with_index, "%.32s%d", seed, i);
		//printf("String: %.35s\n", seed_with_index);

		// Generate digest from Key and index
		// HASH( "KEY+IDX" )
		SHA256(seed_with_index, 35, hash);

        // Hexlify the data, the size of the byte array is 32
		char *hash_hex = hexlify(hash,32);
		//printf("Hash: %.64s\n",hash_hex);

		// Turn the char values from the hex_hash into integers. Sum them 4 by 4 to get larger values
		for (int c = 0; c < 64; c +=  4) {

			// Using absulutes to not have to deal with negative values
			int value = abs((int) hash_hex[c]) + abs((int) hash_hex[c+1]) + abs((int) hash_hex[c+2]) +abs((int) hash_hex[c+3]) ;
			

			/// Debugging Prints
			//printf("%c: ", hash_hex[c]);
			//printf("%d  ",abs((int) hash_hex[c]));

			//printf("%c: ", hash_hex[c+1]);
			//printf("%d  ",abs((int) hash_hex[c+1]));

			//printf("%c: ", hash_hex[c+2]);
			//printf("%d  ",abs((int) hash_hex[c+2]));

			//printf("%c: ", hash_hex[c+3]);
			//printf("%d  ",abs((int) hash_hex[c+3]));
			//printf("Ord: %d, ", value);

			//printf("\n");

			// Add this value to the vector
			vector[counter] = value;
			counter += 1;
		}
		//printf("\n");
		//printf("\n");

	}

	return;
}


void updateFileWithNewTime(int newTime, const char *filePath) {
    FILE *filePointer;
    int previousTime;

    // Open the file in read mode to read the previous time value
    filePointer = fopen(filePath, "r");
    if (filePointer != NULL) {
        // Read the previous time from the file
        fscanf(filePointer, "%d", &previousTime);
        fclose(filePointer);

        // Compare with the new time value
        if (newTime < previousTime) {
            // Open the file in write mode to overwrite with new time
            filePointer = fopen(filePath, "w");
            if (filePointer != NULL) {
                // Write the new time value to the file
                fprintf(filePointer, "%d", newTime);
                fclose(filePointer);
                printf("File updated with new time: %d\n", newTime);
            } else {
                printf("Error opening the file for writing.\n");
            }
        } else {
            printf("New time is not less than the previous time. File remains unchanged.\n");
        }
    } else {
        // If the file does not exist, create it and write the new time
        filePointer = fopen(filePath, "w");
        if (filePointer != NULL) {
            fprintf(filePointer, "%d", newTime);
            fclose(filePointer);
            printf("File created with initial time: %d\n", newTime);
        } else {
            printf("Error opening the file for writing.\n");
        }
    }
}



int main(int argc, char *argv[])
{

    char *password = NULL;
    bool e_des_mode = true;

    // GETOPS
    int opt; 
    while((opt = getopt(argc, argv, "dp:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'p':  
                password = optarg;
                break;  
            case 'd':  
                e_des_mode = false;
                break;  
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    
    struct timespec start, end;
    double elapsed;

    if (e_des_mode){



        //Generate seed from key

        char seed[32];

        SHA256(password, strlen(password), seed);


	    int vector[256*16];

	    generateShuffleVector(seed, vector);

	    // Create base SBox array
	    uint8_t sboxes[256*16];
	    int counter = 0;
	    for (int i = 0; i < 16 ; i ++){
	    	for (int j = 0; j < 256; j++){
	    		sboxes[counter] = BOX[j];
	    		counter++;
	    	}
	    }

	    fisherYates(sboxes,vector);

	    uint8_t shuffledSboxes[16][256];

	    counter = 0;
	    for (int i = 0; i < 16 ; i ++){

	    	for (int j = 0; j < 256; j++){
                shuffledSboxes[i][j] = sboxes[counter];
	    		counter++;
	    	}
	    }




        uint8_t data[8];


        char ch;
        int input_index = 0;

        // This is stupid but works
        bool firstBlock = true;

         clock_gettime(CLOCK_MONOTONIC, &start);

        while(read(STDIN_FILENO, &ch, 1) > 0)
        {

            if (input_index == 0  && !firstBlock){
                printf("%.8s",data);
            }


            // add to block
            data[input_index] = ch;
            input_index++;
            // Again this is stupig way to deal with this
            // but don't know how stdin works in c
            firstBlock = false;


                if (input_index == 8) {
                    // preform decrytion
                    feistelRounds(data,false,shuffledSboxes);

                    // print block as stdout
                    input_index = 0;

                }




        }

        // Remove the padding
        uint8_t padding = data[7];

        
        // Get the current time again
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate the elapsed time in seconds with nanosecond precision
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);

        const char *filePath = "edes_decrypt.txt";
        updateFileWithNewTime(elapsed, filePath);



        for( int i = 0 ; i < 8 - padding; i ++ ){
            printf("%c",data[i]);
        }

    
    }
    else {

        DES_cblock key;
        DES_key_schedule key_schedule;

        // Initialize the key
        DES_string_to_key("your_key", &key);

        // Create the key schedule
        DES_set_key(&key, &key_schedule);

        char data[8];
        char clearText[8];

        char ch;
        int input_index = 0;

        // This is stupid but works
        bool firstBlock = true;


        while(read(STDIN_FILENO, &ch, 1) > 0)
        {

            if (input_index == 0  && !firstBlock){
                printf("%.8s",clearText);
            }


            // add to block
            data[input_index] = ch;
            input_index++;
            // Again this is stupig way to deal with this
            // but don't know how stdin works in c
            firstBlock = false;


                if (input_index == 8) {
                    // preform decrytion
                    DES_ecb_encrypt((const_DES_cblock *)data, (DES_cblock *)clearText, &key_schedule, DES_DECRYPT);                

                    // print block as stdout
                    input_index = 0;

                }




        }

        // Remove the padding
        uint8_t padding = clearText[7];

          clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate the elapsed time in seconds with nanosecond precision
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);



        const char *filePath = "des_decrypt.txt";
        updateFileWithNewTime(elapsed, filePath);


        for( int i = 0 ; i < 8 - padding; i ++ ){
            printf("%c",clearText[i]);
        }

    }

}