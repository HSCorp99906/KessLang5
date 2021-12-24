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
					// printf("%s\n", parse_peek(*parser, parser->curIndex).tok);

					struct AST_NODE* print_node = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node, "type", "print-statement", 0, false);
					ast_insert(head_node, print_node, s);
					print_node->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node->child, "arg", parse_peek(*parser, parser->curIndex).tok, 0, false);

					// printf("%s\n", print_node->child->value);

					// free(print_node->child);

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
			++i;
			return head_node;
		}
	}

	ast_destroy(&head_node, *s);
	return NULL;
}


struct Token parse_peek(struct Parser p, unsigned int index) {
	return p.tokenList.tokens[index];
}
