#include "../include/Var.h"


unsigned int hash(const char* key, size_t hashMapSize) {
	unsigned int checksum = 0;

	for (int i = 0; i < strlen(key); ++i) {
		checksum += (unsigned int)key[i] * 35;
	}

	return checksum % hashMapSize;
}


void init(struct Var** varTable, size_t size) {
	for (int i = 0; i < size; ++i) {
		varTable[i] = NULL;
	}
}


void insert(struct Var* var, struct Var** varTable, struct VarData* varData) {
	unsigned int index = hash(var->key);

	varData->varHashes[varData->varHashesIdx] = index;
	++varData->varHashesIdx;
	++varData->varHashesSize;
	varData->varHashes = (unsigned int*)realloc(sizeof(unsigned int) * varHashesSize);

	if (varTable[index] != NULL) {
		return false;
	}

	varTable[index] = var;
	return true;
}


struct Var* locate(struct Var** varTable, const char* key) {
	if (varTable[index] == NULL) {
		return NULL;
	}

	return varTable[index];
}


void destroy(struct Var** varTable, struct VarData* vdata) {
	for (int i = 0; i < vdata->varHashesSize; ++i) {
		free(varTable[vdata->varHashes[i]]);
		varTable[i] = NULL;
	}

	free(varTable);
	free(vdata->varHashes);
}
