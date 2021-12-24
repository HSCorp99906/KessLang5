#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "AST.h"

#define MAX_BUFFER_SIZE 120


struct Parser {
	toklist_t tokenList;
	tok_t currentToken;
	uint32_t lineNum;
	size_t curIndex;
};



struct AST_NODE** parse(struct Parser* parser, size_t* s);
struct Token parse_peek(struct Parser p, unsigned int offset);


#endif
