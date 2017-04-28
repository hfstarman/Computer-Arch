#include<stdio.h>
#include<stdlib.h>

//defining the Binary Tree Nodes
typedef struct btNode btNode;
struct btNode{

  int data;
  int height;
  btNode *leftChild;
  btNode *rightChild;

};


/*uses preorder traversal to search the tree for slightly faster
efficiency. Returns the height of the found node and returns 0
if the value does not exist is the tree*/
int btSearch(btNode *root, int value){
  
  //will check to see if the node exists
  if(root == NULL)
    return 0;
  
  //checks if the searched for node is this one
  if(value == root->data)
    return root->height;

  //goes to leftChild and starts process again
  //will only return if the value is found
  int left =  btSearch(root->leftChild, value);
  if(left != 0)
    return left;
  
  //same as above except with rightChild
  int right = btSearch(root->rightChild, value);
  if(right != 0)
    return right;

  return 0;
}

/*Method to insert into the binary tree.
will return the height of the node inserted
or will return 0 if there is a duplicate*/
int btInsert(btNode *root, int value, int height){

  /*NOTE: there is no check for (root == NULL) since the
    tree should never be empty when this function is called*/

  //check for duplicates
  if(value == root->data)
    return 0;
  
  //at this stage it will be forced into the next level of the tree
  height++;
  
  //checks if the value should go to the left or right of the root
  if(value > root->data){

    /*value is to the right, will create a new node to be inserted
    to root->rightChild if it does not already exist. Otherwise
    it will continue down the tree*/
    if(root->rightChild == NULL){
      btNode *insert = (btNode*)malloc(sizeof(btNode));
      insert->data = value;
      insert->height = height;
      insert->rightChild = NULL;
      insert->leftChild = NULL;
      root->rightChild = insert;
      return height;
    }else{
      return btInsert(root->rightChild, value, height);
    }

  }else{
    
    /*value is to the left, will create a new node to be inserted
    to root->leftChild if it does not already exist. Otherwise
    it will continue down the tree*/
    if(root->leftChild == NULL){
      btNode *insert = (btNode*)malloc(sizeof(btNode));
      insert->data = value;
      insert->height = height;
      insert->rightChild = NULL;
      insert->leftChild = NULL;
      root->leftChild = insert;
      return height;
    }else{
      return btInsert(root->leftChild, value, height);
    }

  }

  return -2; //function should never reach this spot
}


/*post traversal function that frees the nodes it hits
this needs to be post traversal. If it were pre only
the root node would be freed and there would be a memory
leak. If it were inorder only some of the nodes would be free
and again there would be a memory leak*/
void postFree(btNode *root){

  if(root == NULL)
    return;
  
  postFree(root->leftChild);
  postFree(root->rightChild);
  free(root);
  
  return;

}


int main(int argc, char** argv){

  //checks if there not enought or too many arguments
  if(argc != 2){
    printf("error");
    return 0;
  }

  //file variable creation
  char* fileName = argv[1];
  FILE* file = fopen(fileName, "r");

  //checks to see if the file exists
  if(file == NULL){
    printf("error");
    return 0;
  }


  char ch;
  int value, height;
  btNode *root = NULL;

  //main loop through file
  while(fscanf(file, "%c %d", &ch, &value) != EOF){

      //conditionals to determine what to do with the data
      if(ch == 'i'){
	
	//first node to be inserted will become root
	if(root == NULL){
	  
	  root = (btNode*)malloc(sizeof(btNode));
	  root->data = value;
	  root->height = 1;
	  root->leftChild = NULL;
	  root->rightChild = NULL;
	  printf("inserted 1\n");
	
	}else{
	  
	  /*records the height of where the node was inserted
	  height will be 0 if the value was already in the tree
	  (the 1 in the third argument is the start height)*/
	  height = btInsert(root, value, 1);
	  
	  if(height != 0){
	    printf("inserted %d\n", height);
	  }else{
	    printf("duplicate\n");
	  }
	  
	}


      }else if(ch == 's'){
	
	//records the height of where the node was found
	//height will be 0 if the node was not found
	height = btSearch(root, value);

	if(height != 0){
	  printf("present %d\n", height);
	}else{
	  printf("absent\n");
	}


      }else if(ch!='i' && ch!='s' && ch!='\n'){ //ch was not 'i' nor 's' so there was improper formatting

	printf("error\n");

      }

  }

  postFree(root);
  fclose(file);
  return 0;

}
