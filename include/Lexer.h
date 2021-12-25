#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "Token.h"
#include "split.h"
#include <stdlib.h>
#include <ctype.h>

struct Lexer {
	uint32_t lineNum;
	uint32_t colNum;
	char* curPart;
	char curChar;
	bool error;
};


void tokenize(toklist_t* toklist, struct Lexer* lexer, char* line);
char* peek(unsigned int index, char* line);
void advance(struct Lexer* lexer, char* line);

#endif
