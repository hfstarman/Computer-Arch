#include<stdio.h>
#include<stdlib.h>

/*NOTE: this is a direct copy of eighth.c except for the delete function added*/

//defining the Binary Tree Nodes
typedef struct btNode btNode;
struct btNode{

  int data;
  int height;
  btNode *leftChild;
  btNode *rightChild;

};
//struct btNode* apple;

/*uses preorder traversal to search the tree for slightly faster
efficiency. Returns the height of the found node and returns 0
if the value does not exist is the tree*/
//forgot that I can do a logn search rather simply but instead
//im searching the entire tree for the number
int btSearch(btNode *root, int value, int height){
  
  //will check to see if the node exists
  if(root == NULL)
    return 0;
  
  height++;

  //checks if the searched for node is this one
  if(value == root->data)
    return height;

  //goes to leftChild and starts process again
  //will only return if the value is found
  int left =  btSearch(root->leftChild, value, height);
  if(left != 0)
    return left;
  
  //same as above except with rightChild
  int right = btSearch(root->rightChild, value, height);
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


//this will do the actual deleting, it takes the root, the node
//to be deleted and its parent
btNode* btDelHelper(btNode* root, btNode* temp, btNode* parent){ /*<------------------------------btDelHelper*/

  btNode* toFree;
  //deleting the root node
  if(temp == NULL){
    
    //no children
    if(root->rightChild == NULL && root->leftChild == NULL){
      root = NULL;
      return root;
    }

    //one child
    if(root->rightChild != NULL && root->leftChild == NULL){
      toFree = root;
      root = root->rightChild;
      toFree->rightChild = NULL;
      free(toFree);
      return root;
    }
    
    //one child, the other one
    if(root->rightChild == NULL && root->leftChild != NULL){
      toFree = root;
      root = root->leftChild;
      toFree->leftChild = NULL;
      free(toFree);
      return root;
    }

//DELETING WITH 2 CHILDREN: ROOT
    btNode* parent2 = root;
    toFree = root->rightChild;
    
    while(toFree->leftChild != NULL){
      parent2 = toFree;
      toFree = toFree->leftChild;
    }

    root->data = toFree->data;

    if(parent2 == root)
	parent2->rightChild = toFree->rightChild;	
      else
	parent2->leftChild = toFree->rightChild;
    
    toFree->rightChild = NULL;
    free(toFree);
    return root;

//------------------------------------------
  }else{
    //no children for temp
    if(temp->leftChild == NULL && temp->rightChild == NULL){
      if(parent->rightChild == temp){
	
	parent->rightChild = NULL;
	free(temp);
	return root;
      
      }else{
      
	parent->leftChild = NULL;
	free(temp);
	return root;

      }
      
    //one child, the right one
    }else if(temp->rightChild != NULL && temp->leftChild == NULL){
      if(parent->rightChild == temp){
	parent->rightChild = temp->rightChild;
	free(temp);
	return root;
      }else{
	parent->leftChild = temp->rightChild;
	free(temp);
	return root;
      }
      
      //one child, the left one
    }else if(temp->rightChild == NULL && temp->leftChild != NULL){
      if(parent->rightChild == temp){
	parent->rightChild = temp->leftChild;
	free(temp);
	return root;
      }else{
	parent->leftChild = temp->leftChild;
	free(temp);
	return root;
      }

     
    }else{
//DELETING WITH 2 CHILDREN: NOT ROOT
//------------------------------------------
      btNode* replace = temp->rightChild;
      parent = temp;
      while(replace->leftChild != NULL){
	parent = replace;
	replace = replace->leftChild;
      }
      
      temp->data = replace->data;

      
      if(parent == temp)
	parent->rightChild = replace->rightChild;	
      else
	parent->leftChild = replace->rightChild;


      replace->rightChild = NULL;
      
      free(replace);
      return root;

//------------------------------------------
    }
	  
  }

  return root;
}




//this function will delete a node with a specific value
//it will return 1 upon success or 0 upon failure
btNode* btDelete(btNode* root, int value){  /* <--------------------------------------btDelete*/
  
  if(root == NULL){
    printf("fail\n");
    return root;
  }
  
  //root->height = 1;
  if(btSearch(root, value, 1) == 0){
    printf("fail\n");
    return root;
  }
 
  //checks if the node to be deleted is the root
  if(root->data == value){
    
    root = btDelHelper(root, NULL, NULL);
    //correctHeight(root, 1);
    printf("success\n");
    return root;

  }

  btNode *temp = root, *parent;
  //loop to find the node to be deleted
  while(temp->data != value){
    
    //moves temp accordingly
    if(value > temp->data){
      parent = temp;
      temp = temp->rightChild;
    }else{
      parent = temp;
      temp = temp->leftChild;
    }

  }
  root = btDelHelper(root, temp, parent);
  printf("success\n");
  return root;
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
	  int falseHeight;
	  falseHeight = btInsert(root, value, 1);
	  falseHeight++;
	  height = btSearch(root, value, 0);

	  if(height != 0){
	    printf("inserted %d\n", height);
	  }else{
	    printf("duplicate\n");
	  }
	  
	}

      }else if(ch == 's'){
	
	//records the height of where the node was found
	//height will be 0 if the node was not found
	height = btSearch(root, value, 0);

	if(height != 0){
	  printf("present %d\n", height);
	}else{
	  printf("absent\n");
	}
	
      }else if(ch == 'd'){
        
	root = btDelete(root, value);

      }else if(ch!='i' && ch!='s' && ch!='\n'){ //ch was not 'i' nor 's' so there was improper formatting

	printf("error\n");

      }

  }

  postFree(root);
  fclose(file);
  return 0;

}
