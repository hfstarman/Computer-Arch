#include "second.h"

/*All of these functions are explained after main*/

void initialize(tNode* node);

void readDict(FILE *dict_file, tNode* root);
tNode* insertWord(tNode* root, char* word);

void readData(FILE *data_file, tNode* root);
void matchStr(char* str, tNode* root);

void printResult(FILE *outFile, tNode* root, char* word, int height, int prefix);
void postFree(tNode* root);

/*======================-MAIN-======================*/

int main(int argc, char** argv){

	//checks to see if there is only one argument
	if(argc != 2){
		printf("invalid input");
		return 0;
	}
	
	FILE *mapFile = fopen(argv[1], "r");
	
	//checks if the file exists
	if(mapFile == NULL){
		printf("invalid input");
		return 0;
	}
	
	//takes name of
	char dict[15], data[15];
	int fileNumber = 1;

	//obtains the dict and data files
	while(fscanf(mapFile, "%s %s", dict, data) != EOF){
	
		tNode* root = (tNode*)malloc(sizeof(tNode));
		initialize(root);	
		
		//creates the files to be used
		FILE *dictFile;
		dictFile = fopen(dict, "r");
		FILE *dataFile;
		dataFile = fopen(data, "r");

		/*Creates the Trie from dictionary file*/		
		readDict(dictFile, root);
		memset(dict, 0, sizeof(dict)); 
	
		/*Searches Trie Dictionary and updates words*/
		readData(dataFile, root);
		memset(data, 0, sizeof(data));	

		/*Prepares file to be outputted*/
		FILE *outFile = NULL;
		char fileName[15] = {0};
		sprintf(fileName, "out%d.txt", fileNumber);
		outFile = fopen(fileName, "w");
		
		/*Stores result in outFile*/
		char word[200] = {0};
		printResult(outFile, root, word, 0, 0);
		fclose(outFile);		

		postFree(root);
	
		++fileNumber;
	}

	//for whatever reason its not at eof (formatting error)
	if(!feof(mapFile)){
	  printf("invalid input");
	  return 0;
	}
	
	fclose(mapFile);
  return 0;
}


/*==================HELPER FUNCTIONS==================*/

//--------------------------------------------------------- INITIALIZE

/*Everything in the node is set to 0 or NULL*/
void initialize(tNode* node){

	node->letter = 0;
	int i;
	for(i = 0; i < 26; i++)
		node->children[i] = NULL;
	node->occurrences = 0;
	node->superwords = 0;
	node->isWord = 0;

}

//--------------------------------------------------------- INSERT WORD

/*Takes the root of the Trie and word to be inserted into it.
The function then attempts to go down the Trie and creates 
nodes along the way that do not exist until the end of the 
word has been reached. The final step of the function is to 
set the last node visited/created to the end of a word.*/

tNode* insertWord(tNode* root, char* word){

  int length = strlen(word);
  tNode *temp = root;
  tNode *prev;

  int i;
  for(i = 0; i < length; i++){
    int position = word[i] - 'a';

    prev = temp;
    temp = temp->children[position];

    if(temp == NULL){
      temp = (tNode*)malloc(sizeof(tNode));
      initialize(temp);
      temp->letter = word[i];
      prev->children[position] = temp;
    }

  }
  temp->isWord = 1;
  return root;
}

//--------------------------------------------------------- MATCH STR

/*Function takes the root of the Trie and the word that is
being searched for. trieSearch attempts to find the word
within the trie and will increments superwords if it passes
through them and updates the the occurrence if the function
has reached the end of the word if it exists in the dictionary.*/

void matchStr(char* str, tNode* root){

	int length = strlen(str);
	tNode* temp = root;

	int i;
	for(i = 0; i < length; i++){

		int position = str[i] - 'a';
		temp = temp->children[position];
		
		//no such word in dictionary
		if(temp == NULL)
			return;

		//checks current location is at end of word
		//if(temp->isWord == 1){

			if(isalpha(str[i+1]))
				temp->superwords++;
			else
				temp->occurrences++;
				
		//}

	}
	return;
}

//--------------------------------------------------------- PRINT RESULT

/*This function takes the root of the trie, and empty char
array and the height of the start. This is a recursive
function that will print out the trie in alphabetical order.
This function will be changed at the end to output to a file
instead of printing to the command line.*/

void printResult(FILE *outFile, tNode* root, char* word, int height, int prefix){

	if(root == NULL)
		return;

	if(root->isWord == 1){
		word[height] = 0; //unnecessary but safe
		fprintf(outFile, "%s %d %d\n", word, root->occurrences, prefix);
	}

	prefix += root->occurrences;

	int i;
	for(i = 0; i < 26; i++){

		if(root->children[i] != NULL){
			word[height] = ('a'+i);
			printResult(outFile, root->children[i], word, height+1, prefix);
		}

	}
	return;
}

//--------------------------------------------------------- POSTODER FREE

/*Simple recursive function that takes the root of the trie 
and will free all of the nodes within it at the end of the 
first.c to prevent a memory leak.*/

void postFree(tNode* root){

	if(root == NULL)
		return;

	//itterates across all chidren to free them
	int i;
	for(i = 0; i < 26; i++)
		postFree(root->children[i]);
	
	free(root);
	return;
}

//--------------------------------------------------------- READ DICT

/*The function takes a file pointer to the dictionary file 
and reads the unique words from the dictionary file and 
stores them in an appropriate data structure. Calls
insertWord to put words into the Trie*/

void readDict(FILE *dict_file, tNode* root){
	
	char word[200] = {0}, letter;
	int position = 0;
	while(fscanf(dict_file, "%c", &letter) != EOF){

		//nothing in word and letter is not alpha, so just skip it
		if((!isalpha(letter))&&(strlen(word)==0))
			continue;	

		if(isalpha(letter)){
			letter = tolower(letter);
			word[position] = letter;
			position += 1;
		}else{
			root = insertWord(root, word);
			memset(word, 0, sizeof(word)); //resets word
			position = 0;
		}
			
	}
	fclose(dict_file);
	return;
}

//--------------------------------------------------------- READ DATA

/*Goes through all the same steps as readDict except instead
of storing the word into the Trie it attempts to find it.
(all lines exactly the same except first line after the else)*/

void readData(FILE *data_file, tNode* root){

	char word[200] = {0}, letter;
	int position = 0;
	while(fscanf(data_file, "%c", &letter) != EOF){

		if((!isalpha(letter))&&(strlen(word)==0))
      continue;

		if(isalpha(letter)){
      letter = tolower(letter);
      word[position] = letter;
      position += 1;
    }else{
      matchStr(word, root);
      memset(word, 0, sizeof(word)); //resets word
      position = 0;
    }
		
	}
	fclose(data_file);
	return;
}


//--------------------------------------------------------- END

