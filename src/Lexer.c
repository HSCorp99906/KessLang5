#include "../include/Lexer.h"


void tokenize(toklist_t* toklist, struct Lexer* lexer, char* line) {

    char linecpy[strlen(line)];
    strcpy(linecpy, line);
    strcat(linecpy, " EOL");

    char* part = strtok(linecpy, " ");

	int lastIndex = 7;

	bool semicolonFound = false;

	for (int i = 0; i < strlen(line) - 2; ++i) {
		if (semicolonFound) {
			printf("SyntaxError: Invalid syntax.\nLine %d\n", lexer->lineNum);
			lexer->error = true;
			part = NULL;
			break;
		}

		if (line[i] == ';') {
			semicolonFound = true;
		}
	}

    while (part != NULL) {
        if (strcmp(part, "print") == 0) {
            size_t strsize = 1;
            char* str = (char*)malloc(sizeof(char));

            add_element(toklist, create_token("part", T_PRINT, false));

            unsigned int stridx = 0;

            for (int i = lastIndex; i < strlen(line) && line[i] != '"'; ++i) {
                str[stridx] = line[i];
                ++strsize;
                ++stridx;
                str = (char*)realloc(str, sizeof(char) * strsize);
            }

			int lineIndex = 0;

			str[stridx] = '\0';

            printf("%s\n", str);
            add_element(toklist, create_token("part", T_STR, false)); 

            str = NULL;
        } else if (strchr(part, '"') != NULL) {
			// Empty.
		} else if (strchr(part, ';') != NULL) {
			// Empty.
		} else if (strcmp(part, "EOL") == 0) {
			// Empty.
		} else {
			printf("InvalidSyntaxError: Invalid syntax.\nLine: %d\n", lexer->lineNum);
			lexer->error = true;
			break;
		}

        part = strtok(NULL, " ");
    }
}
