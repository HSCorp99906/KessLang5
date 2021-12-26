#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../include/Token.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Runtime.h"


/*
 * Sefaults can be caused by including a line
 * and not making a token of that line.
 * Either make a token for that line or reject it.
 */


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    if (access(argv[1], F_OK) != 0) {
        printf("Cannot access file.\n");
        return 0;
    }

    size_t lineBufSize = 1;
    char* lineBuf = (char*)malloc(sizeof(char));

    size_t lbrSize = 1;
    char* lineBufRelease = (char*)malloc(sizeof(char));

    FILE* fp = fopen(argv[1], "r");

    struct Lexer lexer = {
        .lineNum = 0,
        .colNum = 0,
        .error = false,
    };

    toklist_t toklist;

    init_tokenlist(&toklist);

    unsigned long lbridx = 0;  // Line buffer release index.
    unsigned int curIdx = 0;
	unsigned long count = 0;  // This value fixed sefault.

	size_t splitBufSize = 1;
	char* splitBuffer = (char*)malloc(sizeof(char));
	unsigned long splitBufIdx = 0;
	bool firstRun = true;

    while (getline(&lineBuf, &lineBufSize, fp) != -1 && !(lexer.error)) {
		lexer.colNum = 0;
		unsigned int curIdx = 0;
		splitBufSize = 1;
		splitBufIdx = 0;
		bool started = true;

		if (!(firstRun)) {
			memset(splitBuffer, '\0', splitBufSize);
			splitBufSize = 1;
			splitBufIdx = 0;
			splitBuffer = (char*)realloc(splitBuffer, sizeof(char));
		}

		while (curIdx < strlen(lineBuf) - 1) {
			++lexer.lineNum;

			for (curIdx; curIdx < strlen(lineBuf); ++curIdx) {
				splitBuffer[splitBufIdx] = lineBuf[curIdx];
				++splitBufIdx;
				++splitBufSize;
				splitBuffer = (char*)realloc(splitBuffer, sizeof(char) * splitBufSize);
				splitBuffer[splitBufSize] = '\0';

				if (firstRun) {
					firstRun = false;
				}


				if (lineBuf[curIdx] == ';') {
					if (!(started)) {
						splitBuffer[0] = 0x08;
					} else {
						started = false;
					}

					splitBuffer[splitBufSize - 1] = '\0';
					splitBufIdx = 0;
					splitBufSize = 1;
					tokenize(&toklist, &lexer, splitBuffer);
					splitBuffer = (char*)realloc(splitBuffer, sizeof(char));
				}
			}
		}

		// tokenize(&toklist, &lexer, lineBuf);
    }

	free(splitBuffer);

	lexer.error = true;

    if (lexer.error) { 
        fclose(fp);
		free(lineBufRelease);
        destroy_tokenlist(&toklist);
        return -1;
    }

	struct Parser parser = {
		.tokenList = toklist,
		.curIndex = 0,
	};

	size_t nodelistSize = 0;

	for (int i = 0; i < count + 1; ++i) { 
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
