#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr, "blah");
		exit(1);
	}
	FILE *input = fopen(argv[1], "r");
	FILE *output = fopen(argv[2], "w+");
	int delay = 12000;
	int volume_scale = 4;
	int size = 44 + (delay * 2);
	char header[size];
	size_t bytes;
	bytes = fread(header, 1, 4, input);
	fwrite(header, 1, bytes, output);
	int integer;
	fread(&integer, sizeof(integer), 1, input);
	integer = integer + (delay * 2);
	fwrite(&integer, sizeof(integer), 1, output);
	bytes = fread(header, 1, 32, input);
	fwrite(header, 1, bytes, output);
	fread(&integer, sizeof(integer), 1, input);
	integer = integer + (delay * 2);
	fwrite(&integer, sizeof(integer), 1, output);
	return 0;
}
