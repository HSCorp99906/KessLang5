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
	
	unsigned long bufIdx = 0;
	size_t bufsize = 1;
	char* buffer = (char*)malloc(sizeof(char));

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
	bool tokenOk = true;
	bool start = true;

	while (lexer->colNum < strlen(line) && !(lexer->error)) {
		lexer->curChar = line[lexer->colNum];

		if (lexer->curChar == ' ') {
			++lexer->colNum;
			continue;
		} else if (lexer->curChar == '\\') {
			++lexer->colNum;
			++lexer->lineNum;
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
					++quoteCount;
					if (!(quoteAllowed)) {
						print_inst = false;
						captureString = false;
						add_element(toklist, create_token(str, T_STR, false, true));
						break;
					}
					
					quoteAllowed = false;
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
				str = (char*)realloc(str, sizeof(char) * strSize);
			}

			 if (quoteCount < 2) {
				printf("TokenError: Invalid use of quotes.\nLine %d\n", lexer->lineNum);
				lexer->error = true;
				break;
			}
			
			alloc = false;
			print_inst = false;
			captureString = false;
		}
		
		buffer[bufIdx] = lexer->curChar;
		++bufIdx;
		++bufsize;
		// printf("%s\n", buffer);

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
			add_element(toklist, create_token("print", T_PRINT, false, false));
			tokenOk = true;
			continue;
		} else {
			tokenOk = false;
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
