#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){

	//checks to see if there is at least one argument
	if(argc < 2){
		printf("error");
		return 0;
	}
	
	//loop that iterates through the arguments and prints
	//the character at the first entry
	int i;
	int words = argc;
	for(i = 1; i < words; i++){
		printf("%c", argv[i][0]);
	}
	
	//this exists to create a space between the output and the command line
	printf("\n");

	return 0;
}
