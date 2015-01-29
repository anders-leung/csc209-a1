#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv) {
	int opt = 0;
	int delay = 8000;
	int volume_scale = 4;
	int source_index = 1;
	int dest_index = 2;
	FILE *input, *input2, *output;

	if (argc == 3 || argc == 5 || argc == 7) {
		while ((opt = getopt(argc, argv, "d:v:")) > -1) {
			switch (opt) {
			case 'd':
				delay = strtol(optarg, NULL, 10);
				break;
			case 'v':
				volume_scale = strtol(optarg, NULL, 10);
				break;
			default:
				fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourvewav destwav\n", argv[0]);
				exit(1);
			}
		}
		if (optind >= argc) {
			fprintf(stderr, "Expected argument after options\n");
			exit(1);
		}
		if (argc == 5) {
			source_index = 3;
			dest_index = 4;
		}
		if (argc == 7) {
			source_index = 5;
			dest_index = 6;
		}
	
		input = fopen(argv[source_index], "r");
		fseek(input, 0, SEEK_END);
		long size = ftell(input);
		fseek(input, 0, SEEK_SET);
		input2 = fopen(argv[source_index], "r");
		output = fopen(argv[dest_index], "w+");
		char header[44];
		size_t bytes;
		bytes = fread(header, 1, 4, input);
		fwrite(header, 1, 4, output);
		int integer;
		fread(&integer, sizeof(integer), 1, input);
		integer = integer + (delay * 2);
		fwrite(&integer, sizeof(integer), 1, output);
		bytes = fread(header, 1, 32, input);
		fwrite(header, 1, bytes, output);
		fread(&integer, sizeof(integer), 1, input);
		integer = integer + (delay * 2);
		fwrite(&integer, sizeof(integer), 1, output);
		fseek(input2, 44, 0);
		short orig[delay], echo[delay];
		bytes = fread(orig, sizeof(short), delay, input);
		fwrite(orig, sizeof(short), bytes, output);
		long current = 44 + (delay * 2);
		int i;
		while (!feof(input) && !ferror(input)) {
			if ((size - current) > delay * 2) {
				fread(orig, sizeof(short), delay, input);
				fread(echo, sizeof(short), delay, input2);
				for (i = 0; i < delay; i++) {
					echo[i] = echo[i] / volume_scale;
					echo[i] = echo[i] + orig[i];
				}
				fwrite(echo, sizeof(short), delay, output);
				current = current + delay * 2;
			} else {
				bytes = fread(orig, sizeof(short), ((size - current) / 2), input);
				fread(echo, sizeof(short), bytes, input2);
				for (i = 0; i < ((size - current) / 2); i++) {
					echo[i] = echo[i] / volume_scale;
					echo[i] = echo[i] + orig[i];
				}
				fwrite(echo, sizeof(short), bytes, output);
				fread(orig, 1, 1, input);
			}
		}
		int zero_samples = delay - ((size - 44) / 2);
		if (zero_samples > 0) {
			for (i = 0; i < zero_samples; i++) {
				short zero = 0;
				fwrite(&zero, sizeof(short), 1, output);
			}
		}
		bytes = fread(echo, sizeof(short), delay, input2);
		for (i = 0; i < delay; i++) {
			echo[i] = echo[i] / volume_scale;
		}
		fwrite(echo, sizeof(short), bytes, output);
		fclose(input);
		fclose(input2);
		fclose(output);
	}

	else {
		fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] <file> <file>\n", argv[0]);
		exit(1);
	}

	return 0;
}

