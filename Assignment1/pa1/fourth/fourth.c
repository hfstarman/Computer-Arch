#include<stdio.h>
#include<stdlib.h>


int main(int argc, char** argv){

//checks to see if there is a file or if there are too many inputs
	if(argc != 2)
		return 0;

	char* fileName = argv[1];
	FILE* file = fopen(fileName, "r");
	
	//checks if the file exists
	if(file == NULL){
		printf("error");
	}
	
	int rows, columns, i, j, add;
	
	//retrieving the size of the matrix
	fscanf(file, "%d	%d", &rows, &columns);
	/*creating matrixA, which will also be the final matrix
	this matrix is atypical, instead of it being an array
	of arrays, I came across the method of malloc-ing
	the whole matrix to one pointer. You will have to 
	type [i*columns + j] to get to the location in the matrix
	you require.*/
	int* matrixA = (int*)malloc(sizeof(int)*rows*columns);
	
	//inputting the number into the matrix
	for(i = 0; i < rows; i++){
		for(j = 0; j < columns; j++){
			fscanf(file, "%d", &matrixA[i*columns+j]);
		}
		
	}

	//these loops add the values of the next matrix to matrixA
	for(i = 0; i < rows; i++){
	  for(j = 0; j < columns; j++){
	  	fscanf(file, "%d", &add); //gets values from file
	  	matrixA[i*columns+j] += add; //adds the values to its respective place
	  }
	}
	
	//this will print the matrix          
	for(i = 0; i < rows; i++){
	  for(j = 0; j < columns; j++){
			printf("%d\t", matrixA[i*columns+j]);
		}
		
		printf("\n"); //needed to separate the rows in the visual

	}

	free(matrixA);
	return 0;

}
