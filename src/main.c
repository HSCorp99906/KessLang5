#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include "../include/Token.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Runtime.h"


/*
 * Sefaults can be caused by including a line
 * and not making a token of that line.
 * Either make a token for that line or reject it.
 */


size_t astBufSize;
struct AST_NODE*** astBuffer;
toklist_t toklist;

struct VarTable varTable;


void kill_process(int sigint) {
	for (int i = 0; i < astBufSize - 1; ++i) {
		ast_destroy(&astBuffer[i]);
	}

	for (int i = 0; i < varTable.size - 1; ++i) {
		free(varTable.vars[i]);
	}

	free(varTable.vars);
	varTable.vars = NULL;

	destroy_tokenlist(&toklist);
	exit(0);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    if (access(argv[1], F_OK) != 0) {
        printf("Cannot access file.\n");
        return 0;
    }

    FILE* fp = fopen(argv[1], "r");

    struct Lexer lexer = {
        .lineNum = 1,
        .colNum = 0,
        .error = false,
    };

    init_tokenlist(&toklist);

    unsigned long lbridx = 0;  // Line buffer release index.
    unsigned int curIdx = 0;
	unsigned long count = 0;  // This value fixed sefault.

	size_t splitBufSize = 1;

	char* buffer;

	fseek(fp, 0, SEEK_END);
	size_t filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = (char*)malloc(sizeof(char) * filesize);
	fread(buffer, filesize, 1, fp);

	uint32_t idxx = 0;
	
	for (int i = 0; i < strlen(buffer); ++i) {
		if (buffer[i] == '\n') {
			buffer[i] = '\\';
		} else if (buffer[i] != ' ') {
			++idxx;
		}
	}

	// Checks if file is empty.
	if (idxx == 0) {
		free(buffer);
		destroy_tokenlist(&toklist);
		exit(0);
	}
	
	tokenize(&toklist, &lexer, buffer);

	free(buffer);

	// lexer.error = true;

    if (lexer.error) { 
        fclose(fp);
        destroy_tokenlist(&toklist);
        return -1;
    }

	struct Parser parser = {
		.tokenList = toklist,
		.curIndex = 0,
		.error = false
	};

	size_t nodelistSize = 0;

	astBufSize = 1;
	uint32_t astBufIdx = 0;
	astBuffer = (struct AST_NODE***)malloc(sizeof(struct AST_NODE**));

	initVarTable(&varTable);

	signal(SIGINT, kill_process);

	while (true) { 
		/*
		 * Fixed segfault by subtracting element count by line count.
		 * No idea how that fixed it.
		 */

		if (parser.curIndex == toklist.elements - 1) {
			break;
		}

		struct AST_NODE** ast = parse(&parser, &nodelistSize);

		if (ast == NULL && parser.error) {
			for (int i = 0; i < astBufSize - 1; ++i) {
				ast_destroy(&astBuffer[i]);
			}

			destroy_tokenlist(&toklist);
			exit(1);

			break;
		} else if (ast == NULL) {
			break;
		}

		astBuffer[astBufIdx] = ast;
		++astBufIdx;
		++astBufSize;
		astBuffer = (struct AST_NODE***)realloc(astBuffer, sizeof(struct AST_NODE**) * astBufSize);
	}

	for (int i = 0; i < astBufSize - 1; ++i) {
		execute(astBuffer[i]);
	}

	kill_process(0);
	
}
