#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	FILE *input, *output;
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <file> <file>\n", argv[0]);
		exit(1);
	}
	
	if ((input = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		exit(1);
	}

	output = fopen(argv[2], "w+");

	char header[44];
	size_t bytes;
	bytes = fread(header, 1, sizeof(header), input);
	fwrite(header, 1, bytes, output);
	
	short left;
	short right;
	while (!feof(input) && !ferror(input)) {
		if (ferror(input)) {
			perror("fread");
			exit(1);
		}
		fread(&left, sizeof(left), 1, input);
		fread(&right, sizeof(right), 1, input);
		short combined = (left - right) / 2;
		fwrite(&combined, sizeof(combined), 1, output);
		fwrite(&combined, sizeof(combined), 1, output);
	}
		
	fclose(input);
	fclose(output);
	return 0;
}
