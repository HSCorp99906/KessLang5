#include "../include/Var.h"
#include <stdio.h>


int main() {
	struct VarData vdata = {
		.varHashes = (unsigned int*)malloc(sizeof(unsigned int*)),
		.varHashesSize = 1,
		.varHashesIdx = 0,
	};

	size_t varSize = 1;
	struct Var** varTable = (struct Var**)malloc(sizeof(struct Var*));
	var_init(varTable, varSize);

	struct Var* testVar = (struct Var*)malloc(sizeof(struct Var));
	testVar->key = "somingV2ar";
	testVar->value = "AAA";
	var_insert(testVar, varTable, &vdata, varSize);

	struct Var* v = var_locate(varTable, vdata, "somingV2ar", varSize);

	if (v != NULL) {
		printf("%s\n", v->value);
	}

	var_destroy(varTable, &vdata);
	varTable = NULL;
}
