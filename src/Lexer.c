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
	bool ignoreMissingSemicolon = false;
	bool captureVar = false;

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

		if (lexer->curChar == ' ' || lexer->curChar == '\t') {
			++lexer->colNum;
			continue;
		} else if (lexer->curChar == '\\') {
			for (int i = lexer->colNum - 1; i > -1; --i) {
				if (line[i] == ' ') {
					continue;
				} else if (line[i] != ' ') {
					if (line[i] != ';' && !(ignoreMissingSemicolon)) {
						printf("SyntaxError: Expected semicolon.\nLine %d\n", lexer->lineNum);
						lexer->error = true;
					} else if (ignoreMissingSemicolon) {
						ignoreMissingSemicolon = false;
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

			bool semicolon = false;

			while (lexer->colNum < strlen(line)) {
				lexer->curChar = line[lexer->colNum];
				if (lexer->curChar == '"') {
					++quoteCount;
					if (!(quoteAllowed)) {
						print_inst = false;
						captureString = false;
						add_element(toklist, create_token(str, T_STR, false, lexer->lineNum));
						free(str);
						break;
					}
					
					quoteAllowed = false;
					++lexer->colNum;
					continue;
				} else if (lexer->curChar == ';') {
					semicolon = true;
				}

				if (quoteCount == 2) {
					break;
				} else if (lexer->curChar == ';') {
					break;
				}

				str[strIdx] = lexer->curChar;
				++strIdx;
				++strSize;
				++lexer->colNum;
				str = (char*)realloc(str, sizeof(char) * strSize);
			}

			if (quoteCount == 0 && !(semicolon)) {
				free(str);
				printf("SyntaxError: Missing semicolon.\nLine %d\n", lexer->lineNum);
				lexer->error = true;
				break;
			}

			 if (quoteCount < 2 && quoteCount != 0) {
				free(str);
				printf("TokenError: Invalid use of quotes.\nLine %d\n", lexer->lineNum);
				lexer->error = true;
				break;
			} else if (quoteCount == 0) {
				bool raiseError = false;

				for (int j = 0; j < strlen(str); ++j) {
					if (str[j] == ' ' && str[j + 1] != ' ') {
						raiseError = true;
					}
				}

				if (raiseError) {
					free(str);
					printf("TokenError: Missing quotes.\nLine %d\n", lexer->lineNum);
					lexer->error = true;
					break;
				} else {
					add_element(toklist, create_token(str, T_VAR, false, lexer->lineNum));
					free(str);
				}
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
						add_element(toklist, create_token(";", T_END_STATEMENT, false, lexer->lineNum));
						break;
					}

					if (isint(lexer->curChar) && !(varNameIntAllowed)) {
						printf("SyntaxError: Invalid syntax.\nLine %d\n", lexer->lineNum);
						lexer->error = true;
						free(value);
						continue;
					} else if (!(isint(lexer->curChar)) && !(isalpha(lexer->curChar)) && lexer->curChar != '_') {
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
						add_element(toklist, create_token(value, T_INT, true, lexer->lineNum));

						if (line[lexer->colNum + 1] == '>') {
							add_element(toklist, create_token("=>", T_ARROW, false, lexer->lineNum));
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
	
			add_element(toklist, create_token(value, T_VAR, true, lexer->lineNum));
			value_idx = 0;
			value_size = 2;
			push_inst = false;
			add_element(toklist, create_token(";", T_END_STATEMENT, false, lexer->lineNum));
			free(value);

			if (lexer->error) {
				break;
			}
		} else if (captureVar) {
				size_t varKeySize = 2;
				unsigned int varKeyIdx = 0;
				char* varKey = (char*)calloc(2, sizeof(char));

				while (lexer->colNum < strlen(line)) {
					lexer->curChar = line[lexer->colNum];

					if (lexer->curChar == ';' || lexer->curChar == ' ') {
						break;
					}

					 varKey[varKeyIdx] = lexer->curChar;
					 ++varKeyIdx;
					 ++varKeySize;
					 varKey = (char*)realloc(varKey, sizeof(char) * varKeySize);
					 ++lexer->colNum;
				}


				bool collectValue = false;
				size_t varValueSize = 2;
				unsigned int varValueIdx = 0;
				char* varValue = (char*)calloc(2, sizeof(char));

				bool isAnInt = false;
				bool assignment = false;

				while (lexer->colNum < strlen(line)) {
					lexer->curChar = line[lexer->colNum];
	

					if (lexer->curChar == ';') {
						break;
					}

					
					if (lexer->curChar == '=') {
						if (line[lexer->colNum + 1] != '=') {
							assignment = true;	
						}
					} else if (isint(lexer->curChar) && !(collectValue)) {
						collectValue = true;
						varValue[varValueIdx] = lexer->curChar;
						++varValueIdx;
						++varValueSize;
						varValue = (char*)realloc(varValue, sizeof(char) * varValueSize);
					} else if (collectValue) {
						varValue[varValueIdx] = lexer->curChar;
						++varValueIdx;
						++varValueSize;
						varValue = (char*)realloc(varValue, sizeof(char) * varValueSize);
					}

					++lexer->colNum;
				}

				if (isint(*varValue)) {
					isAnInt = true;
				}

				add_element(toklist, create_token(varKey, T_VAR, false, lexer->lineNum));
				
				// Added '=' token to token list.
				if (assignment) {
					add_element(toklist, create_token("=", T_EQUALS, false, lexer->lineNum));
				}

				// Added value to list.
				if (isAnInt) {
					add_element(toklist, create_token(varValue, T_INT, true, lexer->lineNum));
				}

				captureVar = false;
				free(varKey);
				free(varValue);
			}
		
		// BUFFER UPDATE
		buffer[bufIdx] = lexer->curChar;
		++bufIdx;
		++bufsize;

		// VARIABLE CHECK
		if (lexer->curChar == '$' && line[lexer->colNum + 1] == ' ') {
			printf("SyntaxError: Invalid Syntax.\nLine %d\n", lexer->lineNum);
		} else if (lexer->curChar == '$') {
			memset(buffer, '\0', bufsize);
			captureVar = true;
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			continue;
		}

		// TOKEN CHECKS
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
			add_element(toklist, create_token("print", T_PRINT, false, lexer->lineNum));
			continue;
		} else if (strcmp(buffer, "push") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			push_inst = true;
			add_element(toklist, create_token("push", T_PUSH, false, lexer->lineNum));
			continue;
		} else if (strcmp(buffer, "if") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("if", T_IF_STATEMENT, false, lexer->lineNum));
			continue;
		} else if (strcmp(buffer, "do") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("do", T_DO, false, lexer->lineNum));
			ignoreMissingSemicolon = true;
			continue;
		} else if (strcmp(buffer, "true") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("true", T_TRUE, false, lexer->lineNum));
			continue;
		} else if (strcmp(buffer, "false") == 0) {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("false", T_FALSE, false, lexer->lineNum));
			continue;
		} else if (strcmp(buffer, "end") == 0) { 
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("end", T_END, false, lexer->lineNum));
			continue;
		} else if (lexer->curChar == '+') {
			memset(buffer, '\0', bufsize);
			bufIdx = 0;
			bufsize = 1;
			buffer = (char*)realloc(buffer, sizeof(char));
			++lexer->colNum;
			add_element(toklist, create_token("+", T_PLUS, false, lexer->lineNum));
			continue;
		}

		buffer = (char*)realloc(buffer, sizeof(char) * bufsize);

		++lexer->colNum;
	}

	free(buffer);
}
