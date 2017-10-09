/*
 * at.h
 *
 *  Created on: Oct 8, 2017
 *      Author: zsofi
 */

#ifndef AT_H_

#include <string.h>
#define AT_H_
#define ARRAY_SIZE 100
#define STRING_SIZE 100
#define TRUE 1;
#define FALSE 0;
#define END 2;

typedef struct st_answer{
	int success;
	int line_count;
	char data[ARRAY_SIZE][STRING_SIZE + 1];
}st_answer;

void at_parser(char ch, st_answer* result);

#endif /* AT_H_ */
