#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
  //will exit the program if there are no arguments or too many added
  if(argc < 2){
  	printf("Error, argument needed!\n");
  	exit(0);
  }else if(argc > 2){
  	printf("Error, too many arguments!\n");
  	exit(0);
  }
  
  //changes argv[1] from a pointer to an integer
  int num = atoi(argv[1]);
  
  if(num < 2){
    printf("no\n");
    exit(0);
  }
  
  //loop to check for divisors
  int i;
  for(i = 2; i < num; i++){
  	
  	if(num%i == 0){
  		printf("no\n");
  		exit(0);
  	}
  	
  }
  
  printf("yes\n");

  return 0;
}
