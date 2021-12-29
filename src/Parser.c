#include "../include/Parser.h"

extern struct VarTable varTable;


struct AST_NODE** parse(struct Parser* parser, size_t* s) {
	struct AST_NODE** head_node = get_tree();
	ast_init(head_node);
	bool ignore = true;  // Ignores print statements on first run.
	static bool staticIgnore = true;

	static int i = 0;

	while (parser->curIndex < parser->tokenList.elements) {
		switch (parse_peek(*parser, parser->curIndex).type) {
			case T_PRINT:
				{
					// printf("%s\n", parse_peek(*parser, parser->curIndex).tok);

					++parser->curIndex;

					char* str = parse_peek(*parser, parser->curIndex).tok;
					
					char* varName = (char*)calloc(strlen(str) + 2, sizeof(char));
					unsigned long varNameIdx = 0;
					bool isVar = str[0] == '$' ? true : false;
					char* isVarStr = (char*)calloc(6, sizeof(char));
					strcpy(isVarStr, "false");

					// TODO: ALLOW MULTIPLE VARIABLES.
					
					if (str[0] == '$') {
						strcpy(isVarStr, "true");
						for (int i = 1; i < strlen(str); ++i) {
							varName[varNameIdx] = str[i];
							++varNameIdx;
						}
					}

					if (isVar) {
						if (!(varLocate(varTable, varName))) {
							printf("SymbolError: '%s' is undefined.\n", varName);
							free(isVarStr);
							free(head_node);
							return NULL;
						}
					}

					struct AST_NODE* print_node = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node, "type", "print-statement", 0, false);
					ast_insert(head_node, print_node, s);
					print_node->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));

					if (!(isVar)) {
						init_node(print_node->child, "arg", parse_peek(*parser, parser->curIndex).tok, 0, false);
					} else {
						init_node(print_node->child, "arg", varName, 0, false);
					}

					print_node->child->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(print_node->child->child, "is-var", isVarStr, 0, false);

					++parser->curIndex;

					return head_node;
					
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
					bool varFound = false;
					bool run = true;

					while (parser->curIndex < parser->tokenList.elements && run) {
						if (parse_peek(*parser, parser->curIndex).type == T_END_STATEMENT) {
							run = false;
						}

						++parser->curIndex;

						if (parse_peek(*parser, parser->curIndex).type == T_ARROW) {
							if (arrowFound) {
								printf("SyntaxError: Expected one '=>' but found more.\n");
								free(head_node);
								return NULL;
							}

							arrowFound = true;
						} else if (parse_peek(*parser, parser->curIndex).type == T_VAR) {
							if (varFound) {
								free(head_node);
								printf("SyntaxError: Unexpected token.\n");
								return NULL;
							}

							varFound = true;
						}

						if (!(run)) {
							parser->curIndex += 2;
						}
					}

					if (!(arrowFound)) {
						printf("SyntaxError: Expected '=>'.\n");
						free(head_node);
						return NULL;
					} else if (!(varFound)) {
						printf("SyntaxError: Expected variable declaration after '=>'\n");
						free(head_node);
						return NULL;
					}

					struct AST_NODE* push_node = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					
					char* tokValue = parse_peek(*parser, lastIdx + 1).tok;
					
					init_node(push_node, "type", "push-instruction", 0, false);

					push_node->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));

					if (isint(tokValue[0])) {	
						init_node(push_node->child, "arg", NULL, atoi(tokValue), true); 
					} else {
						init_node(push_node->child, "arg", tokValue, 0, false);
					}

					struct Var* newVar = (struct Var*)malloc(sizeof(struct Var));
					newVar->key = parse_peek(*parser, parser->curIndex - 2).tok;
					newVar->value = NULL;
					var_insert(&varTable, newVar);

					push_node->child->child = (struct AST_NODE*)malloc(sizeof(struct AST_NODE));
					init_node(push_node->child->child, "var-arg", parse_peek(*parser, parser->curIndex - 2).tok, 0, false);	

					ast_insert(head_node, push_node, s);
					return head_node;
				}
		}
	}

	return NULL;
}


struct Token parse_peek(struct Parser p, unsigned int index) {
	return p.tokenList.tokens[index];
}
