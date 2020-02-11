#include "my_malloc.h"

int main(int argc, char **argv) {
	int *number = my_malloc(sizeof(int));

	*number = 1;

	printf("We are number %d!\n", *number);

	return 0;
}

