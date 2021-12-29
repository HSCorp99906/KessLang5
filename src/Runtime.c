#include "../include/Runtime.h"


extern struct VarData vdata;
extern struct Var** varTable;
extern size_t varTableSize;


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
			struct Var* printVar = var_locate(varTable, vdata, node->child->value, varTableSize);
			
			switch (printVar->datatype) {
				case INT_PTR:
					{
						int* addr = (int*)printVar->value;
						printf("%p\n", addr);
					}

					break;
			}
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

			struct Var* newVar = var_locate(varTable, vdata, node->child->child->value, varTableSize);
			newVar->value = curStkPtrAddr;
	
			free(node->child->child);
			node->child->child = NULL;
		}
	}
}
