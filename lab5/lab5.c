#include <ctype.h>
#include <stdio.h>

#define PAGE_SIZE 4096

void outputPageAndOffset(unsigned long address) {
	  printf("The address %lu contains:\n", address);
	  printf("page number = %lu\n", address / PAGE_SIZE);
	  printf("offset = %lu\n", address % PAGE_SIZE);
}

int main(int argc, char const *argv[]) 
{
  	if (argc <= 1) {
    	  fprintf(stderr, "Error, not enough arguments.\n");
    	  return -1;
  	}
 
  	for(int i = 0; argv[1][i] != '\0'; i++) {
  		  if(!isdigit(argv[1][i])){
    		    fprintf(stderr, "Error, address value is not a non-negative integer.\n");
    		    return -1;
  		  }
  	}

    unsigned long address;
  	sscanf(argv[1], "%lu", &address);

	  outputPageAndOffset(address);
	  return 0;
} 
