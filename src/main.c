#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../include/Token.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"


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

    unsigned long lbridx = 0;
    unsigned int curIdx = 0;

    while (getline(&lineBuf, &lineBufSize, fp) != -1 && !(lexer.error)) {
        while (curIdx < strlen(lineBuf) - 2) {
            for (curIdx; curIdx < strlen(lineBuf); ++curIdx) {
                lineBufRelease[lbridx] = lineBuf[curIdx];
                ++lbrSize;
                lineBufRelease = (char*)realloc(lineBufRelease, sizeof(char) * lbrSize);
                if (lineBuf[curIdx] == ';') {
					lineBufRelease[lbrSize] = '\0';
					printf("%s\n", lineBufRelease);
					tokenize(&toklist, &lexer, lineBufRelease);
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

	while (true) {
		struct AST_NODE** ast = parse(&parser, &nodelistSize);
		
		if (ast == NULL) {
			break;
		}

		ast_destroy(&ast, nodelistSize);
	}

    free(lineBuf);
    free(lineBufRelease);
    destroy_tokenlist(&toklist);
}
