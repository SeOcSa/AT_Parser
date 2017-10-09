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
		at_parser(c, &resulting_struct);
		printf("%d\n",resulting_struct.success);
		if (resulting_struct.success == 2 && resulting_struct.data){
			printf("%s\n",resulting_struct.data);
		}
		fread(&c, sizeof(char), 1, file);
	}
	return 0;
}
