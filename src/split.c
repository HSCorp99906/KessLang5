#include "../include/split.h"


char* split(const char* string, const char* const DEL) {
	size_t strSize = 1;
	char* str = (char*)malloc(sizeof(char));

	static int i = 0;
	unsigned int strIdx = 0;

	static const char* prev_del;

	bool splitting = false;

	if (DEL != NULL) {
		i = 0;
		prev_del = DEL;
	}

	if (i == strlen(string)) {
		free(str);
		return NULL;
	}

	for (i; i < strlen(string); ++i) {
		if (string[i] == ' ') {
			splitting = true;
			continue;
		} else if (splitting) {
			break;
		}

		str[strIdx] = string[i];
		++strSize;
		++strIdx;
		str = (char*)realloc(str, sizeof(char) * strSize);
	}

	str[strIdx] = '\0';

	return str;
}

