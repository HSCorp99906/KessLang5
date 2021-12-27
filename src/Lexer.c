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

	size_t value_size = 2;
	unsigned int value_idx = 0;
	char* value;

	bool tokenOk = true;
	bool start = true;

	while (lexer->colNum < strlen(line) && !(lexer->error)) {
		lexer->curChar = line[lexer->colNum];

		if (lexer->curChar == ' ') {
			++lexer->colNum;
			continue;
		} else if (lexer->curChar == '\\') {
			for (int i = lexer->colNum - 1; i > -1; --i) {
				if (line[i] == ' ') {
					continue;
				} else if (line[i] != ' ') {
					if (line[i] != ';') {
						printf("SyntaxError: Expected semicolon.\nLine %d\n", lexer->lineNum);
						lexer->error = true;
					}

					break;
				}
			}

			if (lexer->error) {
				break;
			}

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

			 if (quoteCount < 2 && quoteCount != 0) {
				printf("TokenError: Invalid use of quotes.\nLine %d\n", lexer->lineNum);
				lexer->error = true;
				break;
			} else if (quoteCount == 0) {
				printf("TokenError: Missing quotes.\nLine %d\n", lexer->lineNum);
				lexer->error = true;
				break;
			}
			
			alloc = false;
			print_inst = false;
			captureString = false;
		} else if (push_inst) {
			value = (char*)calloc(2, sizeof(char));
			bool getVarName = false;
			bool varNameIntAllowed = false;  // Is true when you are allowed to put int in varname.

			while (lexer->colNum < strlen(line) && !(lexer->error)) {
				lexer->curChar = line[lexer->colNum];


				if (getVarName) {	
					if (lexer->curChar == ';') {
						add_element(toklist, create_token(";", T_END_STATEMENT, false, false));
						break;
					}

					if (isint(lexer->curChar) && !(varNameIntAllowed)) {
						printf("SyntaxError: Invalid syntax.\nLine %d\n", lexer->lineNum);
						lexer->error = true;
						free(value);
						continue;
					} else if (!(isint(lexer->curChar)) && !(isalpha(lexer->curChar))) {
						printf("SyntaxError: Invalid syntax.\nLine %d\n", lexer->lineNum);
						lexer->error = true;
						free(value);
						continue;
					}

					value[value_idx] = lexer->curChar;
					++value_idx;
					++lexer->colNum;

					if (!(varNameIntAllowed)) {
						varNameIntAllowed = true;
					}

					continue;
				}

				if (lexer->curChar == ' ') {
					++lexer->colNum;
					continue;
				}

				switch (lexer->curChar) {
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
						if (forbidIntCapture) {
							printf("TokenError: Unexpected token.\nLine %d\n", lexer->lineNum);
							lexer->error = true;
							free(value);
							continue;
						}

						value[value_idx] = lexer->curChar;
						++value_idx;
						++value_size;
						value = (char*)realloc(value, sizeof(char) * value_size);
						break;
					case '=':
						add_element(toklist, create_token(value, T_INT, true, true));

						if (line[lexer->colNum + 1] == '>') {
							add_element(toklist, create_token("=>", T_ARROW, false, false));
						}
	
						forbidIntCapture = true;
						break;
					case '$':
						if (!(getVarName)) {
							memset(value, '\0', value_size);
							value_size = 2;
							value_idx = 0;
							value = (char*)realloc(value, sizeof(char) * value_size);
							getVarName = true;
						}
						break;
					default:
						if (!(forbidIntCapture)) {
							printf("TokenError: Unexpected value for 'push' instruction.\nLine %d\n", lexer->lineNum);
							lexer->error = true;
							free(value);
						}

						break;
				}	

				++lexer->colNum;
			}
	
			add_element(toklist, create_token(value, T_VAR, true, true));
			value_idx = 0;
			value_size = 2;
			push_inst = false;

			if (lexer->error) {
				break;
			}
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
			continue;
		} else if (strcmp(buffer, "push") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			push_inst = true;
			add_element(toklist, create_token("push", T_PUSH, false, false));
			continue;
		}

		buffer = (char*)realloc(buffer, sizeof(char) * bufsize);

		++lexer->colNum;
	}

	free(buffer);
}
