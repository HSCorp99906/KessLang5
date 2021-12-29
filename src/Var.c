#include "../include/Var.h"
#include <stdio.h>


void var_insert(struct VarTable* varTable, struct Var* var) {
	varTable->vars[varTable->varIdx] = var;
	++varTable->varIdx;
	++varTable->size;
	varTable->vars = (struct Var**)realloc(varTable->vars, sizeof(struct Var) * varTable->size);
}

void initVarTable(struct VarTable* varTable) {
	varTable -> vars = (struct Var**)malloc(sizeof(struct Var*));
	varTable->size = 1;
	varTable->varIdx = 0;
}


struct Var* varLocate(struct VarTable varTable, const char* const KEY) {
	for (int i = 0; i < varTable.size - 1; ++i) {
		if (strcmp(varTable.vars[i]->key, KEY) == 0) {
			return varTable.vars[i];
		}
	}

	return NULL;
}
