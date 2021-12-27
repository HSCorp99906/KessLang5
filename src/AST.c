#include "../include/AST.h"


struct AST_NODE** get_tree() {
	struct AST_NODE** ret_val = (struct AST_NODE**)calloc(START_SIZE, sizeof(struct AST_NODE*));
	return ret_val;
}


void ast_init(struct AST_NODE** nodes) {
	struct AST_NODE* curNode;
	struct AST_NODE* tmpNode;

	for (int i = 0; i < START_SIZE; ++i) {
		nodes[i] = NULL;
		curNode = nodes[i];
	}
}


uint32_t ast_hash(char* key) {
	uint32_t checksum = 0;

	for (int i = 0; i < strlen(key); ++i) {
		checksum += key[i] * 55 % START_SIZE;
	}

	return checksum;
}


void init_node(struct AST_NODE* node, char* key, char* value, int32_t valueINT, bool uvi) {
	node->key = key;
	node->value = value;
	node->valueINT = valueINT;
	node->usingValueINT = uvi;
	node->checksum = ast_hash(key);
	node->isChild = false;
}


bool ast_insert(struct AST_NODE** dest, struct AST_NODE* src, size_t* s) {
	uint32_t index = src->checksum;

	if (dest[index] != NULL) {
		return false;
	}

	dest[index] = src;
	*s += 1;
	return true;
}


struct AST_NODE* ast_locate(struct AST_NODE** node, char key[]) {
	if (node[ast_hash(key)] != NULL) {
		if (node[ast_hash(key)]->checksum == ast_hash(key)) {
			return node[ast_hash(key)];
		}

		return NULL;
	}
}


void ast_destroy(struct AST_NODE*** nodes) {
	free(ast_locate(*nodes, "type")->child);
	free(ast_locate(*nodes, "type"));

	free(*nodes);
	*nodes = NULL;
}
