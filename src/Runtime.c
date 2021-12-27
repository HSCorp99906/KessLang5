#include "../include/Runtime.h"


void execute(struct AST_NODE** ast) {
	/*
	 * This is the Runtime program,
	 * it reads the AST and executes
	 * based on AST nodes.
	 */

	static bool canPush = true;

	struct AST_NODE* node = ast_locate(ast, "type");

	if (strcmp(node->value, "print-statement") == 0) {
		printf("%s\n", node->child->value);
	} else if (strcmp(node->value, "push-instruction") == 0) {
		if (node->child->usingValueINT) {
			register int pushVal asm("rax") = node->child->valueINT;
			__asm__("push rax");
			printf("%d pushed to stack.\n", pushVal);
			register int* curStkPtrAddr asm("esp");
			printf("Current address at stack pointer => %p\n", curStkPtrAddr);
			__asm__("pop rax");  // Pops value off stack (for now).
		}
	}
}
