#include "../include/Var.h"
#include <stdio.h>


int main() {
	struct VarData vdata = {
		.varHashes = (unsigned int*)malloc(sizeof(unsigned int*)),
		.varHashesSize = 1,
		.varHashesIdx = 0,
	};

	size_t varSize = 1;
	struct Var** varTable = (struct Var**)malloc(sizeof(struct varTable*));
	var_init(varTable, varSize);

	struct Var* testVar = (struct Var*)malloc(sizeof(struct Var*));
	testVar->key = "somingV2ar";
	var_insert(testVar, varTable, &vdata, varSize);

	if (var_locate(varTable, vdata, "somingV2ar", varSize) == NULL) {

	}

	var_destroy(varTable, &vdata);
	varTable = NULL;
}
