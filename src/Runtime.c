#include "../include/Runtime.h"


void execute(struct AST_NODE** ast) {
	/*
	 * This is the Runtime program,
	 * it reads the AST and executes
	 * based on AST nodes.
	 */

	struct AST_NODE* tok = ast_locate(ast, "type");

	if (strcmp(tok->value, "print-statement") == 0) {
		printf("%s\n", tok->child->value);
	}
}
