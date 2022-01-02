#ifndef VAR_H
#define VAR_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


typedef enum DataType {
	INT,
	INT_PTR,
	STR,
} datatype_t;


struct Var {
	datatype_t datatype;
	char* key;
	void* value;
};


struct VarTable {
	struct Var** vars;
	size_t size;
	unsigned long varIdx;
}; 


void var_insert(struct VarTable* varTable, struct Var* var);
void initVarTable(struct VarTable* varTable);
struct Var* varLocate(struct VarTable varTable, const char* const KEY);

#endif
