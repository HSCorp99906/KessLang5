#include "../include/Runtime.h"


extern struct VarTable varTable;


void execute(struct AST_NODE** ast) {
	/*
	 * This is the Runtime program,
	 * it reads the AST and executes
	 * based on AST nodes.
	 */

	static bool canPush = true;

	struct AST_NODE* node = ast_locate(ast, "type");

	if (strcmp(node->value, "print-statement") == 0) {
		if (strcmp(node->child->child->value, "true") != 0) {  // Is var str boolean.
			printf("%s\n", node->child->value);
		} else {

			struct Var* printVar = varLocate(varTable, node->child->value);

			if (!(printVar)) {
				printf("FATAL: Please report this issue on our GitHub.", node->child->value);
			} else {
				switch (printVar->datatype) {
					case INT_PTR:
						{
							int* addr = (int*)printVar->value;
							printf("%p\n", addr);
						}
						break;
				}
			}

			free(node->child->value);
		}
		
		free(node->child->child->value);
		free(node->child->child);
		node->child->child = NULL;
	} else if (strcmp(node->value, "push-instruction") == 0) {
		if (node->child->usingValueINT) {
			register int pushVal asm("rax") = node->child->valueINT;
			__asm__("push rax");
			register int* curStkPtrAddr asm("esp");
			__asm__("pop rax");  // Pops value off stack (for now).	

			struct Var* var = varLocate(varTable, node->child->child->value);
			var->value = curStkPtrAddr;
			var->datatype = INT_PTR;

			free(node->child->child);
			node->child->child = NULL;
		}
	}
}
