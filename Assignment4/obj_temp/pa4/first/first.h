#ifndef first_h
#define first_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ADDRESSLENGTH 48

/*TYPEDEF STRUCT PROTOTYPES*/
typedef struct _cache cache;
typedef struct _set set;
typedef struct _block block;

/*FUNCTION PROTOTYPES*/
int powerOfTwo(int x);
int assocFormat(char* assoc);
char* hextoBinary(char* hexString);
int bintoDec(char* binaryString);

cache* initializeCache(int cacheSize, int assocType, int blockSize);
void readWriteCacheA(cache* cacheA, char* binaryAddress, int assocType, char command);
void readWriteCacheB(cache* cacheB, char* binaryAddress, int assocType, char command);
void freeCache(cache* usedCache, int cacheSize, int blockSize, int assocType);

/*STRUCTS*/

struct _cache{

	set* setArray; //size set depending on max index
	
	int reads;
	int writes; //These are to be incremented when needed
	int hits;
	int misses;

	int blockBits;
	int indexBits; //to easily keep track of how many bits these are
	int tagBits;

};

struct _set{

	block* blockArray; //set size depending on associativity
	int replace; //to keep track of which block to replace

};

struct _block{

	char valid;
	char info[ADDRESSLENGTH+1];

};


//==============================HELPER FUNCTIONS==============================//


//----------------------------------------------------------------------------LOG BASE 2

//Checks if the number is a power of 2.
//Parameters: int x (the number in question)
//Return value: will return the logx if it is power of 2 
//and 0 otherwise
int logBase2(int x){

	int count = 0;
    while(((x%2)==0) && x > 1){
    	x /= 2;
		count++;
	}

    if(x == 1)
        return count;

    return -1;
}


//----------------------------------------------------------------------------ASSOC FORMAT


//Checks if associativity is in the right format
//Paramenters: char* (the argument to check)
//Return value: returns 0 if its not right or the size of the assoc if it is  
int assocFormat(char* assoc){

    char* format = "assoc:";
    int i;

    for(i = 0; i < strlen(format); i++){
        if(!(format[i]==assoc[i]))
            return 0;
    }

    int length = strlen(assoc);
    length -= strlen(format);
    char* size = (char*)malloc(sizeof(char)*length+1);

    strcpy(size, &assoc[6]);
    int intSize = atoi(size);

    return intSize;
}


//----------------------------------------------------------------------------HEX TO BINARY


/*
This function converts a hexadecimal string into a binary string.
Parameters: char* hexString (the string with the hex value with a 0x prefix)
Returns: char* BinaryString (the translated string)
At the end of the function. The appropriate number of zeros will
be appended to the front to turn the address into 48 bits.
*/
char* hextoBinary(char* hexString){

	char* binaryString = (char*)malloc(sizeof(char)*((strlen(hexString)-2)*4+1)); //removes 0x from size and multiples by 4
	int hexLength = strlen(hexString);

	int i, location = 0;
	for(i = 2; i < hexLength; i++){
	
		switch(hexString[i]){
			case '0': strcpy(&binaryString[location], "0000"); location+=4; break;
			case '1': strcpy(&binaryString[location], "0001"); location+=4; break;
			case '2': strcpy(&binaryString[location], "0010"); location+=4; break;
			case '3': strcpy(&binaryString[location], "0011"); location+=4; break;
			case '4': strcpy(&binaryString[location], "0100"); location+=4; break;
			case '5': strcpy(&binaryString[location], "0101"); location+=4; break;
			case '6': strcpy(&binaryString[location], "0110"); location+=4; break;
			case '7': strcpy(&binaryString[location], "0111"); location+=4; break;
			case '8': strcpy(&binaryString[location], "1000"); location+=4; break;
			case '9': strcpy(&binaryString[location], "1001"); location+=4; break;
			case 'a': strcpy(&binaryString[location], "1010"); location+=4; break;
			case 'b': strcpy(&binaryString[location], "1011"); location+=4; break;
			case 'c': strcpy(&binaryString[location], "1100"); location+=4; break;
			case 'd': strcpy(&binaryString[location], "1101"); location+=4; break;
			case 'e': strcpy(&binaryString[location], "1110"); location+=4; break;
			case 'f': strcpy(&binaryString[location], "1111"); location+=4; break;
			default: printf("ERROR! non hexadecimal value inputted!\n"); return NULL;
		}

	}

	//this section will append the approriate number
	// of zeros to the binary string to force 48 bits	
	char* fullBinaryString = (char*)malloc(sizeof(char)*(ADDRESSLENGTH+1));
	int zeroLength = ADDRESSLENGTH - strlen(binaryString);
	memset(fullBinaryString, 0, strlen(fullBinaryString));
	
	int k;
	for(k = 0; k < zeroLength; k++)
		strcat(fullBinaryString, "0");

	strcat(fullBinaryString, binaryString);
	free(binaryString);

	return fullBinaryString;
}


//----------------------------------------------------------------------------BINARY TO DECIMAL


/*
Funtion to make it easier to find the correct set in one line
Parameters: char* binaryString (string to be converted decimal int)
Returns: int decimal
This function converts a binary string into a decimal integer.
*/
int bintoDec(char* binaryString){

	char* p;
	unsigned long decimal = strtol(binaryString, &p, 2);
	return (int)decimal;

}


//----------------------------------------------------------------------------INITIALIZE CACHE


/*
Parameters: int cacheSize, assocType, blockType (these are for
getting the sizes of array of sets and block arrays)
Returns: the created cache
Note: the function uses the variable cacheA but can be used for 
initializing both of the caches.
*/
cache* initializeCache(int cacheSize, int assocType, int blockSize){

	if(assocType == -2)
		assocType = 1;

	int indexLength = cacheSize/blockSize;

	cache* cacheA = (cache*)malloc(sizeof(cache));
	cacheA->reads = 0;
	cacheA->writes = 0;
	cacheA->hits = 0;
	cacheA->misses = 0;

	cacheA->blockBits = logBase2(blockSize);
	
	if(assocType != -1){ //direct and assoc:n

		cacheA->indexBits = logBase2(indexLength/assocType);
		cacheA->tagBits = ADDRESSLENGTH - (cacheA->blockBits + cacheA->indexBits);
		cacheA->setArray = (set*)malloc(sizeof(set)*(indexLength/assocType));

		int i, m;
		for(i = 0; i < (indexLength/assocType); i++){
			cacheA->setArray[i].blockArray = (block*)malloc(sizeof(block)*assocType);
			cacheA->setArray[i].replace = 0;
			for(m = 0; m < assocType; m++)
				cacheA->setArray[i].blockArray[m].valid = 0;
		}

	}else{
		cacheA->indexBits = 0;
		cacheA->tagBits = ADDRESSLENGTH - (cacheA->blockBits + cacheA->indexBits);
		cacheA->setArray = (set*)malloc(sizeof(set));
		cacheA->setArray[0].replace = 0;
		cacheA->setArray[0].blockArray = (block*)malloc(sizeof(block)*indexLength);

		int k;
		for(k = 0; k < indexLength; k++)
			cacheA->setArray->blockArray[k].valid = 0;

	}

	return cacheA;

}


//----------------------------------------------------------------------------READ WRITE CACHE A


/*
Function that finds correct block updates accordingly to read or write
Parameters: cacheA (the cache to check in); binaryAddress (the address to check);
assocType (to make things easier); command (the character that determines if its a read or write)
Returns: void
This will determine whether the address is a hit or a miss
and will increment accordingly to if its a read or a write.
*/
void readWriteCacheA(cache* cacheA, char* binaryAddress, int assocType, char command){
	
	//cacheA
	//getting index
	char* index = (char*)malloc(sizeof(char)*(cacheA->indexBits+1));
	int i, x = 0;
	for(i = cacheA->tagBits; i < (cacheA->tagBits + cacheA->indexBits); i++){

		index[x] = binaryAddress[i];
		x++;

	}

	int setN = bintoDec(index);
	free(index);
	int j = 0;
	do{
	int match = 1;
		if(cacheA->setArray[setN].blockArray[j].valid == 1){
			int k;
			for(k = 0; k < cacheA->tagBits; k++){

				if(binaryAddress[k] != cacheA->setArray[setN].blockArray[j].info[k]){
					match = 0;
					break;
				}

			}

			//if true then its a hit
			if(match == 1){
				cacheA->hits++;

				//does additional steps on write
				if(command == 'W'){
			
                cacheA->writes++;
                memset(cacheA->setArray[setN].blockArray[j].info, 0, (ADDRESSLENGTH+1));
                strcpy(cacheA->setArray[setN].blockArray[j].info, binaryAddress);

				}

				return;

			}
								
		}

		j++;

	}while(j < assocType);
	
	//if it gets to here its a miss
	int toReplace = cacheA->setArray[setN].replace;
	cacheA->misses++;
	cacheA->reads++;	

	//checks to see if its a write
	if(command == 'W')
		cacheA->writes++;

	cacheA->setArray[setN].blockArray[toReplace].valid = 1;
	memset(cacheA->setArray[setN].blockArray[toReplace].info, 0, (ADDRESSLENGTH+1));
	strcpy(cacheA->setArray[setN].blockArray[toReplace].info, binaryAddress);

	//updates the replacement location of the set
	cacheA->setArray[setN].replace++;
	if(cacheA->setArray[setN].replace >= assocType)
		cacheA->setArray[setN].replace = 0;

	return;
}


//----------------------------------------------------------------------------READ WRITE CACHE B


/*
Function that finds correct block updates accordingly to read or write
Parameters: cacheB (the cache to check in); binaryAddress (the address to check);
assocType (to make things easier); command (the character that determines if its a read or write)
Returns: void
This will determine whether the address is a hit or a miss
and will increment accordingly to if its a read or a write.
*/
void readWriteCacheB(cache* cacheB, char* binaryAddress, int assocType, char command){

	//cacheB
    //getting index
    char* index = (char*)malloc(sizeof(char)*(cacheB->indexBits+1));
    int i;
    for(i = 0; i < (cacheB->indexBits); i++)
        index[i] = binaryAddress[i];

    int setN = bintoDec(index);
    free(index);

    int j = 0;
    do{

        if(cacheB->setArray[setN].blockArray[j].valid == 1){
            //checks tag
            int k, match = 1;
            for(k = 0; k < (cacheB->tagBits); k++){

                if(binaryAddress[(k + cacheB->indexBits)] != cacheB->setArray[setN].blockArray[j].info[(k + cacheB->indexBits)]){
                    match = 0;
                    break;
                }

            }
            //read-hit
            if(match == 1){
                cacheB->hits++;

				if(command == 'W'){

                cacheB->writes++;
                memset(cacheB->setArray[setN].blockArray[j].info, 0, (ADDRESSLENGTH+1));
                strcpy(cacheB->setArray[setN].blockArray[j].info, binaryAddress);

				}

                return;

            }

		}

        j++;

    }while(j < assocType);

    //if it gets to here its a read-miss
    int toReplace = cacheB->setArray[setN].replace;
    cacheB->misses++;
    cacheB->reads++;

	if(command == 'W')
		cacheB->writes++;

    cacheB->setArray[setN].blockArray[toReplace].valid = 1;
    memset(cacheB->setArray[setN].blockArray[toReplace].info, 0, (ADDRESSLENGTH+1));
    strcpy(cacheB->setArray[setN].blockArray[toReplace].info, binaryAddress);

	//updates the replacement location of the set
    cacheB->setArray[setN].replace++;
    if(cacheB->setArray[setN].replace >= assocType)
        cacheB->setArray[setN].replace = 0;

	return;
}


//----------------------------------------------------------------------------FREE CACHE


/*
This function will free a cache
Parameters: a cache (the one to be freed)
Returns: Nothing
The function will traverse the sets in the cache first and free their blocks.
At the end it will free the setArray and then finally the cache itself.
*/
void freeCache(cache* usedCache, int cacheSize, int blockSize, int assocType){

	int count = 0, cacheLength;

	if(assocType == -1)
		cacheLength = 1;
	else if(assocType ==-2)
		cacheLength = (cacheSize/blockSize);
	else
		cacheLength = (cacheSize/(blockSize*assocType));

	int i;
	for(i = 0; i < cacheLength; i++){

		free(usedCache->setArray[i].blockArray);
		count++;

	}

	free(usedCache->setArray);
	free(usedCache);
	return;

}

#endif
