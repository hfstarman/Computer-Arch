#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){

	//checks if there are not enough of too many arguments
	if(argc != 2)
		return 0;

	char* fileName = argv[1];
	FILE* file = fopen(fileName, "r");
	
	//check for if the file exists
	if(file == NULL){
		printf("error");
		return 0;
	}
	
	//creating the main varialbes used in the code and for iteration with for loops
	int rows1, rows2, columns1, columns2;
	int product = 0;
	int i, j, k;
	
	//finds the size of the first matrix
	fscanf(file, "%d	%d", &rows1, &columns1);
	
	//allocates the memory for the first matrix
	//see fourth assignment for thought process
	int* matrixA = (int*)malloc(sizeof(int)*rows1*columns1);
	
	//fills the first matrix with the integers from file
	for(i = 0; i < rows1; i++){
		for(j = 0; j < columns1; j++){
			fscanf(file, "%d", &matrixA[i*columns1+j]);
		}
	}
	
	//gets the size of the second matrix from the file
	fscanf(file, "%d	%d", &rows2, &columns2);
	
	//checks if the matricies are compatible for multiplication
	if(rows2 != columns1){
		printf("error");
		return 0;
	}

	//creates second matrix
	int* matrixB = (int*)malloc(sizeof(int)*rows2*columns2);
	
	//inputs the values into the second matrix from file
	for(i = 0; i < rows2; i++){
		for(j = 0; j < columns2; j++){
			fscanf(file, "%d", &matrixB[i*columns2+j]);
		}
	}
	
	//create the matrix will values of the multiplication of A and B
	int* result = (int*)malloc(sizeof(int)*rows1*columns2);
	
	//multiplies the matrix A and B and places values into result
	for(i = 0; i < rows1; i++){
		for(j = 0; j < columns2; j++){
			for(k = 0; k < rows2; k++){
				product += matrixA[i*rows2+k] * matrixB[k*columns2+j];
			}
			
			//inputs the product into the result		
			result[i*columns2+j] = product;
			product = 0;
			
		}
	}
	
	//this will print the matrix          
	for(i = 0; i < rows1; i++){
	  for(j = 0; j < columns2; j++){
	    printf("%d\t", result[i*columns2+j]);
	  }
	                  
	  printf("\n"); //needed to separate the rows in the visual
	                      
	}
	
	//free all matricies
	free(matrixA);
	free(matrixB);
	free(result); 
	return 0;

}
