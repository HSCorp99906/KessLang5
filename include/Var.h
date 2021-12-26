#ifndef VAR_H
#define VAR_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define START_SIZE 10000


typedef enum DataType {
	INT,
} datatype_t;


struct Var {
	datatype_t datatype;
	void* value;
	char* key;
};


struct VarData {
	unsigned int* varHashes;
	size_t varHashesSize;
	unsigned int varHashesIdx;
};


unsigned int var_hash(const char* key, size_t size);
void var_init(struct Var** var, size_t size);
bool var_insert(struct Var* var, struct Var** varTable, struct VarData* varData, size_t size);
struct Var* var_locate(struct Var** varTable, struct VarData vdata, const char* key, size_t size);
void var_destroy(struct Var** varTable, struct VarData* vdata); 

#endif
