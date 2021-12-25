#ifndef VAR_H
#define VAR_H

#include <stdlib.h>
#include <string.h>

#define START_SIZE 50


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


unsigned int hash(const char* key, size_t hashMapSize);
void init(struct Var* var);
bool insert(struct Var* var, struct Var** varTable, unsigned int* varHashes);
struct Var* locate(struct Var** varTable, const char* key);
void destroy(struct Var** varTable, struct VarData* vdata); 

#endif
