#include "../include/Token.h"


void init_tokenlist(toklist_t* tl) {
	tl->tokens = (struct Token*)malloc(sizeof(struct Token));
	tl->size = 1;
}


void destroy_tokenlist(toklist_t* tl) {
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


tok_t create_token(char* tok, toktype_t type, bool isint) {
	tok_t token = {
		.type = type,
		.isint = isint,
		.tok = (char*)calloc(30, sizeof(char)),
		.lastTok = false,
	};

	const char* eol = "\0";
	strcat(token.tok, tok);
	strcat(token.tok, eol);

	return token;
}
