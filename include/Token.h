#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


typedef enum TokenType {
	T_PRINT,
	T_INT,
	T_STR,
	T_END_STATEMENT,
	T_EOL,
	T_PUSH,
	T_VAR,
	T_ARROW,
	T_IF_STATEMENT,
	T_DO,
	T_TRUE,
	T_FALSE,
	T_END,
	T_EQUALS,
	T_PLUS,
} toktype_t;

typedef struct Token {
	char* tok;
	toktype_t type;
	bool isint;
	bool lastTok;
	bool heapAlloc;
	unsigned long lineNum;
} tok_t;


typedef struct Tokenlist {
	struct Token* tokens;
	size_t size;
	size_t elements;
} toklist_t;


void init_tokenlist(toklist_t* tl);
void destroy_tokenlist(toklist_t* tl);
void add_element(toklist_t* tl, tok_t element);
tok_t create_token(char* tok, toktype_t type, bool isint, unsigned long lineNum);
bool isint(char e);

#endif
