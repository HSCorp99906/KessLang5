#include "../include/Lexer.h"

#define KEYWORD_COUNT 2


char* peek(unsigned int index, char* line) {
	if (index >= strlen(line)) {
		return NULL;
	}

	return &line[index];
}


void tokenize(toklist_t* toklist, struct Lexer* lexer, char* line) {
	unsigned int quoteCount = 0;

	line[strlen(line)] = '\0';

	char keywords[2][25] = {
		"print",
		"push"
	};
	
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

	bool tokensOk = true;

	char lineCpy[strlen(line) + 4];
	strcpy(lineCpy, line);
	strcat(lineCpy, " EOL");
	lineCpy[sizeof(lineCpy)] = '\0';

	char* part = split(lineCpy, " ");
	
	/*
	bool check = true;
	while (check) {
		if (part == NULL) {
			break;
		}

		bool tokFound = false;

		for (int i = 0; i < strlen(part); ++i) {
			if (part[i] == ';') {
				part[i + 1] = 0x08;
				break;
			}
		}

		unsigned int idx = 0;
		check = false;
	}
	*/

	unsigned int idx = 0;

	bool check = true;

	while (part != NULL && check)  {
			if (strcmp(part, "EOL") == 0) {
				break;
			} else if (strchr(part, '"') != NULL) {
				break;
			}

			switch (part[0]) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					check = false;
					continue;
			}

			if (strcmp(part, keywords[idx]) != 0) {
				tokensOk = false;
	
				if (idx < KEYWORD_COUNT) {
					++idx;
				} else {
					idx = 0;
					free(part);
					part = split(lineCpy, NULL);
				}
			} else {
				tokensOk = true;
				break;
			}
	}

	free(part);
	part = NULL;

	if (!(tokensOk)) {
		printf("TokenError: Encountered an unexpected token.\nLine %d\n", lexer->lineNum);
		lexer->error = true;
	}

	/*********************************************
	 *            INSTRUCTIONS 
	 *********************************************/
	
	bool push_inst = false;
	bool pop_inst = false;
	bool print_inst = false;

	
	/*********************************************
	 *             CONDITIONS 
	 *********************************************/

	bool forbidIntCapture = false;
	bool captureString = false;
	bool alloc = false;

	/*********************************************
	 *              VALUES 
	 *********************************************/
	char* str;
	bool memoryOperatorCalled = false;  // '=>'

	while (lexer->colNum < strlen(line) && tokensOk) {
		lexer->curChar = line[lexer->colNum];

		if (lexer->curChar == ' ') {
			++lexer->colNum;
			continue;
		} else if (print_inst) {
			size_t strSize = 1;
			unsigned int strIdx = 0;

			if (!(alloc)) {
				alloc = true;
				str = (char*)calloc(1, sizeof(char));
			}

			bool quoteAllowed = true;
			unsigned short quoteCount = 0;

			while (lexer->colNum < strlen(line)) {
				lexer->curChar = line[lexer->colNum];

				if (lexer->curChar == '"') {
					if (!(quoteAllowed)) {
						printf("TERMINATED\n");
						print_inst = false;
						captureString = false;
						break;
					}
					
					quoteAllowed = false;
					++quoteCount;
					++lexer->colNum;
					continue;
				}

				if (quoteCount == 2) {
					break;
				}


				str[strIdx] = lexer->curChar;
				++strIdx;
				++strSize;
				++lexer->colNum;
				printf("%s\n", str);
				str = (char*)realloc(str, sizeof(char) * strSize);
			}
			
			alloc = false;
			print_inst = false;
			captureString = false;
			

			free(str);
		}
		
		buffer[bufIdx] = lexer->curChar;
		++bufIdx;
		++bufsize;

		if (lexer->curChar == ';') {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			continue;
		} else if (strcmp(buffer, "print") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			print_inst = true;
			captureString = true;
			continue;
		}

		buffer = (char*)realloc(buffer, sizeof(char) * bufsize);

		++lexer->colNum;
	}	

	if (push_inst && !(memoryOperatorCalled)) {
			printf("TokenError: Expected '=>' after 'push <value>'\nLine %d\n", lexer->lineNum);
			lexer->error = true;
	}

	free(buffer);
}
