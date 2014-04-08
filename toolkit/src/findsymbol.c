#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *table;

void search_symbol(char *name)
{
	
}

int main( int argc, const char* argv[])
{
	if(argc < 3)
	{
		printf("usage: %s file [symbol_to_find]\n",argv[0]);
		return 1;
	}
	const char *filename = argv[1];
	table = fopen(filename, "r");

	if (table == NULL) {
		fprintf(stderr, "%s failed to open!\n",filename);
		return 2;
	}
}