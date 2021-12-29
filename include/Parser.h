#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "Var.h"
#include "AST.h"

#undef START_SIZE


struct Parser {
	toklist_t tokenList;
	tok_t currentToken;
	uint32_t lineNum;
	size_t curIndex;
	bool error;
};



struct AST_NODE** parse(struct Parser* parser, size_t* s);
struct Token parse_peek(struct Parser p, unsigned int offset);


#endif
