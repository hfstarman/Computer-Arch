#include<stdio.h>
#include<stdlib.h>

typedef struct node node;
	struct node{
		int data;
    node* p;
};

int main(int argc, char** argv){

	//no input
	if(argc != 2)
		return 0;
	
	char* fileName = argv[1];
	FILE* file;
	char ch;
	int value;
		
	file = fopen(fileName, "r");
	
	//file does not exist
	if(file == NULL){
	  printf("error");
		return -1;
	}
	
	node* first = NULL;
	while(fscanf(file, "%c	%d\n", &ch, &value) != EOF){
		
		if(ch == 'i'){
			node* prev;
			node* temp;
			
			if(first == NULL){
				first = (node*)malloc(sizeof(node));
				first->data = value;
				first->p = NULL;
				
			}else if(value == first->data){
				continue;
				
			}else if(value < first->data){
				node* insert = (node*)malloc(sizeof(node));
				insert->data = value;
				insert->p = first;
				first = insert;
			
			}else{
				
				temp = first;
				while(temp){ //stops at end of list
				
					if(value == temp->data){
						break;
				
					}else if(value < temp->data){
						node* insert = (node*)malloc(sizeof(node));
						insert->data = value;
						insert->p = prev->p;
						prev->p = insert;
						break;
					
					}
					
					prev = temp;
					temp = temp->p;
				
					if(temp == NULL){
						node* insert = (node*)malloc(sizeof(node));
						insert->data = value;
						insert->p = NULL;
						prev->p = insert;
						break;
					
					}
					
				}
			
			}		
		
		//For deleting nodes
		}else if(ch == 'd'){
		
			//Nothing happens if no list exists
			if(first == NULL)
				continue;
				
			node* temp;
			node* prev;
		
			//deletes first node
			if(value == first->data){

				temp = first;
				first = first->p;
				free(temp);
				continue;
				
			}
		
			prev = first;
			temp = first->p; //iterates through the list to find the node to delete
			while(temp != NULL){
				
				if(value == temp->data){
					prev->p = temp->p;
					free(temp);
					break;
				}	
				
				prev = temp;
				temp = temp->p;
				
			}
		
		}else{
			//formatting mistake
			printf("error");
			return 0;
		
		}
		
	}
	
	//for whatever reason its not at eof (formatting error)
	if(!feof(file)){
		printf("error");
		return 0;
	}

	//this loop prints the nodes and frees them right after	
	node* forFree;
	while(first != NULL){

		printf("%d\t", first->data);
		forFree = first;
		first = first->p;
		free(forFree);
		
	}
	
	//printf("\b\n");//this line exists because there must not be trailing whitespace (last tab) in the output
	//nvm autograder hates that
	fclose(file);
	return 0;
}
