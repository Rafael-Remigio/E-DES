# Personal notes and TODOs

## Analysing the given assignment

### The important parts of the assignment

**Introduction**

* Implement a cipher with a DES resemblance,
* it uses solely Feistel Networks and S-Boxes
* We must use key-dependent S-Boxes. 
* E-DES will use longer, 256-bit keys.

**Homework**

* 256-bit key
* Input and Output blocks of 64 bits
* Function used in each Feistel Network should be implemented exclusively
by an S-Box
* More information about S-Boxes bellow
* Unlike DES, there is no initial and final bit permutations
* Example of an fi function
    <img src="images/feistelNetwork.png">

**S-Boxes** 
* All 16 S-Boxes should be different 
* They should be generated from the key
* the generation process should not allow to discover an S-Box from the value of any of the 15 other
* S-Boxes cannot be equal
* it should not be easy to discover the key from any subset of the S-Boxes
* The 16 S-Boxes should be deterministically computed only once
* Do not use language-dependent shuffling features
* 16 S-Boxes must have exactly 16 equal byte values, but these do not need to be equally distributed by all S-Boxes. 
* One may have 3 zeros and another 5 eights but in the totality the S-Boxes must have exactly 16 zeros and 16 eights

**Feistel Network**
* The values in each S-Box are the output for a byte-long input.
* Blocs of 64 so it will be 32 on each slice Li and Ri



## TODO

Learn how to implement substitution boxes and that sort of permutation