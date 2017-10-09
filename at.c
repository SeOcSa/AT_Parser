/*
* at.c
*
*  Created on: Oct 8, 2017
*      Author: zsofi
*/

#include "at.h"

#define INITIAL_STATE 0
#define END_STATE 12
#define CR '\r'
#define LF '\n'
#define OK "OK"
#define ERROR_STATE 999

int state = INITIAL_STATE;
int line_count = 0;

int data_count = 0;
char data[STRING_SIZE + 1];
void succeed(int line_cnt, char* data, st_answer* result);
void fail(st_answer* answer);
void reset(st_answer* result);

void at_parser(char ch, st_answer *result){

	switch (state){
	case INITIAL_STATE:
		if (ch == CR){
			state = 1;
		}
		succeed(line_count, NULL, result);
		break;
	case 1:
		if (ch == LF){
			state = 2;
			line_count++;
			succeed(line_count, NULL, result);
			break;
		}
		fail(result);
		break;
	case 2:
		switch (ch){
		case 'O':
			state = 3;
			strcpy(data, "\0");
			strcat(data, "O");
			succeed(line_count, NULL, result);
			break;
		case 'E':

			state = 5;
			strcpy(data, "\0");
			strcat(data, "E");
			succeed(line_count, NULL, result);
			break;
		case '+':
			state = 10;
			strcpy(data, "\0");
			strcat(data, "+");
			succeed(line_count, NULL, result);
			break;
		default:
			fail(result);
			break;
		}
		break;
	case 3:
		if (ch == 'K'){
			state = 4;
			strcat(data, "K");
			succeed(line_count, data, result);
		}
		else{
			fail(result);
		}
		break;
	case 5:
		if (ch == 'R'){
			state = 6;
			strcat(data, "R");
			succeed(line_count, NULL, result);
		}
		else{
			fail(result);
		}
		break;
	case 6:
		if (ch == 'R'){
			state = 7;
			strcat(data, "R");
			succeed(line_count, NULL, result);
		}
		else{
			fail(result);
		}
		break;
	case 7:
		if (ch == 'O'){
			state = 8;
			strcat(data, "O");
			succeed(line_count, NULL, result);
		}
		else{
			fail(result);
		}
		break;
	case 8:
		if (ch == 'R'){
			state = 9;
			strcat(data, "R\0");
			succeed(line_count, data, result);
			//				resulting_struct.data[0] = "ERROR";
			//				strncpy(resulting_struct.data[0], OK, strlen(OK));
		}
		else{
			fail(result);
		}
		break;
	case 10:
		if (ch == CR){
			state = 11;
		}
		succeed(line_count, NULL, result);
		break;
	case 4:
	case 9:
		if (ch == CR){
			state = 11;
		}
		else{
			fail(result);
		}
		break;
	case 11:
		if (ch == LF){
			reset(result);
		}
	}
	return;
}


void succeed(int line_cnt, char* data, st_answer* result){

	result->success = TRUE;
	result->line_count = line_cnt;
	if (data){
		strncpy(result->data[data_count], data, strlen(data) + 1);
	}
	return;
}

void fail(st_answer* answer){
	answer->success = FALSE;
	reset(answer);
}

void reset(st_answer* result){
	result->success = END;
	state = INITIAL_STATE;
	line_count = 0;
	data_count = 0;
	strcpy(data, "\0");
}