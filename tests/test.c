#include "../include/Var.h"
#include <stdio.h>


int main() {
	int val = 2;

	struct Var var1 = {
		.key = "var1",
		.value = &val,
	};

	struct VarTable varTable;
	initVarTable(&varTable);

	var_insert(&varTable, var1);

	printf("%s\n", varLocate(varTable, "var1")->key);

	free(varTable.vars);
}
