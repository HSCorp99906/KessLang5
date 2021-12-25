#include "../include/Lexer.h"

#define KEYWORD_COUNT 2


char* peek(unsigned int index, char* line) {
	if (index >= strlen(line)) {
		return NULL;
	}

	return &line[index];
}


void tokenize(toklist_t* toklist, struct Lexer* lexer, char* line) {
	bool captureString = false;
	unsigned int quoteCount = 0;

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

	
	/*********************************************
	 *             CONDITIONS 
	 *********************************************/

	bool submitValue = false;  // Submits token value if this is true.
	bool forbidIntCapture = false;

	/*********************************************
	 *              VALUES 
	 *********************************************/

	bool memoryOperatorCalled = false;  // '=>'


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

		
				char* str = (char*)malloc(sizeof(char) * (bufsize + 1));  // This will be freed from another file.
				strcpy(str, buffer);
				str[bufsize - 1] = '\0';
				add_element(toklist, create_token(str, T_STR, false, true));
			}
		} else if (push_inst) {
			char valueBuf[strlen(buffer) + 2];
			unsigned int valueBufIdx = 0;
			bool breakLoop = true;

			for (int i = 0; i < strlen(buffer) && !(forbidIntCapture); ++i) {
				if (buffer[i] == '=' && buffer[i + 1] == '>') {
					breakLoop = false;
					memoryOperatorCalled = true;
				}
			}

			for (int i = 0; i < strlen(buffer) && !(breakLoop) && !(forbidIntCapture); ++i) {
				switch (buffer[i]) {
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
						valueBuf[valueBufIdx] = buffer[i];
						++valueBufIdx;
						break;
					default:
						submitValue = true;
						breakLoop = true;
						break;
				}
			}

			if (submitValue) {
				submitValue = false;
				forbidIntCapture = true;
				unsigned short val = atoi(valueBuf);
				printf("%d\n", val);
			}	
		}

		buffer[bufIdx] = lexer->curChar;
		++bufsize;
		++bufIdx;
		++lexer->colNum;
		buffer = (char*)realloc(buffer, sizeof(char) * bufsize);

		if (strcmp(buffer, "print") == 0) {
			captureString = true;
			bufsize = 1;
			bufIdx = 0;
			buffer = (char*)realloc(buffer, sizeof(char));
			add_element(toklist, create_token("print", T_PRINT, false, false));
		} else if (strcmp(buffer, "push") == 0) {
			memset(buffer, '\0', bufsize);
			push_inst = true;
			bufsize = 1;
			bufIdx = 0;
			buffer = (char*)realloc(buffer, sizeof(char));
			add_element(toklist, create_token("push", T_PUSH, false, false));
		}
	}

	if (push_inst && !(memoryOperatorCalled)) {
			printf("TokenError: Expected '=>' after 'push <value>'\nLine %d\n", lexer->lineNum);
			lexer->error = true;
	}

	free(buffer);
}
