#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv){

	//a check to see if there are enough arguments
	if(argc < 2){
		printf("error");
		return 0;
	}
	
	//loop that iterates through the arguments
	int i, length;
	char* word;
	for(i = 1; i < argc; i++){
		
		//records the word at argv[i] and prints the last letter
		word = argv[i];
		length = strlen(word);
		printf("%c", argv[i][length-1]);
	
	}

	//for separating the output and the command line
	printf("\n");

	return 0;
}
