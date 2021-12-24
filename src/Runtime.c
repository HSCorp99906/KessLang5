#include "../include/Runtime.h"


void run(struct AST_NODE** ast) {
	struct AST_NODE* tok = ast_locate(ast, "type");

	if (strcmp(tok->value, "print-statement") == 0) {
		printf("%s\n", tok->child->value);
	}
}
