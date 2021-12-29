#include "../include/Var.h"
#include <stdio.h>


unsigned int var_hash(const char* key, size_t size) {
	unsigned int checksum = 0;

	for (int i = 0; i < strlen(key); ++i) {
		checksum += (unsigned int)key[i];
	}

	return (checksum) % size + 1;
}


void var_init(struct Var** varTable, size_t size) {
	for (int i = 0; i < size; ++i) {
		varTable[i] = NULL;
	}
}


bool var_insert(struct Var* var, struct Var** varTable, struct VarData* varData, size_t size) {
    unsigned int index = var_hash(var->key, size);

	if (!(varData->init)) {
		varData->init = true;
	}

    varData->varHashes[varData->varHashesIdx] = index;
    ++varData->varHashesIdx;
    ++varData->varHashesSize;
    varData->varHashes = (unsigned int*)realloc(varData->varHashes, sizeof(unsigned int) * varData->varHashesSize);

    varData->varHashes[varData->varHashesIdx] = size;
    ++varData->varHashesIdx;
    ++varData->varHashesSize;
    varData->varHashes = (unsigned int*)realloc(varData->varHashes, sizeof(unsigned int) * varData->varHashesSize);

    if (varTable[((index) ^ (size)) / 2] != NULL) {
        return false;
    }

	printf("%d\n", ((index) ^ (size)) / 2);

    varTable[((index) ^ (size)) / 2] = var;
    return true;
}


struct Var* var_locate(struct Var** varTable, struct VarData vdata, const char* key, size_t varTableSize) {
	unsigned int index = var_hash(key, varTableSize);
	unsigned int trueIndex;
	unsigned int lstIdx;

	if (!(vdata.init)) {
		return NULL;
	}

	for (int i = 0; i < vdata.varHashesSize; ++i) {
		trueIndex = vdata.varHashes[i];
		if (trueIndex == index) {
			lstIdx = i;
			break;
		}

		lstIdx = i;
	}

	if (varTable[((vdata.varHashes[lstIdx]) ^ (vdata.varHashes[lstIdx + 1])) / 2] == NULL) {
		return NULL;
	}

	printf("%d\n", ((vdata.varHashes[lstIdx]) ^ (vdata.varHashes[lstIdx + 1])) / 2);

	return varTable[((vdata.varHashes[lstIdx]) ^ (vdata.varHashes[lstIdx + 1])) / 2];
}


void var_destroy(struct Var** varTable, struct VarData* vdata) {
	for (int i = 0; i < vdata->varHashesSize; ++i) {
		free(varTable[vdata->varHashes[i]]);
		varTable[i] = NULL;
	}

	free(varTable);
	free(vdata->varHashes);
	vdata->varHashes = NULL;
}
