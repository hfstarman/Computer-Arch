#include "first.h"

int main(int argc, char** argv){

/*the first several lines are checking for correct input format and validity of trace file*/


	if(argc !=5){
		printf("ERROR! Input format required: ./first <cache size> <associativity> <block size> <trace file>\n");
		return 0;
	}

	int cacheSize = atoi(argv[1]);
	
	//check for power of 2
	if(logBase2(cacheSize)<0){
		printf("ERROR! Argument 1: <cache size> requires int that is power of 2!\n");
		return 0;
	}

	char* assoc = argv[2];
	int assocType;

	if(strlen(assoc) < 5)
		printf("ERROR! Argument 2: <associativity> improper format!\n");

	if(strcmp("direct", assoc)==0){
		assocType = -2;
	}else if(strcmp("assoc", assoc)==0){
		assocType = -1;
	}else if(assocFormat(assoc)>0){
		int size = assocFormat(assoc);
		if(logBase2(size)<0){
			printf("ERROR! Argument 2: <associativity> improper format! n needs to be power of 2!\n");
			return 0;
		}else
			assocType = size;
	}else{
		printf("ERROR! Argument 2: <associativity> improper format!\n");
		return 0;
	}

	int blockSize = atoi(argv[3]);

	if(logBase2(blockSize)<0){
		printf("ERROR! Argument 3: <block size> improper format! Must be int of power 2!\n");
		return 0;
	}

	char* traceFileName = argv[4];
	FILE* traceFile = fopen(traceFileName, "r");

	if(traceFile == NULL){
		printf("<tracefile> does not exist\n");
		return 0;
	}

    int indexLength = cacheSize/blockSize;

	if(indexLength < 1){
		printf("ERROR! <block size> is larger than <cache size>!\n");
		return 0;
	}
	
	//variables to get input
	char* garbage = (char*)malloc(sizeof(char)*16);
	char command;
	char* address = (char*)malloc(sizeof(char)*15);
	cache* cacheA = initializeCache(cacheSize, assocType, blockSize); //creating the caches
	cache* cacheB = initializeCache(cacheSize, assocType, blockSize);

	if(assocType == -1)
		assocType = (cacheSize/blockSize);

	//MAIN LOOP START
	while(fscanf(traceFile, "%s %c %s", garbage, &command, address)!=EOF){

		/*checks to make sure the command is either read 
		or write before passing into the functions*/
		if(strcmp(garbage,"#eof")!=0 && (command=='R' || command=='W')){

			readWriteCacheA(cacheA, hextoBinary(address), assocType, command);
			readWriteCacheB(cacheB, hextoBinary(address), assocType, command);

		}

	}

	//prints the requested information
	printf("cache A\n");
	printf("Memory reads: %d\n", cacheA->reads);
	printf("Memory writes: %d\n", cacheA->writes);
	printf("Cache hits: %d\n", cacheA->hits);
	printf("Cache misses: %d\n", cacheA->misses);
	printf("cache B\n");
    printf("Memory reads: %d\n", cacheB->reads);
    printf("Memory writes: %d\n", cacheB->writes);
    printf("Cache hits: %d\n", cacheB->hits);
    printf("Cache misses: %d\n", cacheB->misses);

	fclose(traceFile);
	freeCache(cacheA, cacheSize, blockSize, assocType);
	freeCache(cacheB, cacheSize, blockSize, assocType);
	return 0;

}
