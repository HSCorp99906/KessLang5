#include "../include/Var.h"
#include <stdio.h>


int main() {
	struct VarData vdata = {
		.varHashes = (unsigned int*)malloc(sizeof(unsigned int*)),
		.varHashesSize = 1,
		.varHashesIdx = 0,
	};

	free(vdata.varHashes);
}
