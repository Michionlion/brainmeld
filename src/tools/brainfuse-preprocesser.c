#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include-directive.c"
#include "put-directive.c"

#define DIRECTIVE_CHAR '~'

char* loadDirective(char* start, int length) {
	int i;
	char* operand_start = NULL;
	int operand_length = 0;
	int directive_length = 0;
	int in_operand = 0;
	for(i=0; i < length; i++) {
		if((start[i] == '<' || start[i] == '"') && in_operand == 0) {
			in_operand = 1;
			operand_start = start + i + 1;


			//remove trailing spaces from directive pointer length
			int k = i;
			while(start[--k]==' ') {
				directive_length--;
			}

		} else if ((start[i] == '<' || start[i] == '"') && in_operand == 1) {
			in_operand = -1; // no more operand
		} else if(in_operand == 1) {
			//inside operand
			operand_length++;
		} else if(in_operand == 0) {
			//not in operand, add to directive
			directive_length++;
		} else {
			//after operand
		}

		char* op = (char*) malloc(operand_length+1);
		strncpy(operand_start, op, directive_length);
		op[directive_length] = '\0';
		if(strncmp(start, "include", directive_length) == 0) {
			//include direction
			return include(op);
		} else if(strncmp(start, "put", directive_length) == 0) {
			//put directive
			return put(op);
		}


	}

	if(operand_start == NULL) {
		fprintf(stderr, "OPERAND NOT SPECIFIED IN DIRECTIVE");
		return NULL;
	}




	//load directive body (whatever it is)


	return "{dir_body}";
}

char* loadFile(char* name) {

	return "";
}

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Usage: bfp source destination\n");
		return 1;
	}

	FILE* source = fopen(argv[1], "r");
	FILE* dest = fopen(argv[2], "w");
	if(source == NULL || dest == NULL) {
		printf("Error");
		return -1;
	}

	char ch;
	bool in_dir = false;
	char* dir_start;
	int dir_length = 0;
	while((ch=fgetc(source)) != EOF) {
		if(ch==DIRECTIVE_CHAR && !in_dir) {
			//open new directive
			dir_start = (char*) malloc(20);
			in_dir = true;
		} else if (ch==DIRECTIVE_CHAR && in_dir) {
			//close directive
			//insert directive contents
			dir_start[dir_length++] = '\0';
			char* dir_contents = loadDirective(dir_start, dir_length);
			fputs(dir_contents, dest);

			//reset
			in_dir = false;
			dir_start = 0;
			dir_length = 0;
		} else if(in_dir) {
			//inside directive
			//putchar(ch);
			dir_start[dir_length++] = ch;
			//resize if needed
			if(dir_length > 15) dir_start = (char*) realloc(dir_start, dir_length+10);
		} else {
			//outside directive
			fputc(ch, dest);
		}
	}

	fclose(source);
	fclose(dest);

	//write output


	return 0;
}
