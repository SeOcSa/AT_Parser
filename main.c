/*
 * main.c

 *
 *  Created on: Oct 8, 2017
 *      Author: zsofi
 */

#include "at.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	st_answer resulting_struct;

	//FILE* file_write = fopen(argv[1],"wb");
	//char writting[] = "\r\nOK";
	//fwrite(writting,strlen(writting), 1, file_write );
	//fclose(file_write);

	FILE* file = fopen(argv[1],"rb");
	if (!file){
		printf("File not found\n");
		return -1;
	}
	char c;
	fread(&c, sizeof(char), 1, file);
	while(!feof(file)){
//		printf("%x", c);
		at_parser(c, &resulting_struct);
		if (resulting_struct.success == 2 && resulting_struct.data){
			for (int i = 0 ; i < resulting_struct.data_count; i++){
				printf("%s\n",resulting_struct.data[i]);
			}
			printf("\n");
		}
		else{
			if(!resulting_struct.success){
				printf("Parsing ERROR at line: %d. Exiting...\n", resulting_struct.line_count);
				break;
			}
		}
		fread(&c, sizeof(char), 1, file);
	}
	if(feof(file) && resulting_struct.success == 1){
		printf("ERROR: End of file found before constructing the entire result\n\n");
		return 0;
	}
	return 0;
}
