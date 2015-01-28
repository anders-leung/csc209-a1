#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	int delay = 8000;
	int volume_scale = 4;
	FILE *input, *output;
	
	if (argc == 3 || argc == 5 || argc == 7) {
		if (argc == 5) {
			if (strcmp(argv[1], "-d") == 0) {
				delay = argv[2];
			} else {
				volume_scale = argv[2];
			}
		}

		else if (argc == 7) {
			delay = argv[2];
			volume_scale = argv[4];
		}
	
		else {
			input = fopen(argv[-2], "r");
			output = fopen(argv[-1], "w+");
			char header[44 + (delay * 4)];
			fread(header, 1, 4, input);
			int integer;
			fread(&integer, sizeof(integer), 1, input);
			integer = integer + (delay * 2);
			fwrite(&integer, sizeof(integer), 1, header);
			fread(header, 1, 32, input);
			fread(&integer, sizeof(integer), 1, input);
			integer = integer + (delay * 2);
			fwrite(&integer, sizeof(integer), 1, header);
			fwrite(header, sizeof(header), 1, output);
			return 0;
		}
	}

	else {
		fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] <file> <file>\n", argv[0]);
		exit(1);
	}

	return 0;
