#include "../include/Lexer.h"


char* peek(unsigned int index, char* line) {
	return &line[index];
}


void tokenize(toklist_t* toklist, struct Lexer* lexer, char* line) {
	char* keywords[] = {
		"print",
	};


	bool captureString = false;
	unsigned int quoteCount = 0;
	
	/*
	bool captureSpace = true;
	unsigned int spaceCount = 0;

	for (int i = 0; i < strlen(line); ++i) {
		if (line[i] == ' ' && captureSpace) {
			captureSpace = false;
			++spaceCount;
		} else if (line[i] != ' ') {
			captureSpace = true;
		}
	}

	printf("%d\n", spaceCount);
	*/
	
	unsigned long bufIdx = 0;
	size_t bufsize = 1;
	char* buffer = (char*)malloc(sizeof(char));
	
	size_t bufsize1 = 1;
	unsigned long bufIdx1 = 0;
	char* checkBuffer = (char*)malloc(sizeof(char));

	bool tokensOk = true;

	for (int i = 0; i < strlen(line); ++i) {
		if (line[i] != ' ') {
			checkBuffer[bufIdx1] = line[i];
			++bufsize1;
			++bufIdx1;
			checkBuffer = (char*)realloc(checkBuffer, sizeof(char) * bufsize1);
		}

		if (line[i] == ' ') {
			if (strcmp(checkBuffer, "print") != 0) {
				tokensOk = false;
			}

			bufIdx = 0;
			bufsize1 = 2;
			checkBuffer = (char*)realloc(checkBuffer, sizeof(char) * bufsize);
		}
	}

	free(checkBuffer);

	if (!(tokensOk)) {
		printf("UnrecognizedTokenError: Encountered an unexpected token.\nLine %d\n", lexer->lineNum);
		lexer->error = true;
	}

	while (lexer->colNum < strlen(line) && tokensOk) {
		lexer->curChar = line[lexer->colNum];

		if (lexer->curChar == ' ' && !(captureString)) {
			++lexer->colNum;
			continue;
		} else if (captureString) {
			if (lexer->curChar == '"') {
				++quoteCount;
			}

			if (quoteCount == 2) {
				captureString = false;
				buffer[0] = 0x08;
				buffer[1] = 0x08;

		
				char* str = (char*)malloc(sizeof(char) * (bufsize + 1));
				strcpy(str, buffer);
				str[bufsize - 1] = '\0';
				add_element(toklist, create_token(str, T_STR, false, true));
			}
		}

		buffer[bufIdx] = lexer->curChar;
		++bufsize;
		++bufIdx;
		buffer = (char*)realloc(buffer, sizeof(char) * bufsize);

		if (strcmp(buffer, "print") == 0) {
			captureString = true;
			bufsize = 1;
			bufIdx = 0;
			buffer = (char*)realloc(buffer, sizeof(char));
			add_element(toklist, create_token("print", T_PRINT, false, false));
		}

		++lexer->colNum;
	}

	free(buffer);
}
