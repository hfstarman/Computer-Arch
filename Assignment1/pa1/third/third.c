#include<stdio.h>
#include<stdlib.h>

typedef struct node node;
  struct node{
  int data;
  node* p;
};


//traverses the bucket to see check if the value is there (useful for both search and insert)
int present(node* first, int value){ 
	
	//empty so not present
	if(first == NULL)
		return 0;
	
	//traverses table and compares the data 	
	node* temp = first;
	while(temp != NULL){
		
		if(value == temp->data)
			return 1;

		temp = temp->p;
		
	}

	return 0; //data was not matched
	
}


int main(int argc, char** argv){

	node *hashTable[10000];
	char ch;
	int value;

	//too many or not enough arguments
	if(argc != 2)
		return 0;

	char* fileName = argv[1];
	FILE* file = fopen(fileName, "r");
	
	if(file == NULL){
		printf("error");
		return 0;
	}
	
	//zeros out the hashtable to prevent unwanted errors
	int i;
	for(i = 0; i < 10000; i++){
		hashTable[i] = NULL;
	}
	
	node* temp;
	while(fscanf(file,"%c	%d", &ch, &value) != EOF){
	
		//the location is determined by the mod function
		int location;
		if(value >= 0)
			location = value % 10000;
		else
			location = -(value % 10000);
		
		//if ch is 'i' the number will be inserted into hashTable
		if(ch == 'i'){
			
			//inserts into first slot of location if nothing is present
			if(hashTable[location] == NULL){
				
				//creation of node to be inserted
				node* insert = (node*)malloc(sizeof(node));
				insert->data = value;
				insert->p = NULL;
				
				//the actual insert
				hashTable[location] = insert;
				printf("inserted\n");
			
			// used for inserting if there is already a node present at location
			}else{
			
				//checks to see if the value aleady exists at location
				if(present(hashTable[location], value) == 1){
					printf("duplicate\n");
						
				//value is not present and is inserted at the end	of the bucket
				}else{
					
					temp = hashTable[location];
					
					//creates node to be inserted
					node* insert = (node*)malloc(sizeof(node));
					insert->data = value;
					insert->p = NULL;
					
					//goes to end of bucket
					while(temp->p != NULL){
						temp = temp->p;}
					
					//the actual insert	
					temp->p = insert;
					printf("inserted\n");						
						
				}
				
			} 
			temp = hashTable[location];
			
			
		
		}else if(ch == 's'){ //if ch is 's' the code will check if value is in hashTable
		
			//first checks if the location is empty
			if(hashTable[location] == NULL){
				printf("absent\n");
				
			//location is not null
			}else{
				
				//attempts to value in location
				if(present(hashTable[location], value) == 1){
					
					printf("present\n");
					
				}else{
				
					printf("absent\n");
					
				}
				
			}
	
		}else if(ch!='i' && ch!='s' && ch!='\n'){
			//no i nor s detected on the line
			//for whatever reason I was having problems with ch becoming '\n' after each loop
			//made a quick fix but will have to explore this later
			printf("error\n");
		}
	}
	
	fclose(file);
	
	return 0;	
}
