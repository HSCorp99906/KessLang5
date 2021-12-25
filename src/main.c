#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../include/Token.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Runtime.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    if (access(argv[1], F_OK) != 0) {
        printf("Cannot access file.\n");
        return 0;
    }

    size_t lineBufSize = 5;
    char* lineBuf = (char*)malloc(sizeof(char) * lineBufSize);

    size_t lbrSize = 1;
    char* lineBufRelease = (char*)malloc(sizeof(char));

    FILE* fp = fopen(argv[1], "r");

    struct Lexer lexer = {
        .lineNum = 1,
        .colNum = 0,
        .error = false,
    };

    toklist_t toklist;

    init_tokenlist(&toklist);

    unsigned long lbridx = 0;  // Line buffer release index.
    unsigned int curIdx = 0;
	unsigned long count = 0;  // This value fixed sefault.

    while (getline(&lineBuf, &lineBufSize, fp) != -1 && !(lexer.error)) {
		if (strlen(lineBuf) - 1 == 0) {
			continue;
		}

        while (curIdx < strlen(lineBuf) - 2) {
            for (curIdx; curIdx < strlen(lineBuf); ++curIdx) {
                lineBufRelease[lbridx] = lineBuf[curIdx];
                ++lbrSize;
                lineBufRelease = (char*)realloc(lineBufRelease, sizeof(char) * lbrSize);
                if (lineBuf[curIdx] == ';') {
					lineBufRelease[lbrSize] = '\0';
					tokenize(&toklist, &lexer, lineBufRelease);
					++count;
                	lbridx = 0;
					lbrSize = 1;
					lineBufRelease = (char*)realloc(lineBufRelease, sizeof(char));
                    curIdx += 2;
                    break;
                } else {
					++lbridx;
				}

            }
        }

		curIdx = 0;
        ++lexer.lineNum;
    }

    if (lexer.error) { 
        fclose(fp);
        destroy_tokenlist(&toklist);
        return -1;
    }

	struct Parser parser = {
		.tokenList = toklist,
		.curIndex = 0,
	};

	size_t nodelistSize = 0;

	for (int i = 0; i < toklist.elements - count; ++i) { 
		/*
		 * Fixed segfault by subtracting element count by line count.
		 * No idea how that fixed it.
		 */

		struct AST_NODE** ast = parse(&parser, &nodelistSize);
		execute(ast);
		ast_destroy(&ast, nodelistSize);

	}

    free(lineBuf);
    free(lineBufRelease);
    destroy_tokenlist(&toklist);
}
