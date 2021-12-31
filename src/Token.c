#include "../include/Token.h"
#include <stdio.h>


bool isint(char e) {
	switch (e) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		default:
			return false;
	}
}


void init_tokenlist(toklist_t* tl) {
	tl->tokens = (struct Token*)malloc(sizeof(struct Token));
	tl->size = 1;
}


void destroy_tokenlist(toklist_t* tl) {
	for (int i = 0; i < tl->size; ++i) {
		if (tl->tokens[i].heapAlloc) {
			free(tl->tokens[i].tok);
		}
	}

	free(tl->tokens);
	tl->tokens = NULL;
	tl->size = 0;
}


void add_element(toklist_t* tl, tok_t element) {
	tl->tokens[tl->size - 1] = element;
	++tl->size;
	++tl->elements;
	tl->tokens = (struct Token*)realloc(tl->tokens, sizeof(struct Token) * tl->size);
}


tok_t create_token(char* tok, toktype_t type, bool isint, bool heapAlloc) {
	tok_t token = {
		.type = type,
		.isint = isint,
		.tok = (char*)calloc(30, sizeof(char)),
		.lastTok = false,
		.heapAlloc = heapAlloc,
	};

	const char* eol = "\0";
	strcat(token.tok, tok);
	strcat(token.tok, eol);

	return token;
}
