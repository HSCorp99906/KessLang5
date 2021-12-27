#include "../include/Parser.h"


struct AST_NODE** parse(struct Parser* parser, size_t* s) {
	struct AST_NODE** head_node = get_tree();
	ast_init(head_node);
	bool ignore = true;  // Ignores print statements on first run.
	static bool staticIgnore = true;

	static int i = 0;

	for (i; i < parser->tokenList.elements; ++i) {
		switch (parse_peek(*parser, parser->curIndex).type) {
			case T_PRINT:
				if (!(ignore)) {
					// printf("%s\n", parse_peek(*parser, parser->curIndex).tok);
					
					++parser->curIndex;
					struct AST_NODE* print_node = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node, "type", "print-statement", 0, false);
					ast_insert(head_node, print_node, s);
					print_node->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node->child, "arg", parse_peek(*parser, parser->curIndex).tok, 0, false);
					// free(print_node->child);

					++parser->curIndex;
					return head_node;
					
				} else {
					ignore = false;
				}
				break;
			case T_STR:
				++parser->curIndex;
				break;
			case T_INT:
				++parser->curIndex;
				break;
			case T_PUSH:
				{

					int lastIdx = parser->curIndex;
					bool arrowFound = false;

					while (parser->curIndex < parser->tokenList.elements - 2) {
						++parser->curIndex;

						if (parse_peek(*parser, parser->curIndex).type == T_ARROW) {
							if (arrowFound) {
								printf("SyntaxError: Expected one '=>' but found more.\n");
								free(head_node);
								return NULL;
							}

							arrowFound = true;
						}
					}

					if (!(arrowFound)) {
						printf("SyntaxError: Expected '=>'.\n");
						free(head_node);
						return NULL;
					}

					struct AST_NODE* push_node = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					
					char* tokValue = parse_peek(*parser, lastIdx + 1).tok;

					// TODO: Move most implementation from lexer for this section to here.
					
					init_node(push_node, "type", "push-instruction", 0, false); 

					push_node->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));		

					if (isint(tokValue[0])) {	
						init_node(push_node->child, "arg", NULL, atoi(tokValue), true); 
					} else {
						init_node(push_node->child, "arg", tokValue, 0, false);
					}

					ast_insert(head_node, push_node, s);

					++parser->curIndex;
					
					return head_node;
				}
		}
	}

	return NULL;
}


struct Token parse_peek(struct Parser p, unsigned int index) {
	return p.tokenList.tokens[index];
}
