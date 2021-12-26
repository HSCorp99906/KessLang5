#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


char* split(const char* restrict string, const char* restrict const DEL) {
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

	return str;
}


int main() {
	char str[] = "Hello,     World!";
	char* split1 = split(str, " ");
	printf("%s\n", split1);
	free(split1);
	split1 = split(str, NULL);
	printf("%s\n", split1);
	free(split1);
}
