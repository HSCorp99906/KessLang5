#include "../include/Parser.h"


struct AST_NODE** parse(struct Parser* parser, size_t* s) {
	struct AST_NODE** head_node = get_tree();
	ast_init(head_node);
	bool ignore = true;  // Ignores print statements on first run.

	static int i;

	for (i = 0; i < parser->tokenList.size; ++i) {
		switch (parser->tokenList.tokens[i].type) {
			case T_PRINT:
				if (!(ignore)) {
					printf("%s\n", parse_peek(*parser, parser->curIndex + 2).tok);

					++parser->curIndex;
					
				} else {
					ignore = false;
				}
				break;
			case T_STR:
				++parser->curIndex;
				break;
		}

		if (parse_peek(*parser, parser->curIndex).type == T_END_STATEMENT && !(parse_peek(*parser, parser->curIndex).lastTok)) {
			return head_node;
		}
	}

	ast_destroy(&head_node, *s);

	return NULL;
}


struct Token parse_peek(struct Parser p, unsigned int index) {
	return p.tokenList.tokens[index];
}
