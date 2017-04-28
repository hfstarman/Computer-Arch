#ifndef second_h
#define second_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//creates the data structure to be used (trie)
typedef struct _tNode tNode;
struct _tNode{

	char letter;
	tNode* children[26];
	int occurrences;
	int superwords;	
	
	/*isWord checks if we are at the end of a word*/
	int isWord; //1 is true 0 is false
	
};



	
#endif
