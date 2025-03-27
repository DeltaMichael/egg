%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
	#include "interpreter.h"
	#include "util.h"
%}

%union {
    char* str;
    int d;
	CMD* cmd;
	CMD** cmds;
}

%token <str> WORD
%token PIPE
%token EOL

%type <cmd> command
%type <cmds> sequence

%%

seqlist:
       | seqlist sequence EOL {
	   							CMD** seq = $2;
								while(*seq != NULL) {
									char** args = cmd_get(*seq);
									while(*args != NULL) {
										printf("%s ", *args);
										args++;
									}
									printf("\n");
									seq++;
								}
	   							char* current_dir = get_current_dir();
								printf("(%s (EGG> ", current_dir);
							  }
       ;

sequence: sequence PIPE command { CMD** pointer = $1;
								  while(*pointer != NULL) {
									pointer++;
								  }
								  *pointer = $3;
								  $$ = $1;
								}
        | command { CMD** out = calloc(256, sizeof(CMD*)); out[0] = $1; $$ = out; }
        ;

command: command WORD { cmd_append($1, $2); $$ = $1; free($2); }
       | WORD { CMD* out = cmd_init(); cmd_append(out, $1); $$ = out; free($1); }
       ;

%%

int main(int argc, char** argv) {
	system("clear");
	char* current_dir = get_current_dir();
	printf("(%s (EGG> ", current_dir);
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
